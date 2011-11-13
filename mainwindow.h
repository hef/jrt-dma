#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <vector>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void addStackLine( int thread, QString line );
    void addThreadDependency( int thread, QString line );
    void addSyncedObj( int thread, QString line );
    //vector<QListWidget*> displayBoxes;
    void addTab( QString tabName );

private slots:
    void about();
    void addTab();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
