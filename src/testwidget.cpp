#include "testwidget.h"

TestWidget::TestWidget(QWidget *parent) : QTabWidget(parent)
{
    setWindowFlags(Qt::Window);
    setAttribute(Qt::WA_DeleteOnClose);
    resize(300, 400);
}

void TestWidget::addControlResultTab(const QString &tabName, const ControlTestResult &ctr)
{
    TestTableModel *table = new TestTableModel(ctr, this);
    QTableView *view = createTableView();
    view->setModel(table);

    QStringList undetected;
    foreach (const Malfunction &undetect, ctr.undetected)
        undetected += undetect.str();

    int countDetected = ctr.incorrect.size() - ctr.undetected.size();

    QLabel *lblUndetectedHead = createCustomHeadLabel("Не виявлені несправності:");
    QLabel *lblUndetectedMalf = createCustomRectLabel(undetected.join(", "));
    QLabel *lblStatistics = createCustomHeadLabel("Виявлено " + QString::number(countDetected) + "/" +
                                                  QByteArray::number(ctr.incorrect.size()) + " несправностей");

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(view);
    vbox->addWidget(lblUndetectedHead);
    vbox->addWidget(lblUndetectedMalf);
    vbox->addWidget(lblStatistics);

    QWidget *widget = new QWidget;
    widget->setLayout(vbox);
    addTab(widget, tabName);
}

void TestWidget::addDiagnosticResultTab(const QString &tabName, const DiagnosticTestResult &dtr)
{
    TestTableModel *table = new TestTableModel(dtr, this);
    QTableView *view = createTableView();
    view->setModel(table);

    QStringList undefined;
    foreach (const Malfunction &undef, dtr.undefined)
        undefined += undef.str();

    int countDefined = dtr.expected.size() - dtr.undefined.size();

    QLabel *lblWronglyDiagnosedHead = createCustomHeadLabel("Не вірно визначені несправності:");
    QLabel *lblWronglyDiagnosedMalf = createCustomRectLabel(undefined.join(", "));
    QLabel *lblStatistics = createCustomHeadLabel("Визначено " + QString::number(countDefined) + "/" +
                                                  QByteArray::number(dtr.expected.size()) + " несправностей");

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(view);
    vbox->addWidget(lblWronglyDiagnosedHead);
    vbox->addWidget(lblWronglyDiagnosedMalf);
    vbox->addWidget(lblStatistics);

    QWidget *widget = new QWidget;
    widget->setLayout(vbox);
    addTab(widget, tabName);
}

void TestWidget::addControlTestTab(const QString &tabName, const ControlTestInfo &cti)
{
    TestTableModel *table = new TestTableModel(cti, this);
    QTableView *view = createTableView();
    view->setModel(table);

    QStringList undetected;
    foreach (const Malfunction &undetect, cti.undetected)
        undetected += undetect.str();

    QLabel *lblHTest = createCustomHeadLabel("Тест");
    QLabel *lblHUndetectMalf = createCustomHeadLabel("Несправності, що не вивляє тест");
    QLabel *lblUndetectMalf = createCustomRectLabel(undetected.join(", "));

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(lblHTest);
    vbox->addWidget(view);
    vbox->addWidget(lblHUndetectMalf);
    vbox->addWidget(lblUndetectMalf);

    QWidget *widget = new QWidget;
    widget->setLayout(vbox);
    addTab(widget, tabName);
}

void TestWidget::addDiagnosticTestTab(const QString &tabName, const DiagnosticTestInfo &dti)
{
    TestTableModel *table = new TestTableModel(dti, this);
    QTableView *view = createTableView();
    view->setModel(table);

    QString test = dti.test.join(", ");

    QStringList indistinguishable;
    foreach (const auto &ind, dti.indistinguishable)
        indistinguishable += ind.first.str() + " и " + ind.second.str();

    QLabel *lblHTest = createCustomHeadLabel("Тест");
    QLabel *lblTest = createCustomRectLabel(test);
    QLabel *lblHMalf = createCustomHeadLabel("Несправності, що не розрізняє тест");
    QLabel *lblMalf = createCustomRectLabel(indistinguishable.join(", "));

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(lblHTest);
    vbox->addWidget(lblTest);
    vbox->addWidget(view);
    vbox->addWidget(lblHMalf);
    vbox->addWidget(lblMalf);

    QWidget *widget = new QWidget;
    widget->setLayout(vbox);
    addTab(widget, tabName);
}

QLabel *TestWidget::createCustomRectLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setStyleSheet("background-color:white; border: 1px inset black; padding: 5px 2px; font-size: 12px");
    lbl->setWordWrap(true);
    return lbl;
}

QLabel *TestWidget::createCustomHeadLabel(const QString &text)
{
    QLabel *lbl = new QLabel(text);
    lbl->setStyleSheet("font-size: 14px; font-weight: bold");
    lbl->setWordWrap(true);
    return lbl;
}

QTableView *TestWidget::createTableView()
{
    QTableView *view = new QTableView;
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    return view;
}
