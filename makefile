CXX      = g++-10
CXXFLAGS = -std=c++20 -pthread
COMPILE  = $(CXX) $(CXXFLAGS) $(CPPFLAGS)


.PHONY: crack_enigma
crack_enigma:
	@$(COMPILE) -O3 crack_enigma.cpp -o $@


.PHONY: crack_enigma_callgrind
crack_enigma_callgrind:
	@$(COMPILE) -ggdb crack_enigma.cpp -o crack_enigma


.PHONEY: crack_enigma_cachegrind
crack_enigma_cachegrind:
	@$(COMPILE) -ggdb -O3 crack_enigma.cpp -o crack_enigma


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
