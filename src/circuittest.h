#ifndef CIRCUITTEST_H
#define CIRCUITTEST_H

#include "circuit.h"
#include "parsestructures.h"

struct ControlTestResult {
    QMap <QByteArray, QByteArray> correct;
    QMap <Malfunction, QMap <QByteArray, QByteArray> > incorrect;
    QList <Malfunction> undetected;
};

struct DiagnosticTestResult {
    QMap <Malfunction, QMap <QByteArray, QByteArray> > simulated;
    QMap <Malfunction, QMap <QByteArray, QByteArray> > expected;
    QList <Malfunction> undefined;
};

class CircuitTest
{
public:
    static QMap<QByteArray, QMap<QByteArray, ControlTestResult> >
    doControlTest(Circuit &c, const QMap<QByteArray, TaskDescription> &t);
    static QMap <QByteArray, QMap <QByteArray, DiagnosticTestResult> >
    doDiagnosticTest(Circuit &c, const QMap<QByteArray, TaskDescription> &t);
};

#endif // CIRCUITTEST_H
