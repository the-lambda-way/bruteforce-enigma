#define CATCH_CONFIG_MAIN

// Fix parsing bug on long TESTCASE names
//      https://github.com/matepek/vscode-catch2-test-adapter/blob/master/documents/support.md
#define CATCH_CONFIG_CONSOLE_WIDTH 300
#include "catch2/catch.hpp"
