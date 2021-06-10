/*
 * @Author: Ou Yixin
 * @Date: 2021-06-09 23:25:26
 * @LastEditors: Ou Yixin
 * @LastEditTime: 2021-06-10 10:06:25
 * @Description: 
 * @FilePath: /MiniSQL/API/API.hpp
 */
#pragma once

#include "../Interpreter/Interpreter.hpp"

class MiniSQL{
public:
    MiniSQL();
    ~MiniSQL();
    Interpreter interpreter;
private:
};