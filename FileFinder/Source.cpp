#include <iostream>
#include <filesystem>
#include "Finder.h"

namespace fs = std::filesystem;

int main() {
	try {
		Finder::find("icecast.luac");
	}
	catch (...) {
	}
	
}