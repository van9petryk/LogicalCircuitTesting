#include "testbuilder.h"

QList<int> TestBuilder::minimize(QList<QByteArray> &tp)
{
    QList <int> idxTest;
    do {
        int idx = 0, max = 0;
        for (int i = 0; i < tp.first().size(); i++) {
            int count = 0;
            foreach(const QByteArray &col, tp)
                if (col.at(i) == '1')
                    count++;

            if (count > max) {
                idx = i;
                max = count;
            }
        }

        foreach (const QByteArray &col, tp)
            if (col.at(idx) == '1')
                tp.removeOne(col);

        idxTest << idx;
    } while (!tp.empty());

    return idxTest;
}

ControlTestInfo TestBuilder::buildControlTest(Circuit &circuit, const QList<Malfunction> &malfuncs)
{
    ControlTestInfo cti;
    QList <QByteArray> tp;
    const auto &correct = circuit.simulate();
    foreach (const Malfunction &malfunc, malfuncs) {
        const auto &incorrect = circuit.simulate(malfunc);
        QByteArray col;
        for (int i = 0; i < pow(2, circuit.inputsCount()); i++) {
            bool different = false;
            foreach (const QByteArray &out, circuit.getNameOutputs())
                if (correct[out].at(i) != incorrect[out].at(i)) {
                    different = true;
                    break;
                }
            col += different + '0';
        }
        if (col.contains('1'))
            tp += col;
        else
            cti.undetected += malfunc;
    }

    const auto &idxTest = minimize(tp);

    foreach (int idx, idxTest) {
        cti.test << QByteArray::number(idx, 2).rightJustified(circuit.inputsCount(), '0');
        foreach (const QByteArray &out, circuit.getNameOutputs())
            cti.outputs[out] += correct[out].at(idx);
    }

    return cti;
}

DiagnosticTestInfo TestBuilder::buildDiagnosticTest(Circuit &circuit, const QList<Malfunction> &malfuncs)
{
    DiagnosticTestInfo dti;
    QMap <Malfunction, QMap <QByteArray, QByteArray> > incorrect;
    foreach (const Malfunction &malfunc, malfuncs)
        incorrect[malfunc] = circuit.simulate(malfunc);

    QList <QByteArray> tp;
    for (int i = 0; i < malfuncs.size() - 1; i++) {
        for (int j = i + 1; j < malfuncs.size(); j++) {
            QByteArray col;
            for (int k = 0; k < pow(2, circuit.inputsCount()); k++) {
                bool different = false;
                foreach (const QByteArray &out, circuit.getNameOutputs())
                    if (incorrect[malfuncs.at(i)][out].at(k) != incorrect[malfuncs.at(j)][out].at(k)) {
                        different = true;
                        break;
                    }
                col += different + '0';
            }
            if (col.contains('1'))
                tp += col;
            else
                dti.indistinguishable += qMakePair(malfuncs.at(i), malfuncs.at(j));
        }
    }

    const auto &idxTest = minimize(tp);

    foreach (int idx, idxTest) {
        dti.test << QByteArray::number(idx, 2).rightJustified(circuit.inputsCount(), '0');
        foreach (const QByteArray &out, circuit.getNameOutputs())
            foreach (const Malfunction &malfunc, malfuncs)
                dti.outputs[malfunc][out] += incorrect[malfunc][out].at(idx);
    }

    return dti;
}
