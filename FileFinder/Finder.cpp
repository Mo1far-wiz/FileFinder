#include <filesystem>
#include <iostream>
#include "Finder.h"

namespace fs = std::filesystem;

bool _recursion_exit = false;

void recursive_search(fs::path const& root, std::string const& target, size_t loop = 0, bool exit = false) {
	for (auto const& dir_entry : fs::directory_iterator{ root }) {
		if (_recursion_exit == true)
			return;

		if (!dir_entry.is_directory())
			continue;

		if (fs::exists(dir_entry / fs::path(target)) && _recursion_exit != true) {
			std::cout << std::string(loop, ' ') << dir_entry / fs::path(target) << std::endl;
			_recursion_exit = true;
			return;
		}

		try {
			//std::cout << std::string(loop, ' ') << dir_entry << std::endl;
			recursive_search(dir_entry, target, ++loop, _recursion_exit);
		}
		catch (fs::filesystem_error const& er) {
			if (er.code().value() == 5)
				std::cout << "Permission denied : " << dir_entry << std::endl;
			else
				std::cout << er.what() << std::endl;
		}
		catch (std::exception& ex) {
			std::cout << ex.what() << std::endl;
		}
		--loop;
	}
}

void Finder::find(fs::path const& targ)
{
	recursive_search(fs::current_path().root_path(), targ.string());
}