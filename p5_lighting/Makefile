OS := $(shell uname)

ifeq ($(OS), Darwin)
	CLIBS= 		-framework OpenGL -framework GLUT # 2014
	CFLAGS=		$(CLIBS)
else # Linux
	CLIBS= 		-lm -lglut -lGL -lGLU -lX11 # 2014
	CPATHS=		-I/usr/include -L/usr/lib # 2014
	CFLAGS=		$(CPATHS)$(CLIBS)
endif

landscape:	lighting.cpp classes.cpp classes.h baseClasses.cpp baseClasses.h
			g++ lighting.cpp classes.cpp baseClasses.cpp -o Delfino_Jensen_G4 -w $(CFLAGS)

clean:
			rm *.o