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

void Shell::setup() {
	ShellSpace::func_args entry;

	entry.function = cmnd_lambda {
		Serial.print("aljdl");
	};
	entry.n_args	 = 0;
	commands["test"] = entry;
};

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