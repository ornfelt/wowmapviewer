#include "mymenu.h"
#include "mpq.h"
#include "test.h"
#include "dbcfile.h"

#include <fstream>

using namespace std;

enum MAPID {
  MAP_AZEROTH=0,
  MAP_KALIMDOR=1,
  MAP_EXPANSION01=530,
  MAP_NORTHREND=571
};

MyMenu::MyMenu()
{	
  DBCFile f("DBFilesClient\\Map.dbc");
  f.open();
  int y=0;
  int x=5;
	
  for(DBCFile::Iterator i=f.begin(); i!=f.end(); ++i) {
    MapEntry e;
    int size;

    size = 16;
    e.font = f16;
    e.id = i->getInt(0);
    e.name = i->getString(1);
    e.description = i->getString(3); // + locale
    if (e.description == "")
      e.description = e.name;

    if (!e.name.find("Transport") || !e.name.find("Test") || !e.name.find("test"))
      continue;

    if (e.name == "ScottTest" || e.name == "ExteriorTest" || e.name == "QA_DVD" 
        || e.name == "CraigTest" || e.name == "development_nonweighted"
        || e.name == "development")
      continue;

    e.x0 = x;
    e.y0 = y;
    /*
      if (e.id==MAP_AZEROTH || e.id==MAP_KALIMDOR || e.id==MAP_EXPANSION01 || e.id==MAP_NORTHREND) {
      size = 24;
      e.font = f24;
      }
    */

    e.y1 = e.y0 + size;
    y += size;

    if ((y+25) >= video.yres) {
      x += 160; // map name length
      y = 0;
    }

    e.x1 = e.x0 + e.font->textwidth(e.name.c_str());
		
    maps.push_back(e);

  }

  sel = -1;
  cmd = 0;
  world = 0;

  mt = 0;

  refreshBookmarks();

  setpos = true;
  ah = -90.0f;
  av = -30.0f;

  bg = 0;
  lastbg = -1;
  //randBackground();
}

MyMenu::~MyMenu() {
  delete bg;
}

void MyMenu::InitWorld(int x, int y) {
  if (fullscreen) SDL_ShowCursor(SDL_DISABLE);

  gWorld = world;
  world->initDisplay();
  // calc coordinates

  if (setpos) {
    cz=0;
    cx=0;

    if (world->nMaps > 0) {
      float fx = (x/12.0f);
      float fz = (y/12.0f);

      cx = (int)fx;
      cz = (int)fz;

      world->camera = Vec3D(fx * TILESIZE,0,fz * TILESIZE);
      world->autoheight = true;
    } else {
      Vec3D p;
      if (world->gwmois.size()>=1) p = world->gwmois[0].pos;
      else p = Vec3D(0,0,0); // empty map? :|
				
      cx = (int) (p.x / TILESIZE);
      cz = (int) (p.z / TILESIZE);

      world->camera = p + Vec3D(0,25.0f,0);
    }
    world->lookat = world->camera + Vec3D(0,0,-1.0f);

    ah = -90.0f;
    av = -30.0f;
  }

  world->enterTile(cx,cz);
  Test *t = new Test(world,ah,av);
  gStates.push_back(t);

  sel = -1;
  world = 0;

  //delete bg;
  bg = 0;
}

void MyMenu::randBackground() {
  if (bg) delete bg;
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
	
  char *ui[] = {"MainMenu", "NightElf", "Human", "Dwarf", 
                "Orc", "Tauren", "Scourge"}; 
  //, "Bloodelf", "Deathknight", "Draenei" };
  int dark[] = {0,0,1,1,0,0,0,0,1,1};
  int randnum;
  do {
    randnum = randint(0,6);
  } while (randnum == lastbg);
  //randnum = 0;
  char *randui = ui[randnum];
  darken = dark[randnum]!=0;
  char path[256];
  sprintf(path, "Interface\\Glues\\Models\\UI_%s\\UI_%s.mdx", randui, randui);
	
  bg = new Model(path);
  bg->ind = true;
  lastbg = randnum;
}

void MyMenu::tick(float t, float dt) {
  mt += dt * 1000.0f;
  globalTime = (int)mt;
  if (bg) bg->updateEmitters(dt);

  //if (bg==0) randBackground();

  if (gWorld == 0) {
    int32 index = find_index("ZulAman");
    LoadWorld(index);
    InitWorld(354, 377);
    // cmd = CMD_SELECT_MINIMAP;
  }

  if (cmd==CMD_DO_LOAD_WORLD) {
    InitWorld(x, y);
    cmd = CMD_BACK_TO_MENU;
  } else if (cmd == CMD_BACK_TO_MENU) {
    /*if (fullscreen)*/ SDL_ShowCursor(SDL_ENABLE);
    cmd = CMD_SELECT;
    refreshBookmarks();
    setpos = true;
    gWorld = 0;

    // reentry
    //randBackground();
  }
}

void MyMenu::display(float t, float dt) {
  video.clearScreen();
  glDisable(GL_FOG);
  //video.set3D();
  if (bg) {
    Vec4D la(0.1f,0.1f,0.1f,1);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, la);

    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glColor4f(1,1,1,1);
    for (int i=0; i<8; i++) {
      GLuint light = GL_LIGHT0 + i;
      glLightf(light, GL_CONSTANT_ATTENUATION, 0);
      glLightf(light, GL_LINEAR_ATTENUATION, 0.7f);
      glLightf(light, GL_QUADRATIC_ATTENUATION, 0.03f);
      glDisable(light);
    }
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_LIGHTING);
    bg->cam.setup(globalTime);
    bg->draw();
  }

  video.set2D();
  glEnable(GL_BLEND);
  glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_DEPTH_TEST);
  glDisable(GL_CULL_FACE);
  glDisable(GL_LIGHTING);

  if (darken) {
    glDisable(GL_TEXTURE_2D);
    // background is too light so tame it down a bit
    glColor4f(0,0,0,0.35f);
    glBegin(GL_QUADS);
    glVertex2i(0,0);
    glVertex2i(video.xres,0);
    glVertex2i(video.xres,video.yres);
    glVertex2i(0,video.yres);
    glEnd();
  }

  glColor4f(1,1,1,1);

  glEnable(GL_TEXTURE_2D);

  int basex = 200;
  int basey = 0;
  int tilesize = 12;

  if (cmd==CMD_LOAD_WORLD) {
    const char *loadstr = "Loading...";

    //f32->shprint(400, 360, "Loading...");

    f32->shprint(video.xres/2 - f32->textwidth(loadstr)/2, video.yres/2-16, loadstr);

    cmd = CMD_DO_LOAD_WORLD;
  } else if (cmd==CMD_SELECT_MINIMAP) {
    if ((sel != -1) && (world!=0)) {

      if (world->minimap) {
        // minimap time! ^_^
        const int len = 768;
        glColor4f(1,1,1,1);
        glBindTexture(GL_TEXTURE_2D, world->minimap);
        glBegin(GL_QUADS);
        glTexCoord2f(0,0);
        glVertex2i(basex,basey);
        glTexCoord2f(1,0);
        glVertex2i(basex+len,basey);
        glTexCoord2f(1,1);
        glVertex2i(basex+len,basey+len);
        glTexCoord2f(0,1);
        glVertex2i(basex,basey+len);
        glEnd();
      }

      glDisable(GL_TEXTURE_2D);
      for (int j=0; j<64; j++) {
        for (int i=0; i<64; i++) {
          if (world->maps[j][i]) {
            glColor4f(0.7f,0.9f,0.8f,0.2f);
            glBegin(GL_QUADS);
            glVertex2i(basex+i*tilesize, basey+j*tilesize);
            glVertex2i(basex+(i+1)*tilesize, basey+j*tilesize);
            glVertex2i(basex+(i+1)*tilesize, basey+(j+1)*tilesize);
            glVertex2i(basex+i*tilesize, basey+(j+1)*tilesize);
            glEnd();
          }
        }
      }
      glEnable(GL_TEXTURE_2D);

      glColor4f(1,1,1,1);
      if (world->nMaps == 0) {
        f16->shprint(400, 360, "Click to enter");
      } else {
        f16->shprint(400, 0, "Select your starting point");
      }
    }
  } else if (cmd==CMD_SELECT) {
    {
      // intro text
      glColor4f(1,1,1,1);
      /*
        f32->shprint(300,0,"World of Warcraft map viewer");
        //f16->print(380,40,"reverse engineered and written by ufoz");
        f16->shprint(380,40,"http://code.google.com/p/wowmapviewer/");
      */

      f16->shprint(video.xres - 20 - f16->textwidth(APP_VERSION), 10, APP_VERSION);
			
      f24->shprint(680, 74, "Controls");
      f16->shprint(620,110, "F1 - toggle models\n"
                   "F2 - toggle doodads\n"
                   "F3 - toggle terrain\n"
                   "F4 - toggle stats\n"
                   "F5 - save bookmark\n"
                   "F6 - toggle map objects\n"
                   "H - disable highres terrain\n"
                   "I - toggle invert mouse\n"
                   "M - minimap\n"
                   "Esc - back/exit\n"
                   "WASD - move\n"
                   "R - quick 180 degree turn\n"
                   "F - toggle fog\n"
                   "+,- - adjust fog distance\n"
                   "O,P - slower/faster movement\n"
                   "B,N - slower/faster time\n"
                   );

#ifdef SFMPQAPI
      f16->shprint(300, video.yres - 40, "World of Warcraft is (C) Blizzard Entertainment\n"
                   "Uses SFmpqapi, (C) ShadowFlare Software"
                   );
#else
      f16->shprint(160*3, video.yres - 60, "World of Warcraft map viewer\nhttp://code.google.com/p/wowmapviewer/\nWorld of Warcraft is (C) Blizzard Entertainment");
#endif
      /*
        f24->shprint(360, 74, "Bookmarks");
        for (unsigned int i=0; i<bookmarks.size(); i++) {
        f16->shdrawtext(bookmarks[i].x0, bookmarks[i].y0, bookmarks[i].label.c_str());
        }
      */

    }

    for (unsigned int i=0; i<maps.size(); i++) {
      if (i==sel)
        glColor4f(0,1,1,1);
      else
        glColor4f(1,1,1,1);
      maps[i].font->shprint(maps[i].x0, maps[i].y0, maps[i].name.c_str());
    }

    glColor4f(1,1,1,1);
    if (sel != -1) {
      f32->shprint(video.xres/2-f32->textwidth(maps[sel].description.c_str())/2, video.yres-40, maps[sel].description.c_str());
    }
  }
}

void MyMenu::keypressed(SDL_KeyboardEvent *e) {
  if (e->type == SDL_KEYDOWN) {
    if (e->keysym.sym == SDLK_ESCAPE) {
      gPop = true;
    }
  }
}

void MyMenu::mousemove(SDL_MouseMotionEvent *e) {
}

void MyMenu::mouseclick(SDL_MouseButtonEvent *e)
{
  //int y = e->y;
  //unsigned int s = y / 16;
  //if (s < maps.size()) sel = s;

  if (cmd != CMD_SELECT && cmd != CMD_SELECT_MINIMAP) 
    return;

  //gLog("key press %x, %x\n", e->x, e->y);
  // key down, key up twice trigger this
  if (e->x == last_key_x && e->y == last_key_y && e->type != last_key_type) {
    last_key_type = e->type;
    return;
  }
  last_key_x = e->x;
  last_key_y = e->y;
  last_key_type = e->type;

  if (cmd == CMD_SELECT) {
    int32 index = GetWorldIndexFromPos(e->x, e->y);
    if (index >= 0) {
      LoadWorld(index);
      cmd = CMD_SELECT_MINIMAP;
    } else {
      sel = -1;
    }
  } else if (cmd == CMD_SELECT_MINIMAP) {
    if (sel!=-1 && world !=0) {
      x = e->x - 200;
      y = e->y;
      cmd = CMD_LOAD_WORLD;
    } else {
      cmd = CMD_SELECT;
    }
  }
}

int32 MyMenu::GetWorldIndexFromPos(int x, int y) {
  for (uint32 i=0; i<maps.size(); i++) {
    if (maps[i].hit(x, y)) {
      return i;
    }
  }

  return -1;
}

void MyMenu::LoadWorld(int32 index) {
  if (index >= 0 && index != sel) {
    if (world)
      delete world;
	sel = index;
    world = new World(maps[sel].name.c_str(), maps[sel].id);
    cmd = CMD_LOAD_WORLD;
  } else {
    sel = -1;
    if (world)
      delete world;
  }
}

int32 MyMenu::find_index(const std::string& name) {
  for (unsigned int i=0; i<maps.size(); i++) {
    if (maps[i].name == name) {
      return i;
    }
  }

  return -1;
}


void MyMenu::refreshBookmarks() {
  return;

  bookmarks.clear();
  ifstream f("bookmarks.txt");
  if(!f.is_open())
    // No bookmarks file
    return;

  int y = 110;
  const int x = 400;

  while (!f.eof()) {
    Bookmark b;
    f >> b.basename >> b.mapid >> b.pos.x >> b.pos.y >> b.pos.z >> b.ah >> b.av;
    if (f.eof()) break;

    char c;
    do {
      f >> c;
    } while (c == ' ');
    b.name = "";
    char cc[2];
    cc[0] = c;
    cc[1] = 0;
    b.name.append(cc); // oh shit there must be a nicer way to do this
    char buf[256];
    f.getline(buf, 256);
    b.name.append(buf);

    // check for the basename
    bool mapfound = false;
    for (unsigned int i=0; i<maps.size(); i++) {
      if (maps[i].name == b.basename) {
        mapfound = true;
        break;
      }
    }
    if (!mapfound) continue;

    sprintf(buf,"(%s) %s", b.basename.c_str(), b.name.c_str());
    b.label = buf;

    b.x0 = x;
    b.x1 = x + f16->textwidth(b.label.c_str());
    b.y0 = y;
    b.y1 = y+16;
    y += 16;

    bookmarks.push_back(b);
  }
  f.close();
}

