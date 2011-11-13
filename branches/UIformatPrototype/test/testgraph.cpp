#include "testgraph.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef CLEAR_GRAPH
#define CLEAR_GRAPH TRUE
#endif

void TestGraph::init()
{
    DAG = new graph<int>;
    outlist = new list<int>;

    int vertexA = 1;
    int vertexB = 2;
    int vertexC = 3;
    int vertexD = 4;
    int vertexE = 5;

    DAG->insertVertex(vertexA); // test case insert a vertex
    DAG->insertVertex(vertexB); // test case insert a vertex
    DAG->insertVertex(vertexC); // test case insert a vertex
    DAG->insertVertex(vertexD); // test case insert a vertex
    DAG->insertVertex(vertexE); // test case insert a vertex

    DAG->insertEdge(vertexA,vertexC); // test case insert edge A to C
    DAG->insertEdge(vertexA,vertexD); // test case insert edge A to D
    DAG->insertEdge(vertexB,vertexA); // test case insert edge B to A
    DAG->insertEdge(vertexB,vertexD); // test case insert edge B to D
    DAG->insertEdge(vertexD,vertexE); // test case insert edge D to E
}
void TestGraph::cleanup()
{
    delete DAG;
    delete outlist;
}
void TestGraph::testNumberOfVertices()
{
    QCOMPARE(DAG->numberOfVertices(),5);
}
void TestGraph::testNumberOfEdges()
{
    QCOMPARE(DAG->numberOfEdges(),5);
}


/** Testcases that need to be converted to the qtest framework
  @test The original test cases
  These test cases are still usefule despite not being automated
  */
/*
int ConvertToQtTestCases()
{

        int vertexA = 1;
        int vertexB = 2;
        int vertexC = 3;
        int vertexD = 4;
        int vertexE = 5;

        DAG.insertVertex(vertexA); // test case insert a vertex
        DAG.insertVertex(vertexB); // test case insert a vertex
        DAG.insertVertex(vertexC); // test case insert a vertex
        DAG.insertVertex(vertexD); // test case insert a vertex
        DAG.insertVertex(vertexE); // test case insert a vertex

        DAG.insertEdge(vertexA,vertexC); // test case insert edge A to C
        DAG.insertEdge(vertexA,vertexD); // test case insert edge A to D
        DAG.insertEdge(vertexB,vertexA); // test case insert edge B to A
        DAG.insertEdge(vertexB,vertexD); // test case insert edge B to D
        DAG.insertEdge(vertexD,vertexE); // test case insert edge D to E
        DAG.insertEdge(vertexE,vertexB); // test case insert edge E to B

	if (DAG.empty()) // test case empty graph
	{
		cout << "no vertices so far\n";
		//test case check num vertices
		cout << "Number of vertices: " << DAG.numberOfVertices() << "\n";
		//test case check num edges
		cout << "Number of edges: " << DAG.numberOfEdges() << "\n";
	}
	else // test case non empty graph
	{
		cout << "Number of vertices: " << DAG.numberOfVertices() << "\n";
		cout << "Number of edges: " << DAG.numberOfEdges() << "\n";
	}

	if (DAG.noCycle(DAG))
		cout << "no cycle" << "\n";
	else
		cout << "has cycle" << "\n";

	DAG.topologicalSort(DAG,outlist);

	if (CLEAR_GRAPH)
	{
		DAG.clear();
		cout << "Number of vertices: " << DAG.numberOfVertices() << "\n";
		cout << "Number of edges: " << DAG.numberOfEdges() << "\n";
	}

	else
	{
		DAG.eraseEdge(vertexA,vertexB); // test case 
		cout << "Number of vertices: " << DAG.numberOfVertices() << "\n";
		cout << "Number of edges: " << DAG.numberOfEdges() << "\n";

		DAG.eraseVertex(vertexB);
		cout << "Number of vertices: " << DAG.numberOfVertices() << "\n";
		cout << "Number of edges: " << DAG.numberOfEdges() << "\n";

		DAG.eraseVertex(vertexA);
		cout << "Number of vertices: " << DAG.numberOfVertices() << "\n";
		cout << "Number of edges: " << DAG.numberOfEdges() << "\n";
	}


	return 0;
}
*/
