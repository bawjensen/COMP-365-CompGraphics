OS := $(shell uname)

ifeq ($(OS), Darwin)
	CLIBS= 		-framework OpenGL -framework GLUT # 2014
	CFLAGS=		$(CLIBS)
else # Linux
	CLIBS= 		-lm -lglut -lGL -lGLU -lX11 # 2014
	CPATHS=		-I/usr/include -L/usr/lib # 2014
	CFLAGS=		$(CPATHS)$(CLIBS)
endif

wheatonMap:		wheatonMap.cpp
			g++ wheatonMap.cpp classes.cpp textures.cpp -o JensenG1 $(CFLAGS)

clean:
			rm *.o