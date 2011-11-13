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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"),
                       tr("Created for CS442 Fall 2010<br>"
                          "The authors are Steven Sennebogen,<br>"
                          "Sarath Kumar,<br>"
                          "Mike Salata,<br>"
                          "Niyaz Amanullah,<br>"));
    addStackLine( 3, "testAddStackLine" );
    addThreadDependency( 3, "testAddThreadDependency" );
    addSyncedObj( 3, "testAddSyncedObj" );
}


int MainWindow::findTab( QString tabName )
{
    for( int i =(ui->tabWidget->count()-1); i<=0; i--)
    {
        if( ui->tabWidget->tabText(i) == tabName )
            return i;
    }
    return NULL;
}


void MainWindow::addTab()
{
    QString newtab = "NewTab";
    addTab( newtab );
}


void MainWindow::addTab( QString tabName )
{
    QSplitter *threadOutputSplitter;
    //QWidget myNewTab = new QWidget(ui->tabWidget, 0);
    int myNewTab=0;

    myNewTab = ui->tabWidget->addTab( new QWidget( ui->tabWidget->widget( myNewTab ) ), tabName );

    //QWidget* itemToSearch = ui->tabWidget->widget( myNewTab );


    //threadOutputSplitter->addWidget( new QListWidget() );
    //threadOutputSplitter->addWidget( new QLabel( "Thread Dependancies") );
    //QWidget threadDependancies =
    //XXXsplitXXX->addWidget( new QVBox//addWidget( new QVBoxLayout() );

    //threadOutputSplitter =  ui->tabWidget->widget( myNewTab );
    threadOutputSplitter =  new QSplitter( ui->tabWidget->widget( myNewTab ) );
    threadOutputSplitter->setOrientation(Qt::Vertical);
    //vector<QListWidget*> displayBoxes;

/*
    new QLabel( "Thread Dependancies",threadOutputSplitter,0);
    new QListWidget( threadOutputSplitter );
    new QLabel( "Call Stack",threadOutputSplitter,0);
    new QListWidget( threadOutputSplitter );
    new QLabel( "Synchronized Objects",threadOutputSplitter,0);
    new QListWidget( threadOutputSplitter );
    */


    threadOutputSplitter->addWidget( new QLabel( "Thread Dependancies") );
    //displayBoxes.push_back( new QListWidget );
    threadOutputSplitter->addWidget( new QListWidget );

    threadOutputSplitter->addWidget( new QLabel( "Call Stack") );
    //displayBoxes.push_back( new QListWidget );
    threadOutputSplitter->addWidget( new QListWidget );

    threadOutputSplitter->addWidget( new QLabel( "Synchronized Objects") );
    //displayBoxes.push_back( new QListWidget );
    threadOutputSplitter->addWidget( new QListWidget );


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

void MainWindow::addStackLine( int thread, QString message )
{
    QObjectList queryList;queryList = ui->tabWidget->widget(thread)->children();

    QSplitter *outDisplaySplitter = (QSplitter*)queryList.back();
    QListWidget* listDisplayed;
    QWidget* temp = outDisplaySplitter->widget( 2 );queryList = temp->parent()->children();

    listDisplayed = (QListWidget*)queryList.at(2);
    listDisplayed = (QListWidget*)listDisplayed;
    listDisplayed->addItem( message );
}

void MainWindow::addThreadDependency( int thread, QString message )
{
    QObjectList queryList;
    queryList = ui->tabWidget->widget(thread)->children();

    QSplitter *outDisplaySplitter = (QSplitter*)queryList.back();
    QListWidget* listDisplayed;
    QWidget* temp = outDisplaySplitter->widget( 4 );
    queryList = temp->parent()->children();

    listDisplayed = (QListWidget*)queryList.at(4);
    listDisplayed = (QListWidget*)listDisplayed;
    listDisplayed->addItem( message );
}

void MainWindow::addSyncedObj( int thread, QString message )
{
    QObjectList queryList;
    queryList = ui->tabWidget->widget(thread)->children();

    QSplitter *outDisplaySplitter = (QSplitter*)queryList.back();
    QListWidget* listDisplayed;
    QWidget* temp = outDisplaySplitter->widget( 0 );
    queryList = temp->parent()->children();

    listDisplayed = (QListWidget*)queryList.at(0);
    listDisplayed = (QListWidget*)listDisplayed;
    listDisplayed->addItem( message );
}
