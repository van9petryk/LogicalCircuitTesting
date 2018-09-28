#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupCentralWidget();
    setupMenu();
    setupStatusBar();
    setWindowTitle("Испытание тестов для комбинационных схем");
    resize(600, 400);
    ted->getEditor()->setFocus();
}

void MainWindow::updateCursorPosition()
{
    int row = ted->getEditor()->textCursor().blockNumber();
    int col = ted->getEditor()->textCursor().positionInBlock();
    curPos->setText("Row: " + QString::number(row + 1) + ", Col: " + QString::number(col + 1));
}

void MainWindow::taskChooseHandler()
{
    QAction *act = qobject_cast<QAction*>(sender());
    log->setPlainText("");
    static int i = 1;
    try {
        Parser parser(ted->getEditor()->toPlainText().toLocal8Bit());
        Circuit c(parser.getCircuitDescription());
        if (act->text() == "Моделирование исправной схемы") {
            simulation(c);
            return;
        }

        const auto &tasks = parser.getTasks();

        if (act->text() == "Контроль")
            doControlTest(c, tasks);
        else if (act->text() == "Диагностика")
            doDiagnosticTest(c, tasks);
        else if (act->text() == "Для контроля")
            buildControlTest(c, tasks);
        else if (act->text() == "Для диагностики")
            buildDiagnosticTest(c, tasks);
    }
    catch (const ParseError &e) {
        log->setPlainText("[" + QString::number(i++) + "] " + ted->getCurrentTabName() + ": " + e.what());
    }
    catch (const std::runtime_error &e) {
        QMessageBox::critical(0, "Ошибка", e.what());
    }
}

void MainWindow::doControlTest(Circuit &c, const QMap<QByteArray, TaskDescription> &t)
{
    const auto &ct = CircuitTest::doControlTest(c, t);
    if (ct.size() == 0) {
        QMessageBox::information(this, "Контроль невозможен", "Не обнаружено контролирующих тестов");
        return;
    }
    foreach (const QByteArray &taskName, ct.keys()) {
        TestWidget *tw = new TestWidget(this);
        tw->setWindowTitle("Результат контролю '" + ted->getCurrentTabName() + "': TASK '" + taskName + "'");
        foreach (const QByteArray &testName, ct[taskName].keys())
            tw->addControlResultTab(testName, ct[taskName][testName]);
        tw->show();
    }
}

void MainWindow::doDiagnosticTest(Circuit &c, const QMap<QByteArray, TaskDescription> &t)
{
    const auto &dt = CircuitTest::doDiagnosticTest(c, t);
    if (dt.size() == 0) {
        QMessageBox::information(this, "Диагностика невозможна", "Не обнаружено диагностических тестов");
        return;
    }
    foreach (const QByteArray &taskName, dt.keys()) {
        TestWidget *tw = new TestWidget(this);
        tw->setWindowTitle("Результат діагностики '" + ted->getCurrentTabName() +
                           "': TASK '" + taskName + "'");
        foreach (const QByteArray &testName, dt[taskName].keys())
            tw->addDiagnosticResultTab(testName, dt[taskName][testName]);
        tw->show();
    }
}

void MainWindow::buildControlTest(Circuit &c, const QMap<QByteArray, TaskDescription> &t)
{
    if (t.size() == 0) {
        QMessageBox::information(this, "Построение контролирующего теста невозможно", "Не указаны неисправности");
        return;
    }

    TestWidget *tw = new TestWidget(this);
    tw->setWindowTitle("Контролирующие тесты для '" + ted->getCurrentTabName() + "'");
    foreach (const QByteArray &taskName, t.keys()) {
        const auto &ct = TestBuilder::buildControlTest(c, t[taskName].malfunctions);
        tw->addControlTestTab(taskName, ct);
    }
    tw->show();
}

void MainWindow::buildDiagnosticTest(Circuit &c, const QMap<QByteArray, TaskDescription> &t)
{
    if (t.size() == 0) {
        QMessageBox::information(this, "Построение диагностического теста невозможно", "Не указаны неисправности");
        return;
    }

    TestWidget *tw = new TestWidget(this);
    tw->setWindowTitle("Диагностические тесты для '" + ted->getCurrentTabName() + "'");
    foreach (const QByteArray &taskName, t.keys()) {
        const auto &ct = TestBuilder::buildDiagnosticTest(c, t[taskName].malfunctions);
        tw->addDiagnosticTestTab(taskName, ct);
    }
    tw->show();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (ted->closeAll())
        QMainWindow::closeEvent(event);
    else
        event->ignore();
}

void MainWindow::setupMenu()
{
    QMenu *fileMenu = new QMenu("Файл", this);
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction("Новый", ted, SLOT(newTab()), QKeySequence::New);
    fileMenu->addAction("Открыть", ted, SLOT(openFile()), QKeySequence::Open);

    QMenu *simulationMenu  = new QMenu("Моделирование", this);
    menuBar()->addMenu(simulationMenu);
    simulationMenu->addAction("Моделирование исправной схемы", this, SLOT(taskChooseHandler()));

    QMenu *doTestMenu = new QMenu("Тестирование", this);
    menuBar()->addMenu(doTestMenu);

    doTestMenu->addAction("Контроль", this, SLOT(taskChooseHandler()));
    doTestMenu->addAction("Диагностика", this, SLOT(taskChooseHandler()));

    QMenu *buildTestMenu = new QMenu("Построение тестов");
    menuBar()->addMenu(buildTestMenu);

    buildTestMenu->addAction("Для контроля", this, SLOT(taskChooseHandler()));
    buildTestMenu->addAction("Для диагностики", this, SLOT(taskChooseHandler()));
}

void MainWindow::setupCentralWidget()
{
    ted = new TabbedEditor;
    log = new QPlainTextEdit;
    log->setReadOnly(true);
    QTabWidget *tabLog = new QTabWidget;
    tabLog->addTab(log, "Проблемы");
    tabLog->setMaximumHeight(100);

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->setMargin(0);
    vbox->setSpacing(0);
    vbox->addWidget(ted);
    vbox->addWidget(tabLog);

    QWidget *widget = new QWidget;
    widget->setLayout(vbox);

    setCentralWidget(widget);
}

void MainWindow::setupStatusBar()
{
    curPos = new QLabel;
    statusBar()->addPermanentWidget(curPos);
    connect(ted->getEditor(), SIGNAL(cursorPositionChanged()), this, SLOT(updateCursorPosition()));
    updateCursorPosition();
}

void MainWindow::simulation(Circuit &c)
{
    const auto &result = c.simulate();

    QTableView *view = new QTableView;
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    view->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    view->verticalHeader()->setDefaultSectionSize(view->verticalHeader()->minimumSectionSize());
    view->setAttribute(Qt::WA_DeleteOnClose);
    view->resize(400, 500);

    TestTableModel *table = new TestTableModel(c.getNameInputs(), result, view);

    view->setModel(table);
    view->setWindowTitle("Результаты моделирования исправной схемы '" + ted->getCurrentTabName() + "'");
    view->show();
}
