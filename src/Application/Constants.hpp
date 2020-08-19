#pragma once

namespace HardwarePins {
	constexpr int SHFT_REG_LATCH = 9;
	constexpr int SHFT_REG_CLOCK = 11;
	constexpr int SHFT_REG_DATA	 = 12;
	constexpr int MOTOR_REVERSE	 = 5;
	constexpr int MOTOR_FORWARDS = 6;
	constexpr int RUN_BUTTON	 = 13;
	constexpr int CLEAN_BUTTON	 = A3;
}  // namespace HardwarePins

// flush pin is the same

namespace DefaultTimes {
	const int FLUSH_TIME  = 50;
	const int SAMPLE_TIME = 60;
	const int PURGE_TIME  = 2;
	const int IDLE_TIME	  = 3600 - FLUSH_TIME - SAMPLE_TIME - PURGE_TIME;
	const int SETUP_TIME  = 0;
}  // namespace DefaultTimes

namespace DefaultTimesClean {
	const int SAMPLE_TIME = 5;
	const int IDLE_TIME	  = 0;
	const int FLUSH_TIME  = 20;
}  // namespace DefaultTimesClean

namespace DefaultTimesButton {
	const unsigned long DEBOUNCE_TIME = 100;
}

namespace TPICDevices {
	constexpr int INTAKE_POS  = 0;
	constexpr int INTAKE_NEG  = 1;
	constexpr int FLUSH_VALVE = 6;
	constexpr int WATER_VALVE = 7;
}  // namespace TPICDevices