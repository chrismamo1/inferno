OBJ = frontend_opengl_linux.o iprimitives.o iutils.o icoordinates.o error.o igraphics.o isimulation.o frontend.o

OBJ_INC = obj/frontend.o obj/frontend_opengl_linux.o obj/iprimitives.o obj/iutils.o obj/icoordinates.o obj/error.o obj/igraphics.o obj/isimulation.o

HEADERS = src/frontend.h src/frontend_opengl_linux.h src/icoordinates.h src/iprimitives.h src/iutils.h src/error.h src/igraphics.h src/iprettyconsole.h src/isimulation.h src/itypes.h

DEPS = src/frontend.c src/frontend_opengl_linux.c src/inferno.c src/iprimitives.c src/iutils.c src/icoordinates.c src/error.c src/igraphics.c src/isimulation.c src/frontend.c

CFLAGS = -O1 -g -Wall

GCC_LINKS = -lGL -lX11 -lGLU

inferno: $(DEPS) $(HEADERS) $(OBJ)
	gcc $(CFLAGS) src/inferno.c $(OBJ_INC) $(GCC_LINKS) -o inferno

unit_tests: $(HEADERS) $(DEPS) $(OBJ) src/tests/iunit_tests.c
	gcc $(CFLAGS) src/tests/iunit_tests.c $(OBJ_INC) $(GCC_LINKS) -o tests
	./tests

#frontend_opengl_linux.o: frontend_events.o
#	gcc -g -Wall -c src/frontend_opengl_linux.c obj/frontend_events.o -o obj/$@

%.o: src/%.c
	gcc $(CFLAGS) -c $^ -o obj/$@ $(GCC_LINKS)

clean:
	rm obj/*
	rm inferno
