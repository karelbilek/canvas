program=canvas
OBJECTS=main.o RGBa.o canvas.o png_paint.o
HEADS=types.h matrix.h interval.h plane.h RGBa.h gr_object.h canvas.h png_paint.h


CPPFLAGS=-O0 -g -Wall -Weffc++ -Wextra -I./headers 
LDFLAGS=-lpng


REAL_OBJECTS=$(addprefix objects/, ${OBJECTS})
REAL_HEADS=$(addprefix headers/, ${HEADS})

build: ${program}

${REAL_OBJECTS}: objects/%.o: sources/%.cpp $(REAL_HEADS)
	g++ -c $(CPPFLAGS) -o $@ $<

${program}: $(REAL_OBJECTS)
	g++ -o $@ ${REAL_OBJECTS} $(LDFLAGS)

clean:
	rm -f objects/*.o ${program}
