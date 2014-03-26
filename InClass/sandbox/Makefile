OS := $(shell uname)

ifeq ($(OS), Darwin)
	CLIBS= 		-framework OpenGL -framework GLUT # 2014
	CFLAGS=		$(CLIBS)
else # Linux
	CLIBS= 		-lm -lglut -lGL -lGLU -lX11 # 2014
	CPATHS=		-I/usr/include -L/usr/lib # 2014
	CFLAGS=		$(CPATHS)$(CLIBS)
endif

testing:		testing.cpp
			g++ testing.cpp classes.cpp -o testing -w $(CFLAGS)

square:	square.cpp
			g++ square.cpp -o square -w $(CFLAGS)

squareOfWalls:	squareOfWalls.cpp
			g++ squareOfWalls.cpp -o squareOfWalls -w $(CFLAGS)

threeQuarterSphere:	threeQuarterSphere.cpp
			g++ threeQuarterSphere.cpp -o threeQuarterSphere -w $(CFLAGS)

sphereInBox1:	sphereInBox1.cpp
			g++ sphereInBox1.cpp -o sphereInBox1 -w $(CFLAGS)

glutObjects:	glutObjects.cpp
			g++ glutObjects.cpp -o glutObjects -w $(CFLAGS)

clean:
			rm *.o