#pragma once
#include <KPController.hpp>
#include <KPFileLoader.hpp>

//subclassing?

class Application : public KPController {
public:
    void setup() override {}
    void update() override { KPController::update(); }
};