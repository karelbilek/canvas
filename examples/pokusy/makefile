program=example
OBJECTS=main.o 
HEADS=

CPPFLAGS=-O0 -g -Wall -Weffc++ -Wextra -I../../headers -I./headers -I/opt/local/include/
LDFLAGS=-L/opt/local/lib -L../../ -lpng -lcan

REAL_OBJECTS=$(addprefix objects/, ${OBJECTS})
REAL_HEADS=$(addprefix headers/, ${HEADS})

build: ${program}

${REAL_OBJECTS}: objects/%.o: sources/%.cpp $(REAL_HEADS)
	g++ -c $(CPPFLAGS) -o $@ $<

${program}: $(REAL_OBJECTS)
	g++ -o $@ ${REAL_OBJECTS} $(LDFLAGS)

clean:
	rm -f objects/*.o objects/curves/*.o ${program}
