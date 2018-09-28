#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <QByteArray>
#include "input.h"
#include "logicelement.h"
#include "parsestructures.h"

class Circuit
{
public:
    Circuit(const CircuitDescription &cd);
    Circuit(const Circuit &c) = delete;
    Circuit operator =(const Circuit &c) = delete;
    QMap <QByteArray, QByteArray> simulate(const Malfunction &malfunc);
    QMap <QByteArray, QByteArray> simulate(const QList <QByteArray> &inputsValues, const Malfunction &malfunc);
    QMap <QByteArray, QByteArray> simulate();
    QMap <QByteArray, QByteArray> simulate(const QList<QByteArray> &inputsValues);
    int inputsCount() { return inputs.count(); }
    QList <QByteArray> getNameOutputs() { return outputs.keys(); }
    QList <QByteArray> getNameInputs();
    ~Circuit();
private:
    QList <Input*> inputs;
    QList <LogicElement*> elements;
    QMap <QByteArray, Component*> outputs;
    QHash <QByteArray, Component*> components;

    void setInputsValue(const QByteArray &inputsValue);
    void setInputsValue(int inputsValue);
    void eval();
    void doRanging();
};

#endif // CIRCUIT_H
