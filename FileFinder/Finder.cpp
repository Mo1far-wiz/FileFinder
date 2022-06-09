#include <iostream>
#include <thread>
#include <mutex>
#include <filesystem>
#include "Finder.h"

namespace fs = std::filesystem;
std::mutex mutex;
bool _recursion_exit = false;
size_t _max_threds = 8;

void recursive_search(fs::path const& root, std::string const& target, size_t loop = 0, bool exit = false) {
	for (auto const& dir_entry : fs::directory_iterator{ root }) {
		if (_recursion_exit == true)
			return;

		if (!dir_entry.is_directory())
			continue;

		if (fs::exists(dir_entry / fs::path(target)) && _recursion_exit != true) {
			std::scoped_lock<std::mutex> lock(mutex);
			std::cout << std::string(loop, ' ') << dir_entry / fs::path(target) << std::endl;
			_recursion_exit = true;
			return;
		}

		try {
			//std::cout << std::string(loop, ' ') << dir_entry << std::endl;
			recursive_search(dir_entry, target, ++loop, _recursion_exit);
		}
		catch (...) {}
		--loop;
	}
}

void Finder::find(fs::path const& targ)
{
	const fs::path root(fs::current_path().root_path());
	std::list<std::thread> threads;

	for (auto const& dir_entry : fs::directory_iterator{ root }) {
		if (threads.size() >= _max_threds) {
			threads.front().join();
			threads.pop_front();
		}

		if (dir_entry.is_directory())
			threads.push_back(std::thread(recursive_search, dir_entry, targ.string(), 0, false));

		if (_recursion_exit) {
			for (auto& thr : threads)
				thr.join();
			return;
		}
	}
}