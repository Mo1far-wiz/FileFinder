#include <iostream>
#include <thread>
#include <mutex>
#include <filesystem>
#include <list>
#include "Finder.h"

namespace fs = std::filesystem;

std::mutex mutex;
size_t _max_threds = 8;
bool _recursion_exit = false;
fs::path _target_loc("<No such file on>");

void recursive_search(const fs::path& root, const std::string& target, size_t loop = 0) {
	try {
		for (auto const& dir_entry : fs::directory_iterator{ root }) {
			if (!dir_entry.is_directory())
				continue;

			if (fs::exists(dir_entry / fs::path(target)) && _recursion_exit != true) {
				std::scoped_lock<std::mutex> lock(mutex);
				_recursion_exit = true;

				//std::cout << std::string(loop, ' ') << dir_entry / fs::path(target) << std::endl;
				_target_loc = dir_entry / fs::path(target);
				return;
			}

			//std::cout << std::string(loop, ' ') << dir_entry << std::endl;
			recursive_search(dir_entry, target, ++loop);
			--loop;

			if (_recursion_exit == true)
				return;
		}

		return;
	}
	catch (const fs::filesystem_error& er) {
		if (er.code().value() == 5) {
			//std::cout << "Dir access is denied" << std::endl;
		}
		else
			std::cout << er.what() << " | " << er.code() << std::endl;
	}
	catch (const std::exception& ex) {
		std::cout << ex.what() << std::endl;
	}
	catch (...) {	
		throw;
	}
}

fs::path Finder::find(const std::filesystem::path& targ)
{
	const fs::path root(fs::current_path().root_path());
	std::list<std::thread> threads;

	for (auto const& dir_entry : fs::directory_iterator{ root }) {
		if (threads.size() >= _max_threds) {
			threads.front().join();
			threads.pop_front();
		}

		if (dir_entry.is_directory())
			threads.push_back(std::thread(recursive_search, dir_entry, targ.string(), 0));

		if (_recursion_exit) {
			for (auto& thr : threads)
				thr.join();
			return _target_loc;
		}
	}

	for (auto& thr : threads)
		thr.join();

	//std::cout << "No such file on " << root;
	return fs::path(root / "<No such file>");
}