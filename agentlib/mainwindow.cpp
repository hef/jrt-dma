#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect( ui->actionAbout, SIGNAL( triggered() ), this, SLOT( about() ) );
    connect( ui->actionAddTab, SIGNAL( triggered() ), this, SLOT( addTab() ) );
    connect( ui->actionTest, SIGNAL( triggered() ), this, SLOT( test() ) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::test()
{
    //removeStackLine( 3, 0 );
    //removeThreadDependency( 3, 0 );
    //removeSyncedObj( 3, 0 );

    removeTab( QString( "NewTab") );
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
