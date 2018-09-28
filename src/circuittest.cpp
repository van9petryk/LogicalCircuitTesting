#include "circuittest.h"

QMap<QByteArray, QMap<QByteArray, ControlTestResult> > CircuitTest::doControlTest
(Circuit &c,const QMap<QByteArray, TaskDescription> &t)
{
    QMap <QByteArray, QMap <QByteArray, ControlTestResult> > ct;
    foreach (const QByteArray &taskName, t.keys()) {
        const auto &tests = t[taskName].tests;
        foreach (const QByteArray &testName, tests.keys()) {
            if (tests[testName].diagnostic.size() != 0)
                continue;
            ControlTestResult ctr;
            ctr.correct = c.simulate(tests[testName].test);
            foreach (const Malfunction &malfunc, t[taskName].malfunctions) {
                ctr.incorrect[malfunc] = c.simulate(tests[testName].test, malfunc);
                if (ctr.incorrect[malfunc] == ctr.correct)
                    ctr.undetected += malfunc;
            }
            ct[taskName][testName] = ctr;
        }
    }
    return ct;
}

QMap<QByteArray, QMap<QByteArray, DiagnosticTestResult> > CircuitTest::doDiagnosticTest
(Circuit &c, const QMap <QByteArray, TaskDescription> &t)
{
    QMap <QByteArray, QMap <QByteArray, DiagnosticTestResult> > dt;
    foreach (const QByteArray &taskName, t.keys()) {
        const auto &tests = t[taskName].tests;
        foreach (const QByteArray &testName, tests.keys()) {
            if (tests[testName].diagnostic.size() == 0)
                continue;

            DiagnosticTestResult dtr;
            const auto &malfuncs = t[taskName].malfunctions;

            foreach (const Malfunction &malfunc, malfuncs)
                dtr.simulated[malfunc] = c.simulate(tests[testName].test, malfunc);

            foreach (const QByteArray &out, tests[testName].diagnostic.keys())
                for (int i = 0; i < malfuncs.size(); i++)
                    dtr.expected[malfuncs.at(i)][out] = tests[testName].diagnostic[out].at(i);

            foreach (const Malfunction &malfunc, malfuncs) {
                if (dtr.simulated[malfunc] != dtr.expected[malfunc])
                    dtr.undefined += malfunc;
            }
            dt[taskName][testName] = dtr;
        }
    }
    return dt;
}
