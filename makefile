CXX      = g++-9
CXXFLAGS = -std=c++17 -pthread -O3
COMPILE  = $(CXX) $(CXXFLAGS) $(CPPFLAGS)


.PHONY: crack_enigma
crack_enigma:
	@$(COMPILE) crack_enigma.cpp scoreText.c -o $@


.PHONY: crack_enigma2
crack_enigma2:
	@$(COMPILE) crack_enigma2.cpp scoreText.c -o $@


.PHONY: test
test:
	@$(COMPILE) test.cpp scoreText.c -o $@
	@./test


.PHONY: test2
test2:
	@$(COMPILE) test2.cpp scoreText.c -o $@
	@./test2


.PHONY: debug
debug:
	@$(COMPILE) -ggdb crack_enigma.cpp scoreText.c -o crack_enigma


.PHONY: debug-test
debug-test:
	@$(COMPILE) -ggdb test2.cpp scoreText.c -o test2


.PHONY: clean
clean:
	rm -f *.o crack_enigma test
