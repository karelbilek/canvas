program=canvas
OBJECTS=main.o RGBa.o
HEADS=types.h matrix.h interval.h plane.h RGBa.h


CPPFLAGS=-O0 -g -Wall -Weffc++ -Wextra -I./headers
LDFLAGS=


REAL_OBJECTS=$(addprefix objects/, ${OBJECTS})
REAL_HEADS=$(addprefix headers/, ${HEADS})

build: ${program}

${REAL_OBJECTS}: objects/%.o: sources/%.cpp $(REAL_HEADS)
	g++ -c $(CPPFLAGS) -o $@ $<

${program}: $(REAL_OBJECTS)
	g++ -o $@ ${REAL_OBJECTS} $(LDFLAGS)

clean:
	rm -f objects/*.o ${program}
