#include "testmainwindow.h"
#include "mainwindow.h"
void TestMainWindow::init()
{
    mainWindow = new MainWindow();
}
void TestMainWindow::cleanup()
{
    delete mainWindow;
}

void TestMainWindow::show()
{
    mainWindow->show();
    QVERIFY(mainWindow->isVisible());
}

