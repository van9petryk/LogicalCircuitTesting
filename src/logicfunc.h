#ifndef LOGICFUNC_H
#define LOGICFUNC_H

#include <QByteArray>
enum LogicFunc {AND, NAND, OR, NOR, XOR, XNOR, EQV, NOT};

bool _and(bool a, bool b);
bool _or(bool a, bool b);
bool _xor(bool a, bool b);

LogicFunc logicFuncByStr(const QByteArray &str);

#endif // LOGICFUNC_H
