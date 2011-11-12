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
    void addCallStackLine( int thread, QString line );

private slots:
    void about();
    void addTab( QString tabName );

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
