program=canlib.a
OBJECTS=all_shapes.o canvas.o curves/circle.o curves/line.o curves/bezier.o moved_arrays.o point.o RGBa.o shape.o shape_type.o geom_line.o
HEADS=all_shapes.h canvas.h curve.h curves/circle.h curves/line.h curves/bezier.h interval.h matrix.h moved_arrays.h plane.h point.h RGBa.h shape.h types.h shape_type.h geom_line.h


CPPFLAGS=-O3 -I./headers -I/opt/local/include/


REAL_OBJECTS=$(addprefix objects/, ${OBJECTS})
REAL_HEADS=$(addprefix headers/, ${HEADS})

build: ${program}

${REAL_OBJECTS}: objects/%.o: sources/%.cpp $(REAL_HEADS)
	g++ -c $(CPPFLAGS) -o $@ $<

${program}: $(REAL_OBJECTS)
	ar rcs $@ ${REAL_OBJECTS}

clean:
	rm -f objects/*.o objects/curves/*.o ${program}
