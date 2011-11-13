#ifndef TESTMAINWINDOW_H
#define TESTMAINWINDOW_H
#include <QtTest/QtTest>
class MainWindow;
class TestMainWindow : public QObject
{
    Q_OBJECT
private:
    MainWindow* mainWindow;
private slots:
    void init();
    void cleanup();
    void show();
};

#endif // TESTMAINWINDOW_H
