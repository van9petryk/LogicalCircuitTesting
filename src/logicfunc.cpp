#include "logicfunc.h"

bool _and(bool a, bool b) { return a && b; }
bool _or(bool a, bool b) { return a || b; }
bool _xor(bool a, bool b) { return  a != b; }

LogicFunc logicFuncByStr(const QByteArray &str) {
    if (str == "AND")
        return LogicFunc::AND;
    if (str == "NAND")
        return LogicFunc::NAND;
    if (str == "OR")
        return LogicFunc::OR;
    if (str == "NOR")
        return LogicFunc::NOR;
    if (str == "XOR")
        return LogicFunc::XOR;
    if (str == "XNOR")
        return LogicFunc::XNOR;
    if (str == "EQV")
        return LogicFunc::EQV;
    if (str == "NOT")
        return LogicFunc::NOT;
    throw std::runtime_error("not found LogicFunc for " + str);
}
