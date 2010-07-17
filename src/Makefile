all:
	@#g++ -Wall -ansi -pedantic -O3 -funroll-loops -march=core2 impulse.cpp
	@#g++ -Wall -ansi -pedantic -O3 -march=core2 impulse.cpp
	$(CXX) -Wall -ansi -pedantic -fno-strict-aliasing -O3 -o impulse impulse.cpp

debug:
	g++ -Wall -ansi -pedantic -g -pg -DDEBUG -o impulse impulse.cpp

profile:
	g++ -Wall -ansi -pedantic -O3 -g -pg -o impulse impulse.cpp

clean:
	rm -rf impulse gmon.out trace.log
	find . -name "*~" | xargs rm -f

