OS := $(shell uname)

ifeq ($(OS), Darwin)
	CLIBS= 		-framework OpenGL -framework GLUT # 2014
	CFLAGS=		$(CLIBS)
else # Linux
	CLIBS= 		-lm -lglut -lGL -lGLU -lX11 # 2014
	CPATHS=		-I/usr/include -L/usr/lib # 2014
	CFLAGS=		$(CPATHS)$(CLIBS)
endif

mytexture:	mytexture.cpp glm.c glm.h
			g++ mytexture.cpp glm.c -o mytexture -w $(CFLAGS)

multTexture:	multTexture.cpp glm.c glm.h
			g++ multTexture.cpp glm.c -o multTexture -w $(CFLAGS)

lightAndMaterial1:	lightAndMaterial1.cpp
			g++ lightAndMaterial1.cpp -o lightAndMaterial1 -w $(CFLAGS)

clean:
			rm *.o