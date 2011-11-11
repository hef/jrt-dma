#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QWidget>
#include <QHBoxLayout>
#include <QStringList>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect( ui->actionAbout, SIGNAL( triggered() ), this, SLOT(about()) );
    ui->listWidget_3->addItem(new QListWidgetItem("Oak"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_label_linkActivated(const QString &link)
{

}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Gitit"),
                       tr("JRT-D, Created for CS440 Fall 2010<br>"
                          "For access to source code, goto http://github.com/bdenne2/gitit<br>"
                          "Steven Sennebogen<br>"
                          "Sarath Kumar<br>"
                          "Mike Salata<br>"
                          "Niyaz Amanullah<br>"));
    //listWidget_3->addItem(new QListWidgetItem("Oak"));
}

void MainWindow::on_listWidget_3_viewportEntered()
{
    ui->listWidget_3->addItem(new QListWidgetItem("Oak"));
}
