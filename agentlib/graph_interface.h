#ifndef GRAPH_INTERFACE_H
#define GRAPH_INTERFACE_H
#include <iostream>
#include <fstream>

#include <set>				// set class
#include <map>				// ist classmap class
#include <vector>			// vector class
#include <list>			// list class
#include <stack>			// stack class
#include <queue>			// queue class
#include <functional>	// less<T>

template <typename T>
class graph
{
   public:
      class const_iterator: public map< T, int >::const_iterator
      {
         public:
            const_iterator()
            {}

				// converts a map iterator to a graph iterator
            const_iterator(map<T,int>::const_iterator i)
            {
					*((map< T, int >::const_iterator *)this) = i;
			}

				// return the vertex pointed to by the iterator
            const T& operator* () const
            {
               map<T,int>::const_iterator p = *this;

               return (*p).first;
            }
      };

      typedef const_iterator iterator;

		// constructors
	  graph(); // constructor. initialize numVertices and numEdges to 0
	  graph(const graph<T>& g);	// copy constructor

	    // check graph
      bool empty() const; // is the graph empty?

		// access methods
	  int numberOfVertices() const;	// return the number of vertices in the graph
	  int numberOfEdges() const; // return the number of edges in the graph
	  int inDegree(const T& v) const; // return the number of edges entering  v.
	  int outDegree(const T& v) const; // return the number of edges leaving  v.
      set<T> getNeighbors(const T& v) const; // return a set containing the neighbors of v.

		// insertion methods
	  void insertEdge(const T& v1, const T& v2); // add the edge (v1,v2) to the graph.
      void insertVertex(const T& v);	// insert v into the graph.

		// deletion methods
	  void eraseEdge(const T& v1, const T& v2);	// erase edge (v1,v2) from the graph
      void eraseVertex(const T& v);	// erase v from the graph
	  void clear();	// remove all the vertices and edges from the graph

        // iterator methods returns corresponding map iterator
	  iterator begin();
	  iterator end();
	  const_iterator begin() const;
	  const_iterator end() const;


        // algorithms
	// find a topological sort of an acyclic graph
	  void topologicalSort(graph<T> &g, list<T>& tlist);
	  
	// check for an acyclic graph
	  bool noCycle(graph<T>& g);

	// visit each vertex
      void visit(graph<T>& g, const T& sVertex, list<T>& outList);

	private:
      typedef map<T,int> vertexMap;

	  vertexMap vtxMap;

	  vector<vertexInfo<T> > vInfo;

	  int numVertices;
	  int numEdges;

	  stack<int> availStack;

	  int getvInfoIndex(const T& v) const; // obtain the index of v in vInfo
};
