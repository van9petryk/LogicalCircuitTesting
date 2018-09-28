#ifndef TESTTABLEMODEL_H
#define TESTTABLEMODEL_H

#include <QAbstractTableModel>
#include <QBrush>
#include "circuittest.h"
#include "testbuilder.h"

class TestTableModel : public QAbstractTableModel
{
public:
    TestTableModel(const ControlTestResult  &ctr, QObject *parent = 0);
    TestTableModel(const DiagnosticTestResult &dtr, QObject *parent = 0);
    TestTableModel(const DiagnosticTestInfo &dti, QObject *parent = 0);
    TestTableModel(const ControlTestInfo &cti, QObject *parent = 0);
    TestTableModel(const QList <QByteArray> inputs, const QMap <QByteArray, QByteArray> &result, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
private:
    QList <QByteArray> hHead, vHead;
    QList <QList <QByteArray> > table;
    QSet <int> highlight;
};

#endif // TESTTABLEMODEL_H
