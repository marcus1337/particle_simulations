#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <map>


namespace FileNames {
	enum FNAME { SCRIPT1, SCRIPT2, SCRIPT3 };

	static std::string fname_str(FileNames::FNAME var) {
		if (var == FileNames::SCRIPT1)
			return "luascripts.lua";
		return "";
	}
}

class EnumNames {

public:

};
