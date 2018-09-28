#ifndef TABBEDEDITOR_H
#define TABBEDEDITOR_H

#include "codeeditor.h"
#include <QTabBar>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QDir>

class TabbedEditor : public QWidget {
    Q_OBJECT
public:
    TabbedEditor(QWidget *parent = 0);
    CodeEditor* getEditor() { return editor; }
    QString getCurrentTabName() { return tabBar->tabText(tabBar->currentIndex()); }
    bool closeAll();
public slots:
    void loadTabData() { editor->setPlainText(tabBar->tabData(tabBar->currentIndex()).toString()); }
    bool closeTab(int idx);
    void onTabClicked(int idx);

    void newTab();
    void openFile();

private:
    CodeEditor *editor;
    QTabBar *tabBar;
    void saveTabData() { tabBar->setTabData(tabBar->currentIndex(), editor->toPlainText()); }
    bool saveFile(int idx);
    int askSave(const QString &tab);
};

#endif // TABBEDEDITOR_H
