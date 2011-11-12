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
    connect( ui->actionAddTab, SIGNAL( triggered() ), this, SLOT(addTab()) );
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
                       tr("JRT-D, Created for CS442 Fall 2010<br>"
                          "Steven Sennebogen<br>"
                          "Sarath Kumar<br>"
                          "Mike Salata<br>"
                          "Niyaz Amanullah<br>"));
}

void MainWindow::addTab()
{
    //QWidget myNewTab = new QWidget(ui->tabWidget, 0);
    int myNewTab = ui->tabWidget->addTab( new QWidget(ui->tabWidget, 0), "NewTab" );

    QSplitter *threadOutputSplitter = new QSplitter( ui->tabWidget->widget( myNewTab ) );
    //threadOutputSplitter->addWidget( new QListWidget() );
    //threadOutputSplitter->addWidget( new QLabel( "Thread Dependancies") );
    //QWidget threadDependancies =

    threadOutputSplitter->setOrientation(Qt::Vertical);
    //XXXsplitXXX->addWidget( new QVBox//addWidget( new QVBoxLayout() );


    threadOutputSplitter->addWidget( new QLabel( "Thread Dependancies") );
    threadOutputSplitter->addWidget(  new QListWidget() );
    threadOutputSplitter->addWidget( new QLabel( "Call Stack") );
    threadOutputSplitter->addWidget(  new QListWidget() );
    threadOutputSplitter->addWidget( new QLabel( "Synchronized Objects") );
    threadOutputSplitter->addWidget(  new QListWidget() );


    //QVBoxLayout *threadDependancies = new QVBoxLayout( threadOutputSplitter );

    //new QLabel( "Thread Dependancies", threadDependancies, 0);
    //new QListWidget( threadDependancies );
    //new QListWidget( *threadDependancies );

/*
    QVBoxLayout *threadDependancies = new QVBoxLayout( threadOutputSplitter );
    //threadDependancies->setOrientation()
    //QVBoxLayout *threadDependancies = new QVBoxLayout;

    threadDependancies->addWidget( new QLabel( "Thread Dependancies") );
    threadDependancies->addWidget(  new QListWidget() );


    QVBoxLayout *callStack = new QVBoxLayout( threadOutputSplitter );

    callStack->addWidget( new QLabel( "Call Stack") );
    callStack->addWidget(  new QListWidget() );

    QVBoxLayout *objSync = new QVBoxLayout( threadOutputSplitter );

    objSync->addWidget( new QLabel( "Synchronized Objects") );
    objSync->addWidget(  new QListWidget() );
*/

}


void MainWindow::addCallStackLine( int thread, QString line )
{
    //ui->tabWidget->widget( thread )->
}


