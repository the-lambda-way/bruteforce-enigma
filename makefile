TIME_FORMAT="     %E elapsed"

CXX      = g++-10
CXXFLAGS = -std=c++20 -pthread
CPPFLAGS = -flto
COMPILE  = $(CXX) $(CPPFLAGS) $(CXXFLAGS) -O3

# Since de_qgr.h and qgr.h cannot be compiled together, it is convenient to auto generate the dependency list
DEPS     = $(subst \,,$(shell $(CXX) $(CXXFLAGS) -MM include/bruteforce-enigma.h))
INCLUDES = $(notdir $(wordlist 3, $(words $(DEPS)), $(DEPS)))
HDR_ONLY = modular_int.h rotors.h
HDRS     = $(filter-out $(HDR_ONLY),$(INCLUDES))
SRCS     = $(addprefix src/,$(HDRS:.h=.cpp))
OBJS     = $(addprefix build/,$(HDRS:.h=.o))

MAIN = crack_enigma


$(MAIN): $(MAIN).cpp $(OBJS)
	@printf "Building $@ ..."
	@time -f $(TIME_FORMAT) -- $(COMPILE) -Iinclude/ $(MAIN).cpp $(OBJS) -o $@


.PHONY: debug
debug:
	@$(CXX) $(CXXFLAGS) -ggdb -Iinclude/ $(MAIN).cpp $(SRCS) -o $(MAIN)


.PHONY: $(MAIN)_callgrind
$(MAIN)_callgrind:
	@$(CXX) $(CXXFLAGS) -ggdb -Og -Iinclude/ $(MAIN).cpp $(SRCS) -o $(MAIN)


.PHONY: $(MAIN)_cachegrind
$(MAIN)_cachegrind:
	@$(CXX) $(CXXFLAGS) -ggdb -O3 -Iinclude/ $(MAIN).cpp $(SRCS) -o $(MAIN)


test: tests/test.cpp $(OBJS)
	@printf "Building $@ ..."
	@time -f $(TIME_FORMAT) -- $(COMPILE) -Iinclude/ tests/test.cpp $(OBJS) -o $@
	@./test


.PHONY: debug-test
debug-test:
	@$(CXX) $(CXXFLAGS) -ggdb -Iinclude/ tests/test.cpp $(SRCS) -o test


.PHONY: clean
clean:
	rm -f $(MAIN) test
	rm -rf build/


build/%.o: src/%.cpp src/%.h
	@mkdir -p build
	@printf "Building $@ ..."
	@time -f $(TIME_FORMAT) -- $(COMPILE) $< -c -o $@
