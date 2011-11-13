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

    void addThreadDependency( int thread, QString message );
    void removeThreadDependency( int thread, int row );

    void addStackLine( int thread, QString message );
    void removeStackLine( int thread, int row );

    void addSyncedObj( int thread, QString message );
    void removeSyncedObj( int thread, int row );

    void addTab( QString tabName );  //adds a tab to the UI with the given Name
    void removeTab( QString tabName );  //removes a tab to the UI with the given Name
    int findTab( QString tabName );  //returns the index of given tabName

    QListWidget* getDisplayedList( int thread, int row );

private slots:
    void about();
    void addTab();
    void test();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
