#include <iostream>
#include "Finder.h"

int main() {
	std::filesystem::path find("Finder.cpp");
	std::cout << Finder::find(find) << std::endl;
}