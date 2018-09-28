#ifndef COMPONENT_H
#define COMPONENT_H

#include <QByteArray>

class Component
{
public:
    enum Type {INPUT, LOGIC_ELEMENT};
    virtual ~Component() { }
    bool getValue() { return _malfunc != -1 ? _malfunc : _val; }
    void setMalfunction(bool malfunction) { _malfunc = malfunction; }
    void unsetMalfunction() { _malfunc = -1; }
    QByteArray getName() { return _name; }
    Type type() { return _type; }
protected:
    Component(const QByteArray &name, Type type) { _name = name; _type = type; }
    bool _val;
private:
    QByteArray _name;
    int _malfunc = -1;
    Type _type;
};

#endif // COMPONENT_H
