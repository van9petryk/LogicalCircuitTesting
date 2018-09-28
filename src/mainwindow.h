#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QFileDialog>
#include <QMenuBar>
#include <QTabBar>
#include <QVBoxLayout>
#include <QFileInfo>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <QCloseEvent>
#include <QStatusBar>
#include "codeeditor.h"
#include "circuit.h"
#include "parser.h"
#include "testwidget.h"
#include "testbuilder.h"
#include "tabbededitor.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);

public slots:
    void updateCursorPosition();
    void taskChooseHandler();
protected:
    void closeEvent(QCloseEvent * event);

private:
    void setupMenu();
    void setupCentralWidget();
    void setupStatusBar();
    void simulation(Circuit &c);
    void doControlTest(Circuit &c, const QMap <QByteArray, TaskDescription> &t);
    void doDiagnosticTest(Circuit &c, const QMap <QByteArray, TaskDescription> &t);
    void buildControlTest(Circuit &c, const QMap <QByteArray, TaskDescription> &t);
    void buildDiagnosticTest(Circuit &c, const QMap <QByteArray, TaskDescription> &t);

    TabbedEditor *ted;
    QPlainTextEdit *log;
    QLabel *curPos;
};

#endif // MAINWINDOW_H
