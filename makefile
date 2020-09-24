CXX      = g++-10
CXXFLAGS = -std=c++20 -pthread
COMPILE  = $(CXX) $(CXXFLAGS) $(CPPFLAGS)


.PHONY: crack_enigma
crack_enigma:
	@$(COMPILE) -O3 crack_enigma.cpp -o $@


.PHONY: test
test:
	@$(COMPILE) -O3 test.cpp -o $@
	@./test


.PHONY: debug
debug:
	@$(COMPILE) -ggdb crack_enigma.cpp -o crack_enigma


.PHONY: debug-test
debug-test:
	@$(COMPILE) -ggdb test.cpp -o test


.PHONY: clean
clean:
	rm -f *.o crack_enigma test
