#ifndef PARSESTRUCTURES_H
#define PARSESTRUCTURES_H

#include <QByteArray>
#include <QList>
#include "logicfunc.h"

struct LogicElementDescription {
    LogicElementDescription() {}
    LogicElementDescription(const QByteArray &t) {
        logicFunc = logicFuncByStr(t);
    }
    LogicFunc logicFunc;
    QList <QByteArray> inputs;
};

struct Malfunction {
    Malfunction() {}
    Malfunction(const QByteArray &compName, bool value) { name = compName; val = value; }
    QByteArray name;
    bool val;
    bool operator <(const Malfunction &malfunc) const {
        if (name != malfunc.name)
            return name < malfunc.name;
        else
            return val < malfunc.val;
    }
    bool operator ==(const Malfunction &malfunc) const {
        if (name == malfunc.name)
            return val == malfunc.val;
        return false;
    }
    QByteArray str() const {
        return name + "=" + QByteArray::number(val, 2);
    }
};

struct TestDescription {
    TestDescription() {}
    TestDescription (const QList <QByteArray> &t) : test(t) {}
    QList <QByteArray> test;
    QMap <QByteArray, QList <QByteArray> > diagnostic;
};

struct TaskDescription {
    QList <Malfunction> malfunctions;
    QMap <QByteArray, TestDescription> tests;
};

struct CircuitDescription {
    QList <QByteArray> inputs;
    QMap <QByteArray, QByteArray> outputs;
    QMap <QByteArray, LogicElementDescription> elements;
};


#endif // PARSESTRUCTURES_H
