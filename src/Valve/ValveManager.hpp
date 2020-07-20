#pragma once
#include <KPFoundation.hpp>
#include <KPSubject.hpp>
#include <Valve/Valve.hpp>

#include <vector>

class ValveManager {
public:
	std::vector<Valve> valves;
	unsigned int numberOfValvesInUse = 0;
};