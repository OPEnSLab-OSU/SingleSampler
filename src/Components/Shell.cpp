#include <Components/Shell.hpp>
#include <Application/Application.hpp>
#include <Application/Constants.hpp>
#include <Application/Utility.hpp>
#include <KPFoundation.hpp>
#include <SD.h>
#include <ArduinoJson.h>
#define cmnd_lambda [](Application & app, const std::string * args)
#define CALL		(app, args)

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
		"name",
		n args,
		cmnd_lambda { function });

	You will have access to app and args[]. args[0] should always
	be the same as the name of the command. They are C++ strings
*/

void Shell::setup() {
	// run button
	addFunction(
		"sample_button_press",
		0,
		cmnd_lambda { app.sm.begin(); });

	// clean button
	addFunction(
		"clean_button_press",
		0,
		cmnd_lambda { app.csm.begin(); });

	// halt the machine in the sample state
	addFunction(
		"sample_halt",
		0,
		cmnd_lambda { app.sm.halt(); });

	// halt the clean state machine
	addFunction(
		"clean_halt",
		0,
		cmnd_lambda { app.csm.halt(); });

	// halt all state machines
	addFunction(
		"halt",
		0,
		cmnd_lambda {
			app.sm.halt();
			app.csm.halt();
		});

	// print free ram
	addFunction(
		"mem",
		0,
		cmnd_lambda { Serial.println(free_ram()); });

	addFunction(
		"state_read",
		0,
		cmnd_lambda {
			File file = SD.open("state.js", FILE_READ);
			Serial.println(Utility::readEntireFile(file).c_str());
		});

	addFunction(
		"check_sample_flush_time",
		0,
		cmnd_lambda {
			Serial.println(app.sm.getState<SampleStateFlush>(SampleStateNames::FLUSH).time);
		});

	addFunction(
		"set_time",
		1,
		cmnd_lambda {
			if (Utility::msg_posint(args[1].c_str(), 1)) {
				app.power.set(std::stoi(args[1]));
			}
		});
	addFunction(
		"get_time",
		0,
		cmnd_lambda {
			std::string time = std::to_string(app.power.getTime());
			Serial.println(time.c_str());
		});
	
	
};

void Shell::addFunction(const char * name, const unsigned short n_args, ShellSpace::func function) {
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