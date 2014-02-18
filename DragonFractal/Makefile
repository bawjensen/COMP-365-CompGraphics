CLIBS= 		-lm -lglut -lGL -lGLU -lX11 # 2014
CPATHS=		-I/usr/include -L/usr/lib # 2014
CFLAGS=		$(CPATHS)$(CLIBS)

fractal:	fractal.cpp
			g++ fractal.cpp classes.cpp -o JensenG2 $(CFLAGS)

clean:
			rm *.o