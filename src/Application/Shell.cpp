#include <Application/Shell.hpp>
#include <Application/Application.hpp>
#define cmnd_lambda [](Application & app, const std::string * args)

void Shell::runFunction(const std::string * args, const unsigned short length) {
	if (commands.find(args[0]) != commands.end()) {
		if (length - 1 == commands.at(args[0]).n_args) {
			Application & app = *static_cast<Application *>(controller);
			commands.at(args[0]).function(app, args);
		} else {
			Serial.print("Bad arguments");
		}
	} else {
		Serial.print("Bad command.\n");
	}
}
/*
	addFunction(
		cmnd_lambda { function },
		number of args,
		"name");
	in function, args[0] should be the same as "name".

*/
void Shell::setup() {
	// run button
	addFunction(
		cmnd_lambda { app.sm.begin(); },
		0,
		"sample_button_press");

	addFunction(
		cmnd_lambda { app.csm.begin(); },
		0,
		"clean_button_press");
};

void Shell::addFunction(ShellSpace::func function, const unsigned short n_args, const char * name) {
	ShellSpace::func_args entry;
	entry.function = function;
	entry.n_args   = n_args;
	commands[name] = entry;
}

/*

struct BadArgs : public std::exception {
const char * what() const throw() {
	return "Bad args";
}
};
try {
	ShellSpace::func_args access = commands.at(args[0]);
	if (length != access.args) {
		throw BadArgs();
	} else {
		Application & app = *static_cast<Application *>(controller);
		access.function(app, args);
	}
} catch (std::exception & e) {
	Serial.print("Exception: ");
	Serial.print(e.what());
	Serial.print("\n");
}*/