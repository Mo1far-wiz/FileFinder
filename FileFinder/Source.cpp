#include "Finder.h"

int main() {
	std::filesystem::path find("Finder.cpp");
	Finder::find(find);
	return 0;
}