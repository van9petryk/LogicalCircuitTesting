#ifndef LOGICELEMENT_H
#define LOGICELEMENT_H

#include "component.h"
#include "logicfunc.h"
#include <QList>

class LogicElement : public Component
{
public:
    LogicElement(const QByteArray &name, LogicFunc lf);
    ~LogicElement() { }
    LogicElement* addInput(Component* component) { _inputs += component; return this; }
    QList <Component*> getInputs() { return _inputs; }
    void eval();
private:
    bool _inv;
    bool (*lop)(bool, bool);
    QList <Component*> _inputs;
};

#endif // LOGICELEMENT_H
