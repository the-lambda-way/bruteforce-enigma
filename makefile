CXX      = g++-9
CXXFLAGS = -std=c++17 -pthread -O3
COMPILE  = $(CXX) $(CXXFLAGS) $(CPPFLAGS)


.PHONY: crack_enigma
crack_enigma:
	@$(COMPILE) crack_enigma.cpp -o $@


.PHONY: test
test:
	@$(COMPILE) test.cpp -o $@
	@./test


.PHONY: debug
debug:
	@$(CXX) -std=c++17 -pthread -ggdb crack_enigma.cpp -o crack_enigma


.PHONY: debug-test
debug-test:
	@$(CXX) -std=c++17 -pthread -ggdb test.cpp -o test


.PHONY: clean
clean:
	rm -f *.o crack_enigma test
