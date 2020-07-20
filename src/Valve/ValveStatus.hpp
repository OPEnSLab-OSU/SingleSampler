#pragma once

class ValveStatus {
public:
	enum Code {
		unavailable = -1,
		sampled,   // 0
		free,	   // 1
		operating  // 2
	};
	Code code;

	ValveStatus(Code new_code) : code(new_code) {}

	Code code() const {
		return code;
	}

	operator int() const {
		return code;
	}
};