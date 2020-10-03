TIME_FORMAT="     %E elapsed"

CXX      = g++-10
CXXFLAGS = -std=c++20 -pthread
CPPFLAGS = -flto
COMPILE  = $(CXX) $(CPPFLAGS) $(CXXFLAGS)

# Since de_qgr.h and qgr.h cannot be compiled together, it is convenient to auto generate the dependency list
DEPS = $(subst \,,$(shell $(CXX) $(CXXFLAGS) -MM include/bruteforce-enigma.h))
HDRS = $(wordlist 3, $(words $(DEPS)), $(DEPS))
OBJS = $(patsubst include/../src/%,build/%,$(HDRS:.h=.o))

MAIN = crack_enigma


$(MAIN): $(MAIN).cpp $(OBJS)
	@printf "Building $@ ..."
	@time -f $(TIME_FORMAT) -- $(COMPILE) -O3 -Iinclude/ $(MAIN).cpp $(OBJS) -o $@


.PHONY: debug
debug: $(OBJS)
	@$(COMPILE) -ggdb -Og $(MAIN).cpp $(OBJS) -o $(MAIN)


$(MAIN)_callgrind: $(MAIN).cpp $(OBJS)
	@$(COMPILE) -ggdb -Og $(MAIN).cpp $(OBJS) -o $(MAIN)


$(MAIN)_cachegrind: $(MAIN).cpp $(OBJS)
	@$(COMPILE) -ggdb -O3 $(MAIN).cpp $(OBJS) -o $(MAIN)


.PHONY: test
test: $(OBJS)
	@time -f $(TIME_FORMAT) -- $(COMPILE) -O3 tests/test.cpp $(OBJS) -o $@
	@./test


.PHONY: debug-test
debug-test: $(OBJS)
	@$(COMPILE) -ggdb -Og tests/test.cpp $(OBJS) -o test


.PHONY: clean
clean:
	rm -f $(MAIN) test
	rm -rf build/


build/%.o: src/%.cpp src/%.h
	@mkdir -p build
	@printf "Building $@ ..."
	@time -f $(TIME_FORMAT) -- $(COMPILE) -O3 $< -c -o $@

