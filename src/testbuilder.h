#ifndef TESTBUILDER_H
#define TESTBUILDER_H

#include "circuit.h"
struct ControlTestInfo {
    QList <QByteArray> test;
    QMap <QByteArray, QByteArray> outputs;
    QList <Malfunction> undetected;
};

struct DiagnosticTestInfo {
    QList <QByteArray> test;
    QMap <Malfunction, QMap <QByteArray, QByteArray> > outputs;
    QList <QPair <Malfunction, Malfunction> > indistinguishable;
};

class TestBuilder
{
public:
    static ControlTestInfo buildControlTest(Circuit &circuit, const QList <Malfunction> &malfuncs);
    static DiagnosticTestInfo buildDiagnosticTest(Circuit &circuit, const QList <Malfunction> &malfuncs);
private:
    static QList <int> minimize(QList<QByteArray> &tp);
};

#endif // TESTBUILDER_H
