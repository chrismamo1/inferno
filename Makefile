OBJ = frontend_opengl_linux.o iprimitives.o iutils.o icoordinates.o error.o igraphics.o isimulation.o frontend.o

OBJ_INC = obj/frontend.o obj/frontend_opengl_linux.o obj/iprimitives.o obj/iutils.o obj/icoordinates.o obj/error.o obj/igraphics.o obj/isimulation.o

HEADERS = src/frontends/frontend.h src/icoordinates.h src/iprimitives.h src/iutils.h src/error.h src/igraphics.h src/iprettyconsole.h src/isimulation.h src/itypes.h src/frontends/frontend_opengl_linux.h

DEPS = src/frontends/frontend.c src/inferno.c src/iprimitives.c src/iutils.c src/icoordinates.c src/error.c src/igraphics.c src/isimulation.c src/frontends/frontend_opengl_linux.c

CFLAGS = -O0 -g -Wall -std=c99 -pedantic-errors -iquote./frontends -Isrc -Isrc/frontends

GCC_LINKS = -lGL -lX11 -lGLU

CC = gcc-4.9

inferno: $(DEPS) $(HEADERS) $(OBJ)
	$(CC) $(CFLAGS) src/inferno.c $(OBJ_INC) $(GCC_LINKS) -o inferno

unit_tests: $(HEADERS) $(DEPS) $(OBJ) src/tests/iunit_tests.c
	$(CC) $(CFLAGS) -I../frontends src/tests/iunit_tests.c $(OBJ_INC) $(GCC_LINKS) -o tests
	./tests

%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o obj/$@ $(GCC_LINKS)

frontend.o: src/frontends/frontend.c
	$(CC) -I../ $(CFLAGS) -c $^ $(OBJ_INC) $(GCC_LINKS) -o obj/$@

frontend_opengl_linux.o: src/frontends/frontend_opengl_linux.c
	$(CC) -I../ $(CFLAGS) -c $^ $(OBJ_INC) $(GCC_LINKS) -o obj/$@

clean:
	rm obj/*
	rm inferno
