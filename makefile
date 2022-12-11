CXX      = g++
CXXFLAGS = -std=c++20 -pthread
CPPFLAGS =
INCLUDES = -Iinclude/ -Iexternal/

BASE_COMPILE = $(CXX) $(CPPFLAGS) $(CXXFLAGS) $(INCLUDES)
COMPILE      = $(BASE_COMPILE) -O3 -flto

TIME        = /usr/bin/time
TIME_FORMAT = "     %e seconds elapsed"

# Since de_qgr.h and qgr.h cannot be compiled together, it is convenient to auto generate the dependency list
DEPS     = $(subst \,,$(shell $(BASE_COMPILE) -MM include/bruteforce-enigma.h))
INCL     = $(notdir $(wordlist 3, $(words $(DEPS)), $(DEPS)))
HDR_ONLY = modular_int.h leaderboard.h score.h
HDRS     = $(filter-out $(HDR_ONLY),$(INCL))
SRCS     = $(addprefix source/,$(HDRS:.h=.cpp))
OBJS     = $(addprefix build/,$(HDRS:.h=.o))


# ======================================================================================================================
# Main
# ======================================================================================================================
MAIN = crack_enigma


$(MAIN): $(MAIN).cpp $(OBJS)
	@printf "Building $@ ..."
	@$(TIME) --format=$(TIME_FORMAT) -- $(COMPILE) $(MAIN).cpp $(OBJS) -o $@


.PHONY: debug
debug:
	@$(BASE_COMPILE) -ggdb -Og $(MAIN).cpp $(SRCS) -o $(MAIN)


.PHONY: $(MAIN)_callgrind
$(MAIN)_callgrind:
	@echo "Building $@ ..."
	@$(COMPILE) -ggdb $(MAIN).cpp $(SRCS) -o $(MAIN)
	valgrind --tool=callgrind --simulate-cache=yes ./$(MAIN)


build/%.o: source/%.cpp source/%.h
	@mkdir -p build
	@printf "Building $@ ..."
	@$(TIME) --format=$(TIME_FORMAT) -- $(COMPILE) $< -c -o $@


# ======================================================================================================================
# Tests
# ======================================================================================================================
TEST_SRCS = $(filter-out tests/main.test.cpp,$(shell find tests/ -name "*.test.cpp"))
TEST_EXES = $(addprefix build/,$(TEST_SRCS:.cpp=.out))


tests: build/tests/main.test.o $(TEST_EXES)


# Recompiles a single file on change, so the test harness can autorun. Pass relative src= on the command line.
.PHONY: watch-test
watch-test: exe=build/$(basename $(src)).out
watch-test:
	@if [ -f "$(src)" ]; then                     \
		while true; do                            \
			clear;                                \
			$(MAKE) $(exe) --no-print-directory;  \
			                                      \
			echo "watching $(notdir $(src)) ..."; \
			inotifywait -qq -e modify $(src);     \
		done;                                     \
	else                                          \
		echo "file doesn't exist: $(src)";        \
	fi


build/tests/%.test.out: tests/%.test.cpp $(OBJS)
	@printf "Building $(@F) ..."
	@mkdir -p $(@D)
	@$(TIME) --format=$(TIME_FORMAT) -- $(COMPILE) -ggdb build/tests/main.test.o $(OBJS) $< -o $@


build/tests/main.test.o: tests/main.test.cpp
	@printf "Building $(@F) ..."
	@mkdir -p $(@D)
	@$(TIME) --format=$(TIME_FORMAT) -- $(COMPILE) $< -c -o $@


# ======================================================================================================================
# Misc
# ======================================================================================================================
.PHONY: clean
clean:
	rm -f $(MAIN)
	rm -rf build/


.PHONY: clean-tests
	rm -rf build/tests
