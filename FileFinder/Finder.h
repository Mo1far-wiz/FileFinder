#pragma once
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

class Finder
{
private :
	size_t _thread_counter;
	bool _recursion_exit;
	const fs::path _root;
	
public:
	Finder() : _thread_counter(0), _recursion_exit(false), _root(fs::current_path().root_path()) {}

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

	void find(fs::path const& targ) {
		recursive_search(_root, targ.string());
	}
};

