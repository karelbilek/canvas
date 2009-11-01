program=canvas
OBJECTS=main.o RGBa.o canvas.o png_paint.o point.o moved_arrays.o curves/circle.o
HEADS=types.h matrix.h interval.h plane.h RGBa.h curve.h gr_object.h canvas.h png_paint.h stupid_object.h style.h point.h moved_arrays.h curves/circle.h


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
