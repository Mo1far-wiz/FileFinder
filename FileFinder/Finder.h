#pragma once
#include <filesystem>

namespace Finder {
	size_t _max_threds = 8;
	void find(std::filesystem::path const& targ);
}