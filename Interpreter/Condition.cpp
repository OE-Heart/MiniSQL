/*
 * @Author: Yinwhe
 * @Date: 2021-06-19 13:52:10
 * @LastEditors: Yinwhe
 * @LastEditTime: 2021-06-19 13:53:55
 * @Description: file information
 * @Copyright: Copyright (c) 2021
 */
#include "Condition.hpp"

template <typename T>
static bool isCondTrue(const T &lhs, const T &rhs, OP op) {
    switch(op){
        case OP::EQ:  return lhs == rhs;
        case OP::NEQ: return lhs != rhs;
        case OP::L:   return lhs <  rhs;
        case OP::LEQ: return lhs <= rhs;
        case OP::G:   return lhs >  rhs;
        case OP::GEQ: return lhs >= rhs;
        default: return false;
    }
}

bool Condition::isTrue(const Value &v) const {
    if (auto p = std::get_if<int>(&value)) {
        if (auto q = std::get_if<int>(&v)) {
            return isCondTrue(*q, *p, op);
        } else {
            return isCondTrue(std::get<double>(v), double(*p), op);
        }
    } else if (auto p = std::get_if<double>(&value)) {
        if (auto q = std::get_if<int>(&v)) {
            return isCondTrue(double(*q), *p, op);
        } else {
            return isCondTrue(std::get<double>(v), *p, op);
        }
    } else {
        auto lhs = std::get<std::string>(v);
        auto rhs = std::get<std::string>(value);
        return isCondTrue(lhs, rhs, op);
    }
}