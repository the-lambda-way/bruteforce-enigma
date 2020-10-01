CXX      = g++-10
CXXFLAGS = -std=c++20 -pthread
CPPFLAGS = -flto
COMPILE  = $(CXX) $(CPPFLAGS) $(CXXFLAGS)

# Since de_qgr.h and qgr.h cannot be compiled together, it is convenient to auto generate the dependency list
GCC_DEPS = $(subst \,,$(shell $(CXX) $(CXXFLAGS) -MM include/bruteforce-enigma.h))
DEPS     = $(wordlist 3, $(words $(GCC_DEPS)), $(GCC_DEPS))
OBJS 	 = $(patsubst include/../src/%,build/%,$(DEPS:.h=.o))

MAIN = crack_enigma


$(MAIN): $(MAIN).cpp $(OBJS)
	@$(COMPILE) -O3 -Iinclude/ $(MAIN).cpp $(OBJS) -o $@


.PHONY: debug
debug: $(OBJS)
	@$(COMPILE) -ggdb -Og $(MAIN).cpp $(OBJS) -o $(MAIN)


$(MAIN)_callgrind: $(MAIN).cpp $(OBJS)
	@$(COMPILE) -ggdb -Og $(MAIN).cpp $(OBJS) -o $(MAIN)


$(MAIN)_cachegrind: $(MAIN).cpp $(OBJS)
	@$(COMPILE) -ggdb -O3 $(MAIN).cpp $(OBJS) -o $(MAIN)


.PHONY: test
test: $(OBJS)
	@$(COMPILE) -O3 tests/test.cpp $(OBJS) -o $@
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
	$(COMPILE) -O3 $< -c -o $@

