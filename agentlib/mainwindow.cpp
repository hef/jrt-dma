#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect( ui->actionAbout, SIGNAL( triggered() ), this, SLOT(about()) );
    connect( ui->actionAddTab, SIGNAL( triggered() ), this, SLOT(addTab()) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Gitit"),
                       tr("JRT-D, Created for CS442 Fall 2010<br>"
                          "Steven Sennebogen<br>"
                          "Sarath Kumar<br>"
                          "Mike Salata<br>"
                          "Niyaz Amanullah<br>"));
}

void MainWindow::addTab( QString tabName )
{
    int myNewTab = ui->tabWidget->addTab( new QWidget(ui->tabWidget, 0), tabName );

    QSplitter *threadOutputSplitter = new QSplitter( ui->tabWidget->widget( myNewTab ) );

    threadOutputSplitter->setOrientation(Qt::Vertical);

    threadOutputSplitter->addWidget( new QLabel( "Thread Dependancies") );
    threadOutputSplitter->addWidget(  new QListWidget() );
    threadOutputSplitter->addWidget( new QLabel( "Call Stack") );
    threadOutputSplitter->addWidget(  new QListWidget() );
    threadOutputSplitter->addWidget( new QLabel( "Synchronized Objects") );
    threadOutputSplitter->addWidget(  new QListWidget() );

}

void MainWindow::addCallStackLine( int thread, QString line )
{
    //ui->tabWidget->widget( thread )->
}
