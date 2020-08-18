#pragma once

#include <map>
#include <utility>
#include <string>

class Application;
namespace ShellRecieverSpace {
	typedef void (*func)(Application & app, std::string * args);
};	// namespace ShellRecieverSpace

class ShellReciever {
public:
	std::map<std::string, ShellRecieverSpace::func> commands;

	ShellReciever();
	// void runFunction(Application &, std::string[]);
};