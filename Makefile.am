noinst_HEADERS = WallBall.h SoundManager.h GUIManager.h Simulator.h NetworkManager.h PaddleManager.h BallManager.h

bin_PROGRAMS = WallBall
WallBall_CPPFLAGS = -I$(top_srcdir)
WallBall_SOURCES = WallBall.cpp SoundManager.cpp GUIManager.cpp Simulator.cpp NetworkManager.cpp PaddleManager.cpp BallManager.cpp
WallBall_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS) $(SDL_CFLAGS) $(SDL_mixer_CFLAGS) $(SDL_net_CFLAGS)
WallBall_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS) $(SDL_LIBS) $(SDL_mixer_LIBS) $(SDL_net_LIBS)

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign