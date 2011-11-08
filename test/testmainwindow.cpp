#include "testmainwindow.h"
#include "mainwindow.h"
void TestMainWindow::initTestCase()
{
    mainWindow = new MainWindow();
}
void TestMainWindow::cleanupTestCase()
{
    delete mainWindow;
}

void TestMainWindow::show()
{
    mainWindow->show();
    QVERIFY(mainWindow->isVisible());
}

