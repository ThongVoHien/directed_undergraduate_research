OPTS = -Wall -g -std=c++11 
LIBS = -lGL -lglut -lm
ARCH := $(shell uname)
ifeq ($(ARCH), Linux)
else
 MACOSX_DEFINE = -DMACOSX -I/sw/include
 LIBS = -I/usr/common/include -I/usr/include/GL -L/System/Library/Frameworks/OpenGL.framework/Libraries -framework GLUT -framework OpenGL -lGL -lm -lobjc -lstdc++

endif

drawSBcurve:	drawSBcurve.o
	g++ $(OPTS) -o drawSBcurve drawSBcurve.o $(LIBS)

drawSBcurve.o:	drawSBcurve.cpp
	g++ $(OPTS) $(MACOSX_DEFINE) -c drawSBcurve.cpp

clean:
	rm -f *.o drawSBcurve