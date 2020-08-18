#pragma once

#include <map>
#include <utility>
#include <string>

#include <Application/Application.hpp>

namespace ShellRecieverSpace {
	using void (*func)(Application & app, std::string * args);
};	// namespace ShellRecieverSpace

class ShellReciever {
public:
	std::map<std::string, ShellRecieverSpace::func> commands;

	ShellReciever();
	// void runFunction(Application &, std::string[]);
};