OS := $(shell uname)

ifeq ($(OS), Darwin)
	CLIBS= 		-framework OpenGL -framework GLUT # 2014
	CFLAGS=		$(CLIBS)
else # Linux
	CLIBS= 		-lm -lglut -lGL -lGLU -lX11 # 2014
	CPATHS=		-I/usr/include -L/usr/lib # 2014
	CFLAGS=		$(CPATHS)$(CLIBS)
endif

spline:		terrainSpline.cpp
			g++ terrainSpline.cpp classes.cpp -o JensenG3 -w $(CFLAGS)

testing:		testing.cpp
			g++ testing.cpp -o testing -w $(CFLAGS)

clean:
			rm *.o