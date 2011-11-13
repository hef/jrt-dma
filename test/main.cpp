#include <QtGui/QApplication>
#include "testmainwindow.h"
#include "testgraph.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int result = 0;
    {
       TestMainWindow tc;
       result += QTest::qExec(&tc, argc, argv);
    }
    {
       TestGraph tc;
       result += QTest::qExec(&tc, argc, argv);
    }
    return result;
}
