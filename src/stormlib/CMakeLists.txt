cmake_minimum_required(VERSION 3.10)

# Project name
project(stormlib)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Usage to toggle between using static or dynamic runtime libraries:
# cmake -DUSE_MT=ON ..
# cmake -DUSE_MT=OFF ..
option(USE_MT "Use static runtime library (/MT, /MTd)" ON)

# Compiler options
if(MSVC)
  add_definitions(-D__SYS_ZLIB -D_7ZIP_ST -DWIN32 -D_WINDOWS -D_USRDLL)
  # MSVC specific flags
  add_compile_options(/W1)
  add_definitions(-D_WINDOWS -D_USRDLL)
  
  # Configure name dynamically
    if(WIN64)
	  if(USE_MT)
		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		  add_definitions(-D_DEBUG)
		  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
		  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
		  set(NAME_SUFFIX "AS")
		else()
		  add_definitions(-DNDEBUG)
		  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
		  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
		  set(NAME_SUFFIX "AS")
		endif()
	  else()
		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		  add_definitions(-D_DEBUG)
		  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
		  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MDd")
		  set(NAME_SUFFIX "AD")
		else()
		  add_definitions(-DNDEBUG)
		  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
		  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MD")
		  set(NAME_SUFFIX "AD")
		endif()
	  endif()
		
	else()
	
	  if(USE_MT)
		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		  add_definitions(-D_DEBUG)
		  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
		  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MTd")
		  set(NAME_SUFFIX "AS")
		else()
		  add_definitions(-DNDEBUG)
		  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
		  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MT")
		  set(NAME_SUFFIX "AS")
		endif()
	  else()
		if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		  add_definitions(-D_DEBUG)
		  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
		  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /MDd")
		  set(NAME_SUFFIX "AD")
		else()
		  add_definitions(-DNDEBUG)
		  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
		  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /MD")
		  set(NAME_SUFFIX "AD")
		endif()
	  endif()	
  endif()
  
else()
	add_definitions(-D__SYS_ZLIB)
	add_definitions(-D_7ZIP_ST)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
endif()

# Include directories
if (MSVC)
	include_directories(
	  src
	  src/zlib
	)
else()
	include_directories(src)
endif()

# Source files
if (MSVC)
	set(SOURCES
		src/adpcm/adpcm.cpp
		src/bzip2/blocksort.c
		src/bzip2/bzlib.c
		src/bzip2/compress.c
		src/bzip2/crctable.c
		src/bzip2/decompress.c
		src/bzip2/huffman.c
		src/bzip2/randtable.c
		src/FileStream.cpp
		src/huffman/huff.cpp
		src/jenkins/lookup3.c
		src/libtomcrypt/src/hashes/hash_memory.c
		src/libtomcrypt/src/hashes/md5.c
		src/libtomcrypt/src/hashes/sha1.c
		src/libtomcrypt/src/math/ltm_desc.c
		src/libtomcrypt/src/math/multi.c
		src/libtomcrypt/src/math/rand_prime.c
		src/libtomcrypt/src/misc/base64_decode.c
		src/libtomcrypt/src/misc/crypt_argchk.c
		src/libtomcrypt/src/misc/crypt_find_hash.c
		src/libtomcrypt/src/misc/crypt_find_prng.c
		src/libtomcrypt/src/misc/crypt_hash_descriptor.c
		src/libtomcrypt/src/misc/crypt_hash_is_valid.c
		src/libtomcrypt/src/misc/crypt_libc.c
		src/libtomcrypt/src/misc/crypt_ltc_mp_descriptor.c
		src/libtomcrypt/src/misc/crypt_prng_descriptor.c
		src/libtomcrypt/src/misc/crypt_prng_is_valid.c
		src/libtomcrypt/src/misc/crypt_register_hash.c
		src/libtomcrypt/src/misc/crypt_register_prng.c
		src/libtomcrypt/src/misc/zeromem.c
		src/libtomcrypt/src/pk/asn1/der_decode_bit_string.c
		src/libtomcrypt/src/pk/asn1/der_decode_boolean.c
		src/libtomcrypt/src/pk/asn1/der_decode_choice.c
		src/libtomcrypt/src/pk/asn1/der_decode_ia5_string.c
		src/libtomcrypt/src/pk/asn1/der_decode_integer.c
		src/libtomcrypt/src/pk/asn1/der_decode_object_identifier.c
		src/libtomcrypt/src/pk/asn1/der_decode_octet_string.c
		src/libtomcrypt/src/pk/asn1/der_decode_printable_string.c
		src/libtomcrypt/src/pk/asn1/der_decode_sequence_ex.c
		src/libtomcrypt/src/pk/asn1/der_decode_sequence_flexi.c
		src/libtomcrypt/src/pk/asn1/der_decode_sequence_multi.c
		src/libtomcrypt/src/pk/asn1/der_decode_short_integer.c
		src/libtomcrypt/src/pk/asn1/der_decode_utctime.c
		src/libtomcrypt/src/pk/asn1/der_decode_utf8_string.c
		src/libtomcrypt/src/pk/asn1/der_length_bit_string.c
		src/libtomcrypt/src/pk/asn1/der_length_boolean.c
		src/libtomcrypt/src/pk/asn1/der_length_ia5_string.c
		src/libtomcrypt/src/pk/asn1/der_length_integer.c
		src/libtomcrypt/src/pk/asn1/der_length_object_identifier.c
		src/libtomcrypt/src/pk/asn1/der_length_octet_string.c
		src/libtomcrypt/src/pk/asn1/der_length_printable_string.c
		src/libtomcrypt/src/pk/asn1/der_length_sequence.c
		src/libtomcrypt/src/pk/asn1/der_length_short_integer.c
		src/libtomcrypt/src/pk/asn1/der_length_utctime.c
		src/libtomcrypt/src/pk/asn1/der_length_utf8_string.c
		src/libtomcrypt/src/pk/asn1/der_sequence_free.c
		src/libtomcrypt/src/pk/ecc/ltc_ecc_map.c
		src/libtomcrypt/src/pk/ecc/ltc_ecc_mul2add.c
		src/libtomcrypt/src/pk/ecc/ltc_ecc_mulmod.c
		src/libtomcrypt/src/pk/ecc/ltc_ecc_points.c
		src/libtomcrypt/src/pk/ecc/ltc_ecc_projective_add_point.c
		src/libtomcrypt/src/pk/ecc/ltc_ecc_projective_dbl_point.c
		src/libtomcrypt/src/pk/pkcs1/pkcs_1_mgf1.c
		src/libtomcrypt/src/pk/pkcs1/pkcs_1_oaep_decode.c
		src/libtomcrypt/src/pk/pkcs1/pkcs_1_pss_decode.c
		src/libtomcrypt/src/pk/pkcs1/pkcs_1_v1_5_decode.c
		src/libtomcrypt/src/pk/rsa/rsa_exptmod.c
		src/libtomcrypt/src/pk/rsa/rsa_free.c
		src/libtomcrypt/src/pk/rsa/rsa_import.c
		src/libtomcrypt/src/pk/rsa/rsa_make_key.c
		src/libtomcrypt/src/pk/rsa/rsa_verify_hash.c
		src/libtomcrypt/src/pk/rsa/rsa_verify_simple.c
		src/libtommath/bncore.c
		src/libtommath/bn_fast_mp_invmod.c
		src/libtommath/bn_fast_mp_montgomery_reduce.c
		src/libtommath/bn_fast_s_mp_mul_digs.c
		src/libtommath/bn_fast_s_mp_mul_high_digs.c
		src/libtommath/bn_fast_s_mp_sqr.c
		src/libtommath/bn_mp_2expt.c
		src/libtommath/bn_mp_abs.c
		src/libtommath/bn_mp_add.c
		src/libtommath/bn_mp_addmod.c
		src/libtommath/bn_mp_add_d.c
		src/libtommath/bn_mp_and.c
		src/libtommath/bn_mp_clamp.c
		src/libtommath/bn_mp_clear.c
		src/libtommath/bn_mp_clear_multi.c
		src/libtommath/bn_mp_cmp.c
		src/libtommath/bn_mp_cmp_d.c
		src/libtommath/bn_mp_cmp_mag.c
		src/libtommath/bn_mp_cnt_lsb.c
		src/libtommath/bn_mp_copy.c
		src/libtommath/bn_mp_count_bits.c
		src/libtommath/bn_mp_div.c
		src/libtommath/bn_mp_div_2.c
		src/libtommath/bn_mp_div_2d.c
		src/libtommath/bn_mp_div_3.c
		src/libtommath/bn_mp_div_d.c
		src/libtommath/bn_mp_dr_is_modulus.c
		src/libtommath/bn_mp_dr_reduce.c
		src/libtommath/bn_mp_dr_setup.c
		src/libtommath/bn_mp_exch.c
		src/libtommath/bn_mp_exptmod.c
		src/libtommath/bn_mp_exptmod_fast.c
		src/libtommath/bn_mp_expt_d.c
		src/libtommath/bn_mp_exteuclid.c
		src/libtommath/bn_mp_fread.c
		src/libtommath/bn_mp_fwrite.c
		src/libtommath/bn_mp_gcd.c
		src/libtommath/bn_mp_get_int.c
		src/libtommath/bn_mp_grow.c
		src/libtommath/bn_mp_init.c
		src/libtommath/bn_mp_init_copy.c
		src/libtommath/bn_mp_init_multi.c
		src/libtommath/bn_mp_init_set.c
		src/libtommath/bn_mp_init_set_int.c
		src/libtommath/bn_mp_init_size.c
		src/libtommath/bn_mp_invmod.c
		src/libtommath/bn_mp_invmod_slow.c
		src/libtommath/bn_mp_is_square.c
		src/libtommath/bn_mp_jacobi.c
		src/libtommath/bn_mp_karatsuba_mul.c
		src/libtommath/bn_mp_karatsuba_sqr.c
		src/libtommath/bn_mp_lcm.c
		src/libtommath/bn_mp_lshd.c
		src/libtommath/bn_mp_mod.c
		src/libtommath/bn_mp_mod_2d.c
		src/libtommath/bn_mp_mod_d.c
		src/libtommath/bn_mp_montgomery_calc_normalization.c
		src/libtommath/bn_mp_montgomery_reduce.c
		src/libtommath/bn_mp_montgomery_setup.c
		src/libtommath/bn_mp_mul.c
		src/libtommath/bn_mp_mulmod.c
		src/libtommath/bn_mp_mul_2.c
		src/libtommath/bn_mp_mul_2d.c
		src/libtommath/bn_mp_mul_d.c
		src/libtommath/bn_mp_neg.c
		src/libtommath/bn_mp_n_root.c
		src/libtommath/bn_mp_or.c
		src/libtommath/bn_mp_prime_fermat.c
		src/libtommath/bn_mp_prime_is_divisible.c
		src/libtommath/bn_mp_prime_is_prime.c
		src/libtommath/bn_mp_prime_miller_rabin.c
		src/libtommath/bn_mp_prime_next_prime.c
		src/libtommath/bn_mp_prime_rabin_miller_trials.c
		src/libtommath/bn_mp_prime_random_ex.c
		src/libtommath/bn_mp_radix_size.c
		src/libtommath/bn_mp_radix_smap.c
		src/libtommath/bn_mp_rand.c
		src/libtommath/bn_mp_read_radix.c
		src/libtommath/bn_mp_read_signed_bin.c
		src/libtommath/bn_mp_read_unsigned_bin.c
		src/libtommath/bn_mp_reduce.c
		src/libtommath/bn_mp_reduce_2k.c
		src/libtommath/bn_mp_reduce_2k_l.c
		src/libtommath/bn_mp_reduce_2k_setup.c
		src/libtommath/bn_mp_reduce_2k_setup_l.c
		src/libtommath/bn_mp_reduce_is_2k.c
		src/libtommath/bn_mp_reduce_is_2k_l.c
		src/libtommath/bn_mp_reduce_setup.c
		src/libtommath/bn_mp_rshd.c
		src/libtommath/bn_mp_set.c
		src/libtommath/bn_mp_set_int.c
		src/libtommath/bn_mp_shrink.c
		src/libtommath/bn_mp_signed_bin_size.c
		src/libtommath/bn_mp_sqr.c
		src/libtommath/bn_mp_sqrmod.c
		src/libtommath/bn_mp_sqrt.c
		src/libtommath/bn_mp_sub.c
		src/libtommath/bn_mp_submod.c
		src/libtommath/bn_mp_sub_d.c
		src/libtommath/bn_mp_toom_mul.c
		src/libtommath/bn_mp_toom_sqr.c
		src/libtommath/bn_mp_toradix.c
		src/libtommath/bn_mp_toradix_n.c
		src/libtommath/bn_mp_to_signed_bin.c
		src/libtommath/bn_mp_to_signed_bin_n.c
		src/libtommath/bn_mp_to_unsigned_bin.c
		src/libtommath/bn_mp_to_unsigned_bin_n.c
		src/libtommath/bn_mp_unsigned_bin_size.c
		src/libtommath/bn_mp_xor.c
		src/libtommath/bn_mp_zero.c
		src/libtommath/bn_prime_tab.c
		src/libtommath/bn_reverse.c
		src/libtommath/bn_s_mp_add.c
		src/libtommath/bn_s_mp_exptmod.c
		src/libtommath/bn_s_mp_mul_digs.c
		src/libtommath/bn_s_mp_mul_high_digs.c
		src/libtommath/bn_s_mp_sqr.c
		src/libtommath/bn_s_mp_sub.c
		src/lzma/C/LzFind.c
		src/lzma/C/LzFindMt.c
		src/lzma/C/LzmaDec.c
		src/lzma/C/LzmaEnc.c
		src/lzma/C/Threads.c
		src/pklib/explode.c
		src/pklib/implode.c
		src/SBaseCommon.cpp
		src/SBaseFileTable.cpp
		src/SCompression.cpp
		src/SFileAddFile.cpp
		src/SFileAttributes.cpp
		src/SFileCompactArchive.cpp
		src/SFileCreateArchive.cpp
		src/SFileExtractFile.cpp
		src/SFileFindFile.cpp
		src/SFileListFile.cpp
		src/SFileOpenArchive.cpp
		src/SFileOpenFileEx.cpp
		src/SFilePatchArchives.cpp
		src/SFileReadFile.cpp
		src/SFileVerify.cpp
		src/sparse/sparse.cpp
		src/zlib/adler32.c
		src/zlib/compress2.c
		src/zlib/crc32.c
		src/zlib/deflate.c
		src/zlib/inffast.c
		src/zlib/inflate.c
		src/zlib/inftrees.c
		src/zlib/trees.c
		src/zlib/zutil.c
		stormlib_dll/DllMain.c
	)
else()
	set(SOURCES
		src/adpcm/adpcm.cpp
		src/huffman/huff.cpp
		src/sparse/sparse.cpp
		src/FileStream.cpp
		src/SBaseCommon.cpp
		src/SCompression.cpp
		src/SFileAddFile.cpp
		src/SFileAttributes.cpp
		src/SFileCompactArchive.cpp
		src/SFileCreateArchive.cpp
		src/SFileExtractFile.cpp
		src/SFileFindFile.cpp
		src/SFileListFile.cpp
		src/SFileOpenArchive.cpp
		src/SFileOpenFileEx.cpp
		src/SFilePatchArchives.cpp
		src/SFileReadFile.cpp
		src/SBaseFileTable.cpp
		src/SFileVerify.cpp
		# C source files
		src/jenkins/lookup3.c
		src/zlib/deflate.c
		src/zlib/trees.c
		src/zlib/crc32.c
		src/zlib/zutil.c
		src/zlib/adler32.c
		src/zlib/inflate.c
		src/zlib/inftrees.c
		src/zlib/inffast.c
		src/zlib/compress2.c
		# libtomcrypt C source files
		src/libtomcrypt/src/hashes/hash_memory.c
		src/libtomcrypt/src/hashes/md5.c
		src/libtomcrypt/src/hashes/sha1.c
		src/libtomcrypt/src/math/ltm_desc.c
		src/libtomcrypt/src/math/multi.c
		src/libtomcrypt/src/math/rand_prime.c
		src/libtomcrypt/src/misc/base64_decode.c
		src/libtomcrypt/src/misc/crypt_argchk.c
		src/libtomcrypt/src/misc/crypt_find_hash.c
		src/libtomcrypt/src/misc/crypt_find_prng.c
		src/libtomcrypt/src/misc/crypt_hash_descriptor.c
		src/libtomcrypt/src/misc/crypt_hash_is_valid.c
		src/libtomcrypt/src/misc/crypt_libc.c
		src/libtomcrypt/src/misc/crypt_ltc_mp_descriptor.c
		src/libtomcrypt/src/misc/crypt_prng_descriptor.c
		src/libtomcrypt/src/misc/crypt_prng_is_valid.c
		src/libtomcrypt/src/misc/crypt_register_hash.c
		src/libtomcrypt/src/misc/crypt_register_prng.c
		src/libtomcrypt/src/misc/zeromem.c
		src/libtomcrypt/src/pk/asn1/der_decode_bit_string.c
		src/libtomcrypt/src/pk/asn1/der_decode_boolean.c
		src/libtomcrypt/src/pk/asn1/der_decode_choice.c
		src/libtomcrypt/src/pk/asn1/der_decode_ia5_string.c
		src/libtomcrypt/src/pk/asn1/der_decode_integer.c
		src/libtomcrypt/src/pk/asn1/der_decode_object_identifier.c
		src/libtomcrypt/src/pk/asn1/der_decode_octet_string.c
		src/libtomcrypt/src/pk/asn1/der_decode_printable_string.c
		src/libtomcrypt/src/pk/asn1/der_decode_sequence_ex.c
		src/libtomcrypt/src/pk/asn1/der_decode_sequence_flexi.c
		src/libtomcrypt/src/pk/asn1/der_decode_sequence_multi.c
		src/libtomcrypt/src/pk/asn1/der_decode_short_integer.c
		src/libtomcrypt/src/pk/asn1/der_decode_utctime.c
		src/libtomcrypt/src/pk/asn1/der_decode_utf8_string.c
		src/libtomcrypt/src/pk/asn1/der_length_bit_string.c
		src/libtomcrypt/src/pk/asn1/der_length_boolean.c
		src/libtomcrypt/src/pk/asn1/der_length_ia5_string.c
		src/libtomcrypt/src/pk/asn1/der_length_integer.c
		src/libtomcrypt/src/pk/asn1/der_length_object_identifier.c
		src/libtomcrypt/src/pk/asn1/der_length_octet_string.c
		src/libtomcrypt/src/pk/asn1/der_length_printable_string.c
		src/libtomcrypt/src/pk/asn1/der_length_sequence.c
		src/libtomcrypt/src/pk/asn1/der_length_short_integer.c
		src/libtomcrypt/src/pk/asn1/der_length_utctime.c
		src/libtomcrypt/src/pk/asn1/der_length_utf8_string.c
		src/libtomcrypt/src/pk/asn1/der_sequence_free.c
		src/libtomcrypt/src/pk/ecc/ltc_ecc_map.c
		src/libtomcrypt/src/pk/ecc/ltc_ecc_mul2add.c
		src/libtomcrypt/src/pk/ecc/ltc_ecc_mulmod.c
		src/libtomcrypt/src/pk/ecc/ltc_ecc_points.c
		src/libtomcrypt/src/pk/ecc/ltc_ecc_projective_add_point.c
		src/libtomcrypt/src/pk/ecc/ltc_ecc_projective_dbl_point.c
		src/libtomcrypt/src/pk/pkcs1/pkcs_1_mgf1.c
		src/libtomcrypt/src/pk/pkcs1/pkcs_1_oaep_decode.c
		src/libtomcrypt/src/pk/pkcs1/pkcs_1_pss_decode.c
		src/libtomcrypt/src/pk/pkcs1/pkcs_1_v1_5_decode.c
		src/libtomcrypt/src/pk/rsa/rsa_exptmod.c
		src/libtomcrypt/src/pk/rsa/rsa_free.c
		src/libtomcrypt/src/pk/rsa/rsa_import.c
		src/libtomcrypt/src/pk/rsa/rsa_make_key.c
		src/libtomcrypt/src/pk/rsa/rsa_verify_hash.c
		src/libtomcrypt/src/pk/rsa/rsa_verify_simple.c
		# libtommath C source files
		src/libtommath/bncore.c
		src/libtommath/bn_fast_mp_invmod.c
		src/libtommath/bn_fast_mp_montgomery_reduce.c
		src/libtommath/bn_fast_s_mp_mul_digs.c
		src/libtommath/bn_fast_s_mp_mul_high_digs.c
		src/libtommath/bn_fast_s_mp_sqr.c
		src/libtommath/bn_mp_2expt.c
		src/libtommath/bn_mp_abs.c
		src/libtommath/bn_mp_add.c
		src/libtommath/bn_mp_addmod.c
		src/libtommath/bn_mp_add_d.c
		src/libtommath/bn_mp_and.c
		src/libtommath/bn_mp_clamp.c
		src/libtommath/bn_mp_clear.c
		src/libtommath/bn_mp_clear_multi.c
		src/libtommath/bn_mp_cmp.c
		src/libtommath/bn_mp_cmp_d.c
		src/libtommath/bn_mp_cmp_mag.c
		src/libtommath/bn_mp_cnt_lsb.c
		src/libtommath/bn_mp_copy.c
		src/libtommath/bn_mp_count_bits.c
		src/libtommath/bn_mp_div.c
		src/libtommath/bn_mp_div_2.c
		src/libtommath/bn_mp_div_2d.c
		src/libtommath/bn_mp_div_3.c
		src/libtommath/bn_mp_div_d.c
		src/libtommath/bn_mp_dr_is_modulus.c
		src/libtommath/bn_mp_dr_reduce.c
		src/libtommath/bn_mp_dr_setup.c
		src/libtommath/bn_mp_exch.c
		src/libtommath/bn_mp_exptmod.c
		src/libtommath/bn_mp_exptmod_fast.c
		src/libtommath/bn_mp_expt_d.c
		src/libtommath/bn_mp_exteuclid.c
		src/libtommath/bn_mp_fread.c
		src/libtommath/bn_mp_fwrite.c
		src/libtommath/bn_mp_gcd.c
		src/libtommath/bn_mp_get_int.c
		src/libtommath/bn_mp_grow.c
		src/libtommath/bn_mp_init.c
		src/libtommath/bn_mp_init_copy.c
		src/libtommath/bn_mp_init_multi.c
		src/libtommath/bn_mp_init_set.c
		src/libtommath/bn_mp_init_set_int.c
		src/libtommath/bn_mp_init_size.c
		src/libtommath/bn_mp_invmod.c
		src/libtommath/bn_mp_invmod_slow.c
		src/libtommath/bn_mp_is_square.c
		src/libtommath/bn_mp_jacobi.c
		src/libtommath/bn_mp_karatsuba_mul.c
		src/libtommath/bn_mp_karatsuba_sqr.c
		src/libtommath/bn_mp_lcm.c
		src/libtommath/bn_mp_lshd.c
		src/libtommath/bn_mp_mod.c
		src/libtommath/bn_mp_mod_2d.c
		src/libtommath/bn_mp_mod_d.c
		src/libtommath/bn_mp_montgomery_calc_normalization.c
		src/libtommath/bn_mp_montgomery_reduce.c
		src/libtommath/bn_mp_montgomery_setup.c
		src/libtommath/bn_mp_mul.c
		src/libtommath/bn_mp_mulmod.c
		src/libtommath/bn_mp_mul_2.c
		src/libtommath/bn_mp_mul_2d.c
		src/libtommath/bn_mp_mul_d.c
		src/libtommath/bn_mp_neg.c
		src/libtommath/bn_mp_n_root.c
		src/libtommath/bn_mp_or.c
		src/libtommath/bn_mp_prime_fermat.c
		src/libtommath/bn_mp_prime_is_divisible.c
		src/libtommath/bn_mp_prime_is_prime.c
		src/libtommath/bn_mp_prime_miller_rabin.c
		src/libtommath/bn_mp_prime_next_prime.c
		src/libtommath/bn_mp_prime_rabin_miller_trials.c
		src/libtommath/bn_mp_prime_random_ex.c
		src/libtommath/bn_mp_radix_size.c
		src/libtommath/bn_mp_radix_smap.c
		src/libtommath/bn_mp_rand.c
		src/libtommath/bn_mp_read_radix.c
		src/libtommath/bn_mp_read_signed_bin.c
		src/libtommath/bn_mp_read_unsigned_bin.c
		src/libtommath/bn_mp_reduce.c
		src/libtommath/bn_mp_reduce_2k.c
		src/libtommath/bn_mp_reduce_2k_l.c
		src/libtommath/bn_mp_reduce_2k_setup.c
		src/libtommath/bn_mp_reduce_2k_setup_l.c
		src/libtommath/bn_mp_reduce_is_2k.c
		src/libtommath/bn_mp_reduce_is_2k_l.c
		src/libtommath/bn_mp_reduce_setup.c
		src/libtommath/bn_mp_rshd.c
		src/libtommath/bn_mp_set.c
		src/libtommath/bn_mp_set_int.c
		src/libtommath/bn_mp_shrink.c
		src/libtommath/bn_mp_signed_bin_size.c
		src/libtommath/bn_mp_sqr.c
		src/libtommath/bn_mp_sqrmod.c
		src/libtommath/bn_mp_sqrt.c
		src/libtommath/bn_mp_sub.c
		src/libtommath/bn_mp_submod.c
		src/libtommath/bn_mp_sub_d.c
		src/libtommath/bn_mp_toom_mul.c
		src/libtommath/bn_mp_toom_sqr.c
		src/libtommath/bn_mp_toradix.c
		src/libtommath/bn_mp_toradix_n.c
		src/libtommath/bn_mp_to_signed_bin.c
		src/libtommath/bn_mp_to_signed_bin_n.c
		src/libtommath/bn_mp_to_unsigned_bin.c
		src/libtommath/bn_mp_to_unsigned_bin_n.c
		src/libtommath/bn_mp_unsigned_bin_size.c
		src/libtommath/bn_mp_xor.c
		src/libtommath/bn_mp_zero.c
		src/libtommath/bn_prime_tab.c
		src/libtommath/bn_reverse.c
		src/libtommath/bn_s_mp_add.c
		src/libtommath/bn_s_mp_exptmod.c
		src/libtommath/bn_s_mp_mul_digs.c
		src/libtommath/bn_s_mp_mul_high_digs.c
		src/libtommath/bn_s_mp_sqr.c
		src/libtommath/bn_s_mp_sub.c
		# LZMA and PKLIB C source files
		src/lzma/C/LzFind.c
		src/lzma/C/LzmaDec.c
		src/lzma/C/LzmaEnc.c
		src/pklib/explode.c
		src/pklib/implode.c
	)
endif()

# Define the shared and static library targets
add_library(stormlib SHARED ${SOURCES})
add_library(stormlib_static STATIC ${SOURCES})

# Link libraries
if (MSVC)
	#target_link_libraries(stormlib PRIVATE bz2 z)
  
	# Specify subsystem and target machine for linker
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")
	target_link_options(stormlib PRIVATE "/MACHINE:X64")
	#target_link_options(stormlib PRIVATE "/MACHINE:X86")
	
	# Generator expressions for different output file names and destinations
	#set_target_properties(stormlib_static PROPERTIES
	#	ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
	#	OUTPUT_NAME_DEBUG "${PROJECT_NAME}DAD"
	#	OUTPUT_NAME_RELEASE "${PROJECT_NAME}RAD"
	#)

	# If you want to handle 32-bit and 64-bit builds differently (done at the top name configuration)
	# Example: cmake .. -DWIN64=ON
	#if(WIN64)
	#	set_target_properties(stormlib_static PROPERTIES
	#		OUTPUT_NAME_DEBUG "${PROJECT_NAME}DAD_x64"
	#		OUTPUT_NAME_RELEASE "${PROJECT_NAME}RAD_x64"
	#	)
	#endif()
	
	# Set name dynamically
	set_target_properties(stormlib_static PROPERTIES
	  OUTPUT_NAME_DEBUG "${PROJECT_NAME}D${NAME_SUFFIX}"
	  OUTPUT_NAME_RELEASE "${PROJECT_NAME}R${NAME_SUFFIX}"
	)

	# Handling Module Definition Files for DLLs
	set_target_properties(stormlib PROPERTIES
		WINDOWS_EXPORT_ALL_SYMBOLS TRUE
		LINK_FLAGS "/DEF:${CMAKE_SOURCE_DIR}/stormlib_dll/StormLib.def"
	)
  
	set_target_properties(stormlib_static PROPERTIES
		WINDOWS_EXPORT_ALL_SYMBOLS TRUE
		LINK_FLAGS "/DEF:${CMAKE_SOURCE_DIR}/stormlib_dll/StormLib.def"
	)
	
else()
	target_link_libraries(stormlib bz2 z)
	target_link_libraries(stormlib_static bz2 z)
endif()

# Installation rules
install(TARGETS stormlib stormlib_static
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)
install(FILES src/stormlib.h src/StormPort.h DESTINATION include/stormlib)

# MSVC-specific options
if(MSVC)
  set_target_properties(stormlib PROPERTIES
    OUTPUT_NAME "StormLib"
    WINDOWS_EXPORT_ALL_SYMBOLS TRUE
  )
  
  # Set MSVC toolset version
  #set(CMAKE_GENERATOR_TOOLSET "v143" CACHE STRING "Platform Toolset" FORCE)
endif()
