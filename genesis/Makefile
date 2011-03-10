all:
	@clear
	@#g++ -Wall -ansi -pedantic -O3 -funroll-loops -march=core2 impulse.cpp
	@#g++ -Wall -ansi -pedantic -O3 -march=core2 impulse.cpp
	@#$(CXX) -Wall -ansi -pedantic -fno-strict-aliasing -O3 -funroll-loops $(LIBS) -Iinclude -o impulse source/impulse.cpp
	$(CXX) -Wall -ansi -pedantic -O3 -funroll-loops $(LIBS) -Iinclude -o impulse source/impulse.cpp

test:
	@clear
	$(CXX) -Wall -ansi -pedantic -O3 -funroll-loops $(LIBS) -DTEST -Iinclude -o impulse source/impulse.cpp
	@./impulse

bench:
	@clear
	$(CXX) -Wall -ansi -pedantic -O3 -funroll-loops $(LIBS) -DBENCH -Iinclude -o impulse source/impulse.cpp
	@time -p ./impulse

debug:
	@clear
	$(CXX) -Wall -ansi -pedantic -g -DTEST -DDEBUG -Iinclude -o impulse source/impulse.cpp

test-debug:
	@clear
	$(CXX) -Wall -ansi -pedantic -g $(LIBS) -DTEST -DDEBUG -Iinclude -o impulse source/impulse.cpp
	@./impulse

