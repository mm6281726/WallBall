noinst_HEADERS = WallBall.h

bin_PROGRAMS = WallBall
WallBall_CPPFLAGS = -I$(top_srcdir)
WallBall_SOURCES = WallBall.cp
WallBall_CXXFLAGS = $(OGRE_CFLAGS) $(OIS_CFLAGS) $(bullet_CFLAGS)
WallBall_LDADD = $(OGRE_LIBS) $(OIS_LIBS) $(bullet_LIBS)

EXTRA_DIST = buildit makeit
AUTOMAKE_OPTIONS = foreign