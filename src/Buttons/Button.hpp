#include <KPFoundation.hpp>
#include <Components/StateMachine.hpp>
class Button {
public:
	unsigned short pin;
	const char * name;
	bool last_read;
	unsigned long last_time;
	unsigned long debounce_time;
	Button(unsigned short n_pin, const char * n_name,
		unsigned long n_debounce = DefaultTimesButton::DEBOUNCE_TIME)
		: pin(n_pin), name(n_name), debounce_time(n_debounce) {
		pinMode(pin, INPUT);
		last_read = digitalRead(pin);
		last_time = millis();
	}
	virtual void listen(StateMachine & sm) {
		if (millis() - last_time > debounce_time) {
			if (last_read != digitalRead(pin) && LOW == last_read) {
				act(sm);
			}
			last_time = millis();
		}
		last_read = digitalRead(pin);
	}
	virtual void act(StateMachine & sm) {
		sm.begin();
	};
};