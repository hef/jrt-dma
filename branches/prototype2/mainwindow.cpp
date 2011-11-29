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

    connect( ui->actionAbout, SIGNAL( triggered() ), this, SLOT( about() ) );
    connect( ui->actionAddTab, SIGNAL( triggered() ), this, SLOT( addTab() ) );
    connect( ui->actionTest_2, SIGNAL( triggered() ), this, SLOT( test() ) );
    connect( ui->actionExit, SIGNAL( triggered() ), this, SLOT( exit() ) );

    removeTab( QString( "Thread 1") );
    removeTab( QString( "Thread 2") );
    removeTab( QString( "Thread 3") );
    addTab( QString( "DEFAULT") );

    iterate = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::exit()
{
    exit();
}

void MainWindow::test()
{
    switch( iterate )
    {
    case 0:
        addTab( QString( "DestroyJavaVM") );
        break;
    case 1:
        addTab( QString( "Thread-1") );
        break;
    case 2:
        addTab( QString( "Thread-0") );
        break;
    case 3:
        addTab( QString( "Attach Listener") );
        addTab( QString( "Signal Dispatcher") );
        addTab( QString( "Finalizer") );
        addTab( QString( "Reference Handler") );
        break;
    case 4:
        QMessageBox::about(this, tr("Java Run Time Deadlock Monitoring agent (jrt-dma)"),
                           tr("Beginning deadlock detection<br>"));
        break;
    case 5:
        removeTab( QString( "Reference Handler") );
        break;
    case 6:
        removeTab( QString( "Finalizer") );
        break;
    case 7:
        removeTab( QString( "Signal Dispatcher") );
        break;
    case 8:
        removeTab( QString( "Attach Listener") );
        break;
    case 9:
        removeTab( QString( "DestroyJavaVM") );
        ui->tabWidget->setCurrentIndex( findTab( QString("Thread-0")) );
        break;
    case 10:
        addThreadDependency( findTab( QString( "Thread-0")), QString("BLOCKED on java.lang.String@419431dc owned by: Thread-1") );
        addStackLine( findTab(QString( "Thread-0")), QString( "deadlock$1.run(deadlock.java:26)"));
        addSyncedObj( findTab( QString( "Thread-0")), QString( " - locked java.lang.String@4c48d0c9"));
        break;
    case 11:
        ui->tabWidget->setCurrentIndex( findTab( QString("Thread-1")) );
        addThreadDependency( findTab( QString( "Thread-1")), QString("BLOCKED on java.lang.String@4c48d0c9 owned by: Thread-0") );
        addStackLine( findTab(QString( "Thread-1")), QString( "deadlock$2.run(deadlock.java:49)"));
        addSyncedObj( findTab( QString( "Thread-1")), QString( " - locked java.lang.String@419431dc"));
        break;
    }

    iterate++;
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"),
                       tr("Created for CS442 Fall 2010<br>"
                          "The authors are Steven Sennebogen,<br>"
                          "Sarath Kumar,<br>"
                          "Mike Salata,<br>"
                          "Niyaz Amanullah,<br>"));
    //addStackLine( 3, "testAddStackLine" );
    //addThreadDependency( 3, "testAddThreadDependency" );
    //addSyncedObj( 3, "testAddSyncedObj" );
}


int MainWindow::findTab( QString tabName )
{
    for( int i =(ui->tabWidget->count()-1); i>=0; i--)
    {
        if( ui->tabWidget->tabText(i) == tabName )
            return i;
    }
    return -1;
}

void MainWindow::removeTab( QString tabName )
{
    if( ui->tabWidget->count() == 1 )
        addTab( QString( "DEFAULT") );
    int whatWeFind = findTab(tabName);
    if( whatWeFind >= 0 );
        ui->tabWidget->removeTab( whatWeFind );
}

void MainWindow::addTab()
{
    QString newtab = "NewTab";
    addTab( newtab );
}


void MainWindow::addTab( QString tabName )
{
    QSplitter *threadOutputSplitter;
    int myNewTab=0;

    myNewTab = ui->tabWidget->addTab( new QWidget( ui->tabWidget->widget( myNewTab ) ), tabName );

    threadOutputSplitter =  new QSplitter( ui->tabWidget->widget( myNewTab ) );
    threadOutputSplitter->setOrientation(Qt::Vertical);

    threadOutputSplitter->addWidget( new QLabel( "Thread Dependancies") );
    threadOutputSplitter->addWidget( new QListWidget );

    threadOutputSplitter->addWidget( new QLabel( "Call Stack") );
    threadOutputSplitter->addWidget( new QListWidget );

    threadOutputSplitter->addWidget( new QLabel( "Synchronized Objects") );
    threadOutputSplitter->addWidget( new QListWidget );

    if( ui->tabWidget->count() ==2 )
        removeTab( QString( "DEFAULT"));

}

void MainWindow::addStackLine( int thread, QString message )
{
    QListWidget* listDisplayed = getDisplayedList( thread, 1 );
    listDisplayed->addItem( message );
}

void MainWindow::removeStackLine( int thread, int row )
{
    QListWidget* listDisplayed = getDisplayedList( thread, 1 );
    listDisplayed->takeItem( row );//->~QListWidgetItem();
}

void MainWindow::addThreadDependency( int thread, QString message )
{
    QListWidget* listDisplayed = getDisplayedList( thread,2 );
    listDisplayed->addItem( message );
}

void MainWindow::removeThreadDependency( int thread, int row )
{
    QListWidget* listDisplayed = getDisplayedList( thread, 2 );
    listDisplayed->takeItem( row );//->~QListWidgetItem();
}


void MainWindow::addSyncedObj( int thread, QString message )
{

    QListWidget* listDisplayed = getDisplayedList( thread, 3 );
    listDisplayed->addItem( message );
}

void MainWindow::removeSyncedObj( int thread, int row )
{
    QListWidget* listDisplayed = getDisplayedList( thread, 3 );
    listDisplayed->takeItem( row );//->~QListWidgetItem();
}

QListWidget* MainWindow::getDisplayedList( int thread, int row )
{
    QObjectList queryList;
    queryList = ui->tabWidget->widget(thread)->children();

    QSplitter *outDisplaySplitter = (QSplitter*)queryList.back();
    QListWidget* listDisplayed;
    QWidget* temp = outDisplaySplitter->widget( 4-(row-1)*2 );
    queryList = temp->parent()->children();

    listDisplayed = (QListWidget*)queryList.at( 4-(row-1)*2 );
    listDisplayed = (QListWidget*)listDisplayed;
    return listDisplayed;
}
