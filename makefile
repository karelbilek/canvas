program=libcan.a
OBJECTS=canvas.o curves/circle.o curves/line.o curves/bezier.o moved_arrays.o point.o RGBa.o shape.o shape_type.o geom_line.o shape_types/disk.o shape_types/segment.o shape_types/bezier_segment.o shape_types/regular.o shape_types/rectangle.o shape_types/polygon.o shape_types/elipse.o
HEADS=all_shapes.h canvas.h curve.h curves/circle.h curves/line.h curves/bezier.h interval.h moved_arrays.h plane.h point.h RGBa.h shape.h types.h shape_type.h geom_line.h shape_types/disk.h shape_types/segment.h shape_types/bezier_segment.h shape_types/regular.h shape_types/rectangle.h shape_types/polygon.h all_curves.h shape_types/elipse.h


CPPFLAGSHARD=-O3 -I./headers -I/opt/local/include/
CPPFLAGS=-O0 -g -Wall -Weffc++ -Wextra -I./headers -I/opt/local/include/


REAL_OBJECTS=$(addprefix objects/, ${OBJECTS})
REAL_HEADS=$(addprefix headers/, ${HEADS})

build: dirs ${program}

dirs:
	if [ ! -d objects ]; then mkdir objects; fi
	if [ ! -d objects/shape_types ]; then mkdir objects/shape_types; fi
	if [ ! -d objects/curves ]; then mkdir objects/curves; fi


${REAL_OBJECTS}: objects/%.o: sources/%.cpp $(REAL_HEADS)
	g++ -c $(CPPFLAGS) -o $@ $<

${program}: $(REAL_OBJECTS)
	ar rcs $@ ${REAL_OBJECTS}

clean:
	rm -f objects/*.o objects/curves/*.o ${program}
