#ifndef DBCFILE_H
#define DBCFILE_H
#include <cassert>
#include <string>
#include "wowmapview.h"

#ifdef __GNUC__
#define WARN_IF_UNUSED __attribute__ ((warn_unused_result))
#else
#define WARN_IF_UNUSED
#endif

class DBCFile
{
public:
	DBCFile(const std::string &filename);
	~DBCFile();

	// Open database. It must be openened before it can be used.
	bool open() WARN_IF_UNUSED;

	// Database exceptions
	class Exception
	{
	public:
		Exception(const std::string &message): message(message)
		{ }
		virtual ~Exception()
		{ }
		const std::string &getMessage() {return message;}
	private:
		std::string message;
	};
	class NotFound: public Exception
	{
	public:
		NotFound(): Exception("Key was not found")
		{ }
	};
	// Iteration over database
	class Iterator;
	class Record
	{
	public:
		Record& operator= (const Record& r)
		{
            file = r.file;
			offset = r.offset;
			return *this;
		}
		float getFloat(size_t field) const
		{
			assert(field < file.fieldCount);
			return *reinterpret_cast<float*>(offset+field*4);
		}
		unsigned int getUInt(size_t field) const
		{
			assert(field < file.fieldCount);
			return *reinterpret_cast<unsigned int*>(offset+(field*4));
		}
		int getInt(size_t field) const
		{
			assert(field < file.fieldCount);
			return *reinterpret_cast<int*>(offset+field*4);
		}
		unsigned char getByte(size_t ofs) const
		{
			assert(ofs < file.recordSize);
			return *reinterpret_cast<unsigned char*>(offset+ofs);
		}
		const char *getString(size_t field) const
		{
			assert(field < file.fieldCount);
			size_t stringOffset = getUInt(field);
			if (stringOffset >= file.stringSize)
				stringOffset = 0;
			assert(stringOffset < file.stringSize);
			return reinterpret_cast<char*>(file.stringTable + stringOffset);
		}
		const char *getLocalizedString( size_t field, size_t locale = -1 ) const
		{
			size_t loc = locale;
			if( locale == -1 )
			{
				assert(field < file.fieldCount -  8 );
				for( loc = 0; loc < 8; loc++ )
				{
					size_t stringOffset = getUInt(field + loc);
					if( stringOffset != 0 )
						break;
				}
			}
			
			assert( field + loc < file.fieldCount );
			size_t stringOffset = getUInt( field + loc );
			assert( stringOffset < file.stringSize );
			return reinterpret_cast<char*>( file.stringTable + stringOffset );
		}
	private:
		DBCFile &file;
		unsigned char *offset;
		Record(DBCFile &file, unsigned char *offset): file(file), offset(offset) {}

		friend class DBCFile;
		friend class Iterator;
	};
	/** Iterator that iterates over records
	*/
	class Iterator
	{
	public:
		Iterator(DBCFile &file, unsigned char *offset): 
			record(file, offset) {}
		/// Advance (prefix only)
		Iterator & operator++() { 
			record.offset += record.file.recordSize;
			return *this; 
		}	
		/// Return address of current instance
		Record const & operator*() const { return record; }
		const Record* operator->() const {
			return &record;
		}
		/// Comparison
		bool operator==(const Iterator &b) const
		{
			return record.offset == b.record.offset;
		}
		bool operator!=(const Iterator &b) const
		{
			return record.offset != b.record.offset;
		}
	private:
		Record record;
	};

	// Get record by id
	Record getRecord(size_t id);
	/// Get begin iterator over records
	Iterator begin();
	/// Get begin iterator over records
	Iterator end();
	/// Trivial
	size_t getRecordCount() const { return recordCount;}
	size_t getFieldCount() const { return fieldCount; }
	Record getByID( unsigned int id, size_t field = 0 ) 
	{
		for( Iterator i = begin( ); i!=end( ); ++i )
		{
			if( i->getUInt( field ) == id )
				return ( *i );
		}
		throw NotFound( );
	}
	int getRecordSize() { return recordSize; }
	int getRecordCount() { return recordCount; }
	int getFieldCount() { return fieldCount; }
	int getStringSize() { return stringSize; }
private:
	std::string filename;
	size_t recordSize;
	size_t recordCount;
	size_t fieldCount;
	size_t stringSize;
	unsigned char *data;
	unsigned char *stringTable;
};

#endif
