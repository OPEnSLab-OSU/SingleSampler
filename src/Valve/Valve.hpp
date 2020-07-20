#pragma once
#include <KPFoundation.hpp>

#include <Application/Constants.hpp>
#include <Valve/ValveStatus.hpp>

struct Valve {
	int id	   = ValveStatus::unavailable;
	int status = ValveStatus::unavailable;

	Valve()					   = default;
	Valve(const Valve & other) = default;
	Valve & operator=(const Valve & other) = default;
};	// State of the valve