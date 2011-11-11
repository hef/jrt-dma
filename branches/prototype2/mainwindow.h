#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private slots:
    void about();
    void on_label_linkActivated(const QString &link);

    void on_listWidget_3_viewportEntered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
