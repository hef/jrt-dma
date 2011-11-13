#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "DDAgent_global.h"
#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addStackLine( int thread, QString message );
    void addThreadDependency( int thread, QString message );
    void addSyncedObj( int thread, QString message );
    void addTab( QString tabName );
    int findTab( QString tabName );  //returns the index of given tab
    int removeSyncedObj( int thread, QString message );

private slots:
    void about();
    void addTab();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
