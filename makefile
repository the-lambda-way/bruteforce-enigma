CXX      = g++-10
CXXFLAGS = -std=c++20 -pthread
CPPFLAGS = -flto
COMPILE  = $(CXX) $(CPPFLAGS) $(CXXFLAGS)

OBJS     = bestlist.o bruteforce-enigma.o enigma.o models.o qgr.o score.o
MAIN     = crack_enigma


crack_enigma: $(MAIN).cpp $(OBJS)
	@$(COMPILE) -O3 $(MAIN).cpp $(OBJS) -o $@


.PHONY: debug
debug: $(OBJS)
	@$(COMPILE) -ggdb -Og $(MAIN).cpp $(OBJS) -o crack_enigma


crack_enigma_callgrind: $(MAIN).cpp $(OBJS)
	@$(COMPILE) -ggdb -Og $(MAIN).cpp $(OBJS) -o crack_enigma


crack_enigma_cachegrind: $(MAIN).cpp $(OBJS)
	@$(COMPILE) -ggdb -O3 $(MAIN).cpp $(OBJS) -o crack_enigma


.PHONY: test
test: $(OBJS)
	@$(COMPILE) -O3 test.cpp $(OBJS) -o $@
	@./test


.PHONY: debug-test
debug-test: $(OBJS)
	@$(COMPILE) -ggdb -Og test.cpp $(OBJS) -o test


.PHONY: clean
clean:
	rm -f *.o $(MAIN) test


%.o: %.cpp
	$(COMPILE) -O3 $< -c