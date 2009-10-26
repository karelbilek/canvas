program=canvas
OBJECTS=main.o
HEADS=types.h matrix.h interval.h plane.h


CPPFLAGS=-O0 -g -Wall -Weffc++ -Wextra -I./headers
LDFLAGS=

build: ${program}

${OBJECTS}: $(addprefix headers/, ${HEADS})

${program}: ${OBJECTS}
	g++ -o $@ $(OBJECTS) $(LDFLAGS)

clean:
	rm -f *.o ${program}
