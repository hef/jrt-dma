#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QWidget>
#include <QHBoxLayout>
#include <QStringList>
#include <vector>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect( ui->actionAbout, SIGNAL( triggered() ), this, SLOT( about() ) );
    connect( ui->actionAddTab, SIGNAL( triggered() ), this, SLOT( addTab() ) );

    //vector<QListWidget*> displayBoxes;
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
    QString testcallstack = "testcallstack";
    addStackLine( 3, "testcallstack" );
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

void MainWindow::addStackLine( int thread, QString line )
{

    QObjectList queryList;
    //QMessageBox::about(this,tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"), ui->tabWidget->widget(thread)->metaObject()->className());
    queryList = ui->tabWidget->widget(thread)->children();
    //QMessageBox::about(this,tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"), queryList.back()->metaObject()->className());
    //queryList = queryList.back()->children();
    //queryList = queryList.back()->children();
    //QListWidget *listDisplayed = queryList.at( 3 )->metaObject();
    //QString myName = queryList.at( 3 )->metaObject()->className();
    //listDisplayed->addItem( line );

    QSplitter *outDisplaySplitter = (QSplitter*)queryList.back();
    QMessageBox::about(this,tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"), ""+outDisplaySplitter->count() );
    /*
    if( queryList.back()->metaObject()- )
    {
        outDisplaySplitter = queryList.back()->metaObject()->splitter();
    }
    else
        outDisplaySplitter = (QSplitter*)queryList.back()->metaObject();

        */
    //QSplitter *outDisplaySplitter = (QSplitter*)queryList.back()->metaObject();
    for( int i = (outDisplaySplitter->count()-1); i>=0; i-- )
    {
        //QMessageBox::about(this,tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"), outDisplaySplitter->widget( i )->metaObject()->className() );
        queryList = outDisplaySplitter->children();
        //QMessageBox::about(this,tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"), queryList.back()->metaObject()->className());
    }
    QListWidget* listDisplayed;
    QWidget* temp = outDisplaySplitter->widget( 2 );
    //QMessageBox::about(this,tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"), temp->parent()->metaObject()->className());
    queryList = temp->parent()->children();
    //QMessageBox::about(this,tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"), queryList.back()->metaObject()->className());


    listDisplayed = (QListWidget*)queryList.front();
    listDisplayed = (QListWidget*)listDisplayed;
    listDisplayed->addItem( line );
    //listDisplayed->addItem( line );
    //queryList = outDisplaySplitter->children();
    //QMessageBox::about(this,tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"), ""+outDisplaySplitter->count() );
    //QListWidget* listDisplayed = (QListWidget*)queryList.back()->metaObject();
    //listDisplayed->addItem( line );

    //displayBoxes[(thread*3)+1]->addItem( line );
    /*
    ui->tabWidget->widget(thread)->
    //FUUUUCKYOU->
    QSplitter* FUUUUCKYOU2;
    QSplitter* FUUUUCKYOU = ((QSplitter*)(ui->tabWidget->widget(thread)));
    FUUUUCKYOU2 = (QSplitter*)(FUUUUCKYOU->widget( 3 ));
    FUUUUCKYOU2->addWidget( new QString( line ) );
    */
}

void MainWindow::addThreadDependency( int thread, QString line )
{
    QObjectList queryList;
    //QMessageBox::about(this,tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"), ui->tabWidget->widget(thread)->metaObject()->className());
    queryList = ui->tabWidget->widget(thread)->children();
    //QMessageBox::about(this,tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"), queryList.back()->metaObject()->className());
    //queryList = queryList.back()->children();
    //queryList = queryList.back()->children();
    //QListWidget *listDisplayed = queryList.at( 3 )->metaObject();
    //QString myName = queryList.at( 3 )->metaObject()->className();
    //listDisplayed->addItem( line );

    QSplitter *outDisplaySplitter = (QSplitter*)queryList.back();
    QMessageBox::about(this,tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"), ""+outDisplaySplitter->count() );
    /*
    if( queryList.back()->metaObject()- )
    {
        outDisplaySplitter = queryList.back()->metaObject()->splitter();
    }
    else
        outDisplaySplitter = (QSplitter*)queryList.back()->metaObject();

        */
    //QSplitter *outDisplaySplitter = (QSplitter*)queryList.back()->metaObject();
    for( int i = (outDisplaySplitter->count()-1); i>=0; i-- )
    {
        //QMessageBox::about(this,tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"), outDisplaySplitter->widget( i )->metaObject()->className() );
        queryList = outDisplaySplitter->children();
        //QMessageBox::about(this,tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"), queryList.back()->metaObject()->className());
    }
    QListWidget* listDisplayed;
    QWidget* temp = outDisplaySplitter->widget( 2 );
    //QMessageBox::about(this,tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"), temp->parent()->metaObject()->className());
    queryList = temp->parent()->children();
    //QMessageBox::about(this,tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"), queryList.back()->metaObject()->className());


    listDisplayed = (QListWidget*)queryList.front();
    listDisplayed = (QListWidget*)listDisplayed;
    listDisplayed->addItem( line );
    //listDisplayed->addItem( line );
    //queryList = outDisplaySplitter->children();
    //QMessageBox::about(this,tr("About Java Run Time Deadlock Monitoring agent (jrt-dma)"), ""+outDisplaySplitter->count() );
    //QListWidget* listDisplayed = (QListWidget*)queryList.back()->metaObject();
    //listDisplayed->addItem( line );

    //displayBoxes[(thread*3)+1]->addItem( line );
    /*
    ui->tabWidget->widget(thread)->
    //FUUUUCKYOU->
    QSplitter* FUUUUCKYOU2;
    QSplitter* FUUUUCKYOU = ((QSplitter*)(ui->tabWidget->widget(thread)));
    FUUUUCKYOU2 = (QSplitter*)(FUUUUCKYOU->widget( 3 ));
    FUUUUCKYOU2->addWidget( new QString( line ) );
    */
}

void MainWindow::addSyncedObj( int thread, QString line )
{

}

