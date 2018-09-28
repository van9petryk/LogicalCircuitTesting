#include "testttablemodel.h"

TestTableModel::TestTableModel(const ControlTestResult &ctr, QObject *parent) : QAbstractTableModel(parent)
{
    hHead = ctr.correct.keys();
    vHead += "---";
    foreach (const Malfunction &malfunc, ctr.incorrect.keys())
        vHead += malfunc.str();
    table += ctr.correct.values();
    foreach (const auto &val, ctr.incorrect)
        table += val.values();
    foreach (const Malfunction &malfunc, ctr.undetected) {
        int idx = vHead.indexOf(malfunc.str());
        if (idx != -1)
            highlight += idx;
    }
}

TestTableModel::TestTableModel(const DiagnosticTestResult &dtr, QObject *parent) : QAbstractTableModel(parent)
{
    hHead = dtr.simulated.first().keys();
    foreach (const QByteArray &out, dtr.simulated.first().keys())
        hHead += out + "'";
    foreach (const Malfunction &malfunc, dtr.simulated.keys())
        vHead += malfunc.str();
    foreach (const auto &malfunc, dtr.simulated.keys())
        table += dtr.simulated[malfunc].values() + dtr.expected[malfunc].values();
    foreach (const Malfunction &malfunc, dtr.undefined) {
        int idx = vHead.indexOf(malfunc.str());
        if (idx != -1)
            highlight += idx;
    }
}

TestTableModel::TestTableModel(const DiagnosticTestInfo &dti, QObject *parent)
    : QAbstractTableModel(parent)
{
    hHead = dti.outputs.first().keys();
    foreach (const Malfunction &malf, dti.outputs.keys())
        vHead += malf.str();
    foreach (const auto &malfunc, dti.outputs.values())
        table += malfunc.values();
}

TestTableModel::TestTableModel(const ControlTestInfo &cti, QObject *parent) : QAbstractTableModel(parent)
{
    hHead.append("Входной сигнал");
    hHead.append(cti.outputs.keys());
    for (int i = 0; i < cti.test.size(); i++) {
        vHead += QByteArray::number(i + 1);
        QList <QByteArray> row;
        row += cti.test.at(i);
        foreach (const QByteArray &out, cti.outputs)
            row += out.mid(i, 1);
        table << row;
    }
}

TestTableModel::TestTableModel(const QList<QByteArray> inputs, const QMap<QByteArray, QByteArray> &result,
                               QObject *parent) : QAbstractTableModel(parent)
{
    hHead.append(inputs);
    hHead.append(result.keys());
    int length = pow(2, inputs.count());
    for (int i = 0; i < length; i++) {
        vHead += QByteArray::number(i + 1);
        QList <QByteArray> row;
        QByteArray input = QByteArray::number(i, 2).rightJustified(inputs.count(), '0');
        for (int j = 0; j < inputs.count(); j++)
            row += input.mid(j, 1);
        foreach (const QByteArray &out, result)
            row += out.mid(i, 1);
        table << row;
    }
}

int TestTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return vHead.size();
}

int TestTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return hHead.size();
}

QVariant TestTableModel::data(const QModelIndex &index, int role) const
{
    int row = index.row(), col = index.column();
    if (role == Qt::DisplayRole)
        return table.at(row).at(col);
    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    return QVariant();
}

QVariant TestTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if (orientation == Qt::Horizontal)
            return hHead.at(section);
        return vHead.at(section);
    }
    if (role == Qt::ForegroundRole && orientation == Qt::Vertical)
        if (highlight.contains(section))
            return QBrush(Qt::red);

    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    return QVariant();
}

Qt::ItemFlags TestTableModel::flags(const QModelIndex &index) const
{
    Q_UNUSED(index);
    return Qt::ItemIsEnabled;
}
