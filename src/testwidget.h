#ifndef TESTWIDGET_H
#define TESTWIDGET_H

#include <QVBoxLayout>
#include <QLabel>
#include <QTabWidget>
#include <QTableView>
#include <QHeaderView>
#include "circuittest.h"
#include "testbuilder.h"
#include "testttablemodel.h"

class TestWidget : public QTabWidget
{
public:
    TestWidget(QWidget *parent = 0);
    void addControlResultTab(const QString &tabName, const ControlTestResult& ctr);
    void addDiagnosticResultTab(const QString &tabName, const DiagnosticTestResult &dtr);
    void addControlTestTab(const QString &tabName, const ControlTestInfo &cti);
    void addDiagnosticTestTab(const QString &tabName, const DiagnosticTestInfo &dti);
private:
    QLabel *createCustomRectLabel(const QString &text);
    QLabel *createCustomHeadLabel(const QString &text);
    QTableView *createTableView();
};

#endif // TESTWIDGET_H
