#pragma once

#include "../Interpreter/Interpreter.hpp"

class MiniSQL{
public:
    MiniSQL();
    ~MiniSQL();
    Interpreter interpreter;
private:
};