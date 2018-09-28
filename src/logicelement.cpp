#include "logicelement.h"

LogicElement::LogicElement(const QByteArray &name, LogicFunc lf) : Component(name, LOGIC_ELEMENT)
{
    if (lf == NAND || lf == NOR || lf == XNOR || lf == NOT)
        _inv = true;
    else _inv = false;

    if (lf == AND || lf == NAND)
        lop = &_and;
    else if (lf == OR || lf == NOR)
        lop = &_or;
    else if (lf == XOR || lf == XNOR)
        lop = &_xor;
    else
        lop = nullptr;
}

void LogicElement::eval()
{
    bool val = _inputs.first()->getValue();
    if (lop != nullptr)
        for (int i = 1; i < _inputs.size(); i++)
            val = lop(val, _inputs.at(i)->getValue());

    _val = val != _inv;
}
