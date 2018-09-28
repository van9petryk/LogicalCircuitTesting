#ifndef INPUT_H
#define INPUT_H

#include "component.h"

class Input : public Component
{
public:
    Input(const QByteArray &name) : Component(name, INPUT) { }
    ~Input() { }
    void setValue(bool value) { _val = value; }
};

#endif // INPUT_H
