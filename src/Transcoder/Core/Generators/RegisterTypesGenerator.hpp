#pragma once
#include "../Project.hpp"
#include "Generator.hpp"

class RegisterTypesGenerator : public Generator {
public:
    void generate(Module* module);
};
