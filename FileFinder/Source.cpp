#include <iostream>
#include "Finder.h"

int main() {
	std::filesystem::path find("Findr.cpp");
	std::cout << Finder::find(find) << std::endl;
	return 0;
}