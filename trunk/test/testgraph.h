#ifndef TESTGRAPH_H
#define TESTGRAPH_H

#include <QtTest/QtTest>
#include "graph.h"
class TestGraph : public QObject
{
    Q_OBJECT
private:
    graph<int>* DAG;
    list<int>* outlist;
private slots:
    void init();
    void cleanup();
    void testNumberOfVertices();
    void testNumberOfEdges();
};

#endif //TESTGRAPH
