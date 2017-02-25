/* 2107-02-21 */
#include <iostream>

#include "terminal.h"


int main() {
    std::cout << terminal::bold << terminal::bg::green << terminal::fg::red
              << "Hello World" << terminal::reset << std::endl
              << "Hello World" << std::endl
              << terminal::faint << terminal::bg::yellow << terminal::fg::magenta
              << "Hello World" << terminal::reset << std::endl;

	return 0;
}

