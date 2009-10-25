program=canvas
OBJECTS=main.o
HEADS=types.h matrix.h interval.h


CPPFLAGS=-O0 -g -Wall -I./headers
LDFLAGS=

build: ${program}

${OBJECTS}: $(addprefix headers/, ${HEADS})

${program}: ${OBJECTS}
	g++ -o $@ $(OBJECTS) $(LDFLAGS)

clean:
	rm -f *.o ${program}
