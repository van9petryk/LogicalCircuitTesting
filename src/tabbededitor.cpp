#include "tabbededitor.h"

TabbedEditor::TabbedEditor(QWidget *parent) : QWidget(parent)
{
    tabBar = new QTabBar;
    editor = new CodeEditor;

    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(tabBar);
    vbox->addWidget(editor);
    vbox->setMargin(0);
    vbox->setSpacing(0);
    setLayout(vbox);

    tabBar->setDrawBase(false);
    tabBar->setTabsClosable(true);
    newTab();

    connect(tabBar, SIGNAL(tabBarClicked(int)), this, SLOT(onTabClicked(int)));
    connect(tabBar, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(tabBar, SIGNAL(currentChanged(int)), this, SLOT(loadTabData()));
}

void TabbedEditor::newTab()
{
    int i = 1;
    QString tabName;
    bool found;
    do {
        found = true;
        tabName = "Circuit " + QString::number(i++);

        for (int j = 0; j < tabBar->count(); j++)
            if (tabBar->tabText(j) == tabName) {
                found = false;
                break;
            }
    } while (!found);
    tabBar->addTab(tabName);

    saveTabData();
    tabBar->setCurrentIndex(tabBar->count() - 1);
}

bool TabbedEditor::closeAll()
{
    bool closedAll = true;
    for (int i = tabBar->count() - 1; i >= 0; i--) {
        if (!closeTab(i)) {
            closedAll = false;
            break;
        }
    }
    return closedAll;
}

void TabbedEditor::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "Tests/", "Circuit Test File (*.ct)");

    if (fileName.isEmpty())
        return;
    QFile file(fileName);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        int idx = tabBar->currentIndex();
        QString baseName = QFileInfo(fileName).baseName();
        if (!tabBar->accessibleTabName(idx).isEmpty() || !editor->toPlainText().isEmpty()) {
            saveTabData();
            tabBar->addTab(baseName);
            idx = tabBar->count() - 1;
            tabBar->setCurrentIndex(idx);
        }
        else
            tabBar->setTabText(idx, baseName);
        tabBar->setAccessibleTabName(idx, fileName);
        editor->setPlainText(file.readAll());
        file.close();
    }
}

bool TabbedEditor::saveFile(int idx)
{
    QString fileName = tabBar->accessibleTabName(idx);
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadWrite | QIODevice::Text)) {
            if (file.readAll() != tabBar->tabData(idx)) {
                int code = askSave(tabBar->tabText(idx));
                if (code == QMessageBox::Cancel)
                    return false;
                else if (code == QMessageBox::Save) {
                    file.resize(0);
                    file.write(tabBar->tabData(idx).toByteArray());
                }
            }
        }
    }
    else if (!tabBar->tabData(idx).toByteArray().isEmpty()) {
        int code = askSave(tabBar->tabText(idx));
        if (code == QMessageBox::Cancel)
            return false;
        else if (code == QMessageBox::Save) {
            QString folder = "Tests";
            if (!QDir(folder).exists())
                QDir().mkdir(folder);
            QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", folder + "/" +
                                                            tabBar->tabText(idx), "Circuit Test File (*.ct)");
            if (fileName.isEmpty())
                return false;
            QFile file(fileName);
            if (file.open(QIODevice::WriteOnly | QIODevice::Text))
                file.write(tabBar->tabData(idx).toByteArray());
        }
    }
    return true;
}

void TabbedEditor::onTabClicked(int idx)
{
    if (idx == -1 || tabBar->currentIndex() == idx)
        return;
    saveTabData();
}

bool TabbedEditor::closeTab(int idx)
{
    saveTabData();

    if (!saveFile(idx))
        return false;

    tabBar->removeTab(idx);

    if (tabBar->count() == 0)
        newTab();

    return true;
}

int TabbedEditor::askSave(const QString &tab)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Сохранить");
    msgBox.setText("Сохранить изменения в '" + tab + "'?");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    return msgBox.exec();
}
