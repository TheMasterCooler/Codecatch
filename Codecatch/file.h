#pragma once
#include <iostream>
#include <vector>

namespace CC {
	namespace File {
		int Init();
		void SaveCode(std::string fPath, std::string code);
		std::vector<std::string> SplitDirectory(std::string dir, std::string separator);
	}
}