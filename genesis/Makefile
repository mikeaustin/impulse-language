#OPTS =	-Woverloaded-virtual --param inline-unit-growth=10000 --param large-function-growth=10000 --param max-inline-insns-single=10000
OPTS =	-Wfatal-errors

all:
	@clear
	@#g++ -Wall -ansi -pedantic -O3 -funroll-loops -march=core2 impulse.cpp
	@#g++ -Wall -ansi -pedantic -O3 -march=core2 impulse.cpp
	@#$(CXX) -Wall -ansi -pedantic -fno-strict-aliasing -O3 -funroll-loops $(LIBS) -Iinclude -o impulse source/impulse.cpp
	$(CXX) $(OPTS) -Wall -ansi -pedantic -O3 -funroll-loops $(LIBS) -Iinclude -o impulse source/impulse.cpp

run: all
	@./impulse

test:
	@clear
	$(CXX) -Wall -ansi -pedantic -O3 -funroll-loops $(LIBS) -DTEST -Iinclude -o impulse source/impulse.cpp
	@./impulse

bench:
	@clear
	$(CXX) -Wall -Wfatal-errors -ansi -pedantic -O3 -pg -funroll-loops $(LIBS) -DBENCH -Iinclude -o impulse source/impulse.cpp
	@time -p ./impulse

trace: clear
	$(CXX) -Wall -ansi -pedantic -g $(LIBS) -DTRACE -Iinclude -o impulse source/impulse.cpp
	
debug:
	@clear
	$(CXX) -Wall -ansi -pedantic -g -pg $(LIBS) -DDEBUG -Iinclude -o impulse source/impulse.cpp

test-debug:
	@clear
	$(CXX) -Wall -Wfatal-errors -ansi -pedantic -g $(LIBS) -DTEST -DDEBUG -Iinclude -o impulse source/impulse.cpp
	@./impulse

clear:
	@clear

