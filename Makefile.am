noinst_HEADERS = WallBall.h SoundManager.h Simulator.h

bin_PROGRAMS = WallBall
WallBall_CPPFLAGS = -I$(top_srcdir)
WallBall_SOURCES = WallBall.cpp SoundManager.cpp Simulator.cpp
WallBall_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(SDL_CFLAGS)
WallBall_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(SDL_LIBS)

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign
