#ifndef GRAPH_H
#define GRAPH_H

#pragma warning(disable:4786)

#include <iostream>
#include <fstream>

#include <set>				// set class
#include <map>				// ist classmap class
#include <vector>			// vector class
#include <list>			// list class
#include <stack>			// stack class
#include <queue>			// queue class
#include <functional>	// less<T>

#include "except.h"	// exception classes

using namespace std;

class neighbor
{
	public:
                /// index of the destination vertex in the vector vInfo of vertex properties
		int dest;

		// constructor
		neighbor(int d=0): dest(d)
		{}

                /// operators for the neighbor class that compare the destination vertices
		friend bool operator< (const neighbor& lhs, const neighbor& rhs)
		{
			return lhs.dest < rhs.dest;
		}

		friend bool operator== (const neighbor& lhs, const neighbor& rhs)
		{
			return lhs.dest == rhs.dest;
		}
};

/// maintains vertex properties, including its set of neighbors
template <typename T>
class vertexInfo
{
	public:
                /// used by graph algorithms
		enum vertexColor { WHITE, GRAY, BLACK };

                /// iterator pointing at a pair<T,int> object in the vertex map
                typename map<T,int>::iterator vtxMapLoc;

                /// set of adjacent (neighbor) objects for the current vertex
		set<neighbor> edges;

		/// maintains the in-degree of the vertex
		int inDegree;

                /// indicates whether the object currently represents a vertex
		bool occupied;

                /// indicate if a vertex is marked in an algorithm that traverses the vertices of a graph
		vertexColor color;

                /// available to algorithms for storing relevant data values
		int dataValue;

                /// available to graph algorithms; holds parent which is a vertex that has an edge terminating in the current vertex
		int parent;

                /// default constructor
		vertexInfo(): inDegree(0), occupied(true)
		{}

                /// constructor with iterator pointing to the vertex in the map
                vertexInfo(typename map<T,int>::iterator iter):
				vtxMapLoc(iter), inDegree(0), occupied(true)
		{}
};

template <typename T>
class graph
{
   public:
      class const_iterator: public map< T, int >::const_iterator
      {
         public:
            const_iterator()
            {}

            /// converts a map iterator to a graph iterator
            const_iterator(typename map<T,int>::const_iterator i)
            {
                                        *((typename map< T, int >::const_iterator *)this) = i;
			}

            /// return the vertex pointed to by the iterator
            const T& operator* () const
            {
               typename map<T,int>::const_iterator p = *this;

               return (*p).first;
            }
      };

      typedef const_iterator iterator;

          /// constructors
          graph(); /// constructor. initialize numVertices and numEdges to 0
          graph(const graph<T>& g);	/// copy constructor

            /// check graph
      bool empty() const; /// is the graph empty?

          //access methods
          int numberOfVertices() const;	/// return the number of vertices in the graph
          int numberOfEdges() const; /// return the number of edges in the graph
          int inDegree(const T& v) const; /// return the number of edges entering  v.
          int outDegree(const T& v) const; // return the number of edges leaving  v.

      set<T> getNeighbors(const T& v) const; /// return a set containing the neighbors of v.

                /// insertion methods
          void insertEdge(const T& v1, const T& v2); /// add the edge (v1,v2) to the graph.
      void insertVertex(const T& v);	/// insert v into the graph.

                /// deletion methods
          void eraseEdge(const T& v1, const T& v2);	/// erase edge (v1,v2) from the graph
      void eraseVertex(const T& v);	/// erase v from the graph
          void clear();	/// remove all the vertices and edges from the graph

      /// iterator methods returns corresponding map iterator
	  iterator begin();
	  iterator end();
	  const_iterator begin() const;
	  const_iterator end() const;

          /// algorithms
          /// find a topological sort of an acyclic graph
	  void topologicalSort(graph<T> &g, list<T>& tlist);

          /// check for an acyclic graph
      bool noCycle(graph<T>& g);

          /// visit each vertex
      void visit(graph<T>& g, const T& sVertex, list<T> &outList);

	private:
      typedef map<T,int> vertexMap;


            /** store vertex in a map with its name as the key and the index
            of the corresponding vertexInfo object in the vInfo
            vector as the value*/
            vertexMap vtxMap;


            /// list of vertexInfo objects corresponding to the vertices
            vector<vertexInfo<T> > vInfo;

	  int numVertices;
          int numEdges; /// current size (vertices and edges) of the graph

          stack<int> availStack; /// availability stack for storing unused indices in vInfo

       /** check graph
       uses vtxMap to obtain the index of v in vInfo */
	  int getvInfoIndex(const T& v) const;

};

/////////////////////////////////////////////////////////////
/////////////////////// CHECK GRAPH /////////////////////////
/////////////////////////////////////////////////////////////
/****************************************************************************/
template <typename T>
bool graph<T>::empty() const
/****************************************************************************/
{
	return numVertices == 0;
}

/****************************************************************************/
template <typename T>
int graph<T>::getvInfoIndex(const T& v) const
// uses vtxMap to obtain the index of v in vInfo
/****************************************************************************/
{
	// iter used in map lookup
        typename vertexMap::const_iterator iter;

	// index that is returned
	int pos;

	// find the map entry with key v
	iter = vtxMap.find(v);

	// make sure v is in the map
	if (iter == vtxMap.end())
		pos = -1;
	else
		// the index into vInfo is the value of the map entry
		pos = (*iter).second;

	return pos;
}

/////////////////////////////////////////////////////////////
////////////////////// CONSTRUCTORS /////////////////////////
/////////////////////////////////////////////////////////////
/****************************************************************************/
template <typename T>
graph<T>::graph(): numVertices(0), numEdges(0)
// constructor. initialize numVertices and numEdges to 0
/****************************************************************************/
{}

/****************************************************************************/
template <typename T>
graph<T>::graph(const graph<T>& g)
// copy constructor
/****************************************************************************/
{
	*this = g;	// shallow copy g
}


/////////////////////////////////////////////////////////////
/////////////////////// ACCESSORS ///////////////////////////
/////////////////////////////////////////////////////////////
/****************************************************************************/
template <typename T>
int graph<T>::numberOfVertices() const
/****************************************************************************/
{
	return numVertices;
}

/****************************************************************************/
template <typename T>
int graph<T>::numberOfEdges() const
/****************************************************************************/
{
	return numEdges;
}

/****************************************************************************/
template <typename T>
int graph<T>::inDegree(const T& v) const
// return the number of edges entering  v
/****************************************************************************/
{
	// find the vInfo index for v
	int pos = getvInfoIndex(v);

	if (pos != -1)
		// in-degree is stored in vInfo[pos]
		return vInfo[pos].inDegree;
	else
		// throw an exception
		throw graphError("graph inDegree(): vertex not in the graph");
}

/****************************************************************************/
template <typename T>
int graph<T>::outDegree(const T& v) const
// return the number of edges leaving  v
/****************************************************************************/
{
	// find the vInfo index for v
	int pos=getvInfoIndex(v);

	if (pos != -1)
		// out-degree is number of elements in the edge set
		return vInfo[pos].edges.size();
	else
		// throw an exception
		throw graphError("graph outDegree(): vertex not in the graph");
}

/****************************************************************************/
template <typename T>
set<T> graph<T>::getNeighbors(const T& v) const
// return the list of all adjacent vertices
/****************************************************************************/
{
	// set returned
	set<T> adjVertices;

	// obtain the position of v from the map
	int pos = getvInfoIndex(v);

	// if v not in list of vertices, throw an exception
	if (pos == -1)
		throw
			graphError("graph getNeighbors(): vertex not in the graph");

	// construct an alias for the set of edges in vertex pos
	const set<neighbor>& edgeSet = vInfo[pos].edges;

	// use setIter to traverse the edge set
	set<neighbor>::const_iterator setIter;

	// index of vertexInfo object corresponding to an adjacent vertex
	int aPos;

	for (setIter=edgeSet.begin(); setIter != edgeSet.end(); setIter++)
	{	// "(*setIter).dest" is index into vInfo
		aPos = (*setIter).dest;
		// insert vertex data into a set. vInfo[aPos].vtxMapLoc"
		// is a map iterator. dereference it to access the vertex
		adjVertices.insert ((*vInfo[aPos].vtxMapLoc).first);
	}

   return adjVertices;
}


/////////////////////////////////////////////////////////////
/////////////////////// MODIFIERS ///////////////////////////
/////////////////////////////////////////////////////////////
/****************************************************************************/
template <typename T>
void graph<T>::insertEdge(const T& v1, const T& v2)
// add the edge (v1,v2)
/****************************************************************************/
{
	// obtain the vInfo indices
	int pos1=getvInfoIndex(v1), pos2=getvInfoIndex(v2);

	// check for an error
	if (pos1 == -1 || pos2 == -1)
		// if v1 or v2 not in list of vertices, throw an exception
		throw graphError("graph insertEdge(): vertex not in the graph");
	else if (pos1 == pos2)
		throw graphError("graph insertEdge(): self-loops are not allowed");


	// attempt to insert edge (pos2) into the edge set of vertex pos1
	pair<set<neighbor>::iterator, bool> result =
			vInfo[pos1].edges.insert(neighbor(pos2));

	// make sure edge was not already in the set
	if (result.second)
	{
		// increment the number of edges
		numEdges++;
		// the in-degree of v2 is one more
		vInfo[pos2].inDegree++;
	}
}

/****************************************************************************/
template <typename T>
void graph<T>::insertVertex(const T& v)
// insert v into the graph
/****************************************************************************/
{
	int index;

	// attempt to insert v into the map with index 0.
	// if successful, insert an iterator pointing at it
	// into the vertex list at location index. index is obtained
	// from the availability stack or by creating a new entry
	// at the back of the vector. fix the map entry to refer
	// to index and increment numVertices. if the insertion did
	// not take place, the vertex already exists. generate an
	// exception
        pair<typename vertexMap::iterator, bool> result =
                vtxMap.insert(typename vertexMap::value_type(v,0));
	if (result.second)
	{
		// see if there is an entry in vInfo freed by an earlier
		// call to eraseVertex()
		if (!availStack.empty())
		{
			// yes. get its index
			index = availStack.top();
			availStack.pop();
			// call to constructor builds a empty edge set
			vInfo[index] = vertexInfo<T>(result.first);
		}
		else
		{
			// no. we'll have to increase the size of vInfo
			vInfo.push_back(vertexInfo<T>(result.first));
			index = numVertices;
		}

		(*result.first).second = index;
		numVertices++;
	}
	else
		throw graphError("graph insertVertex(): vertex already in the graph");
}

/****************************************************************************/
template <typename T>
void graph<T>::eraseEdge(const T& v1, const T& v2)
// erase edge (v1,v2) from the graph
/****************************************************************************/
{
	// obtain the indices of v1 and v2 in vInfo
	int pos1=getvInfoIndex(v1), pos2=getvInfoIndex(v2);

	// check for an error
	if (pos1 == -1 || pos2 == -1)
		// if v1 or v2 not in list of vertices, throw an exception
		throw graphError("graph eraseEdge(): vertex not in the graph");

	// construct an alias to the edge set of vInfo[pos1]
	set<neighbor>& edgeSet = vInfo[pos1].edges;
	set<neighbor>::iterator setIter;

	// search for pos2 in the edge set
	setIter = edgeSet.find(neighbor(pos2));
	// if pos2 is in the set, erase it; otherwise, output an
	// error message
	if (setIter != edgeSet.end())
	{
		edgeSet.erase(setIter);
		// the in-degree of v2 is one less
		vInfo[pos2].inDegree--;
		numEdges--;
	}
	else
		throw graphError("graph eraseEdge(): edge not in the graph");
}

/****************************************************************************/
template <typename T>
void graph<T>::eraseVertex(const T& v)
/****************************************************************************/
{
	// use to search for and remove v from the map
        typename vertexMap::iterator mIter;
	// pos is index of v in the vertex list
	int pos, j;
	// used in removal of edges to v
	set<neighbor>::iterator sIter;

	// search the map for the key v
	mIter = vtxMap.find(v);
	// if vertex is not present, terminate the erase
	if (mIter == vtxMap.end())
		// if v not in list of vertices, throw an exception
		throw graphError("graph eraseVertex(): vertex not in the graph");

	// obtain the index of v in vInfo
	pos = (*mIter).second;

	// erase vertex from the map and decrement number of vertices
	vtxMap.erase(mIter);
	numVertices--;

	// mark the vertex entry in vInfo as not occupied. the index pos is now
	// available. push it on the availability stack for use later if we
	// insert a vertex
	vInfo[pos].occupied = false;
	availStack.push(pos);

	// cycle through vInfo and remove all edges going to v
	for (j=0; j < vInfo.size(); j++)
		// skip all unoccupied entries, including pos
		if (vInfo[j].occupied)
		{
			// construct an alias for the set vInfo[j].edges
			set<neighbor>& edgeSet = vInfo[j].edges;

			sIter = edgeSet.begin();
			// cycle through the edge set
			while (sIter != edgeSet.end())
				if ((*sIter).dest == pos)
				{
					// found pos. remove it from the set and
					// decrement the edge count
					edgeSet.erase(sIter);
					numEdges--;
					break;
				}
				else
					// took no action. just move forward
					sIter++;
		}

	// decrement numEdges by the number of edges for vertex v
	numEdges -= vInfo[pos].edges.size();

	// the in-degree for all of v's neighbors must be decreased by 1
	set<neighbor>& edgesFromv = vInfo[pos].edges;
	for (sIter=edgesFromv.begin(); sIter != edgesFromv.end(); sIter++)
	{
		j = (*sIter).dest;
		vInfo[j].inDegree--;
	}

	// clear the edge set. construct an alias for vInfo[pos].edges
	// and use erase to clear the set
	set<neighbor>& edgeSet = vInfo[pos].edges;
	edgeSet.erase(edgeSet.begin(), edgeSet.end());
}

/****************************************************************************/
template <typename T>
void graph<T>::clear()
// erase the graph
/****************************************************************************/
{
	// clear the vertex list, vertex map and the
	// availability stack
	vInfo.erase(vInfo.begin(), vInfo.end());
	vtxMap.erase(vtxMap.begin(), vtxMap.end());
	while(!availStack.empty())
		availStack.pop();

	// set the number of vertices and edges to 0
	numVertices = 0;
	numEdges = 0;
}

/////////////////////////////////////////////////////////////
/////////////////////// ITERATORS ///////////////////////////
/////////////////////////////////////////////////////////////
/****************************************************************************/
template <typename T>
typename graph<T>::iterator graph<T>::begin()
/****************************************************************************/
{
  return graph<T>::iterator(vtxMap.begin());
}

/****************************************************************************/
template <typename T>
typename graph<T>::iterator graph<T>::end()
/****************************************************************************/
{
	return graph<T>::iterator(vtxMap.end());
}

/****************************************************************************/
template <typename T>
typename graph<T>::const_iterator graph<T>::begin() const
/****************************************************************************/
{
	return graph<T>::iterator(vtxMap.begin());
}

/****************************************************************************/
template <typename T>
typename graph<T>::const_iterator graph<T>::end() const
/****************************************************************************/
{
  return graph<T>::iterator(vtxMap.end());
}


/////////////////////////////////////////////////////////////
/////////////////////// ALGORITHMS //////////////////////////
/////////////////////////////////////////////////////////////
/****************************************************************************/
template <typename T>
void graph<T>::topologicalSort(graph<T> &g, list<T>& tlist)
   // find a topological sort of an acyclic graph
/****************************************************************************/
{
	int i;

	// clear the list that will contain the sort
	tlist.erase(tlist.begin(), tlist.end());

	for (i=0;i < g.vInfo.size(); i++)
		if (g.vInfo[i].occupied)
			g.vInfo[i].color = vertexInfo<T>::WHITE;

	// cycle through the vertices, calling visit() for each
	// WHITE vertex. check for a cycle
	try
	{
		for (i=0;i < g.vInfo.size(); i++)
			if (g.vInfo[i].occupied && g.vInfo[i].color == vertexInfo<T>::WHITE)
				visit(g, (*(g.vInfo[i].vtxMapLoc)).first, tlist);
	}

	catch(graphError&)
	{
	//////////////////////////////////////////////////
	////////  ADD AGENT CODE TO HANDLE CYCLE  ////////
	/////////  ADD GUI CODE TO HANDLE CYCLE  /////////
	//////////////////////////////////////////////////
        //	cout << "graph topologicalSort(): graph has a cycle" << "\n";
	}
}

/****************************************************************************/
template <typename T>
bool graph<T>::noCycle(graph<T>& g)
/****************************************************************************/
{
	int i;
	// use for calls to visit()
	list<T> aList;

	// initialize all vertices to WHITE
	for (i=0;i < g.vInfo.size(); i++)
		if (g.vInfo[i].occupied)
			g.vInfo[i].color = vertexInfo<T>::WHITE;

	// scan vInfo, calling visit() for each WHITE vertex.
	// catch a graphError exception in a call to visit()
	try
	{
		for (i=0;i < g.vInfo.size(); i++)
			if (g.vInfo[i].occupied && g.vInfo[i].color ==
												vertexInfo<T>::WHITE)
					visit(g,(*(g.vInfo[i].vtxMapLoc)).first, aList);
	}

	catch (const graphError&)
	{
		return false;
	}

	return true;
}


/****************************************************************************/
template <typename T>
void graph<T>::visit(graph<T>& g, const T& sVertex, list<T> &outList)
/****************************************************************************/
{
	// indices for vertex positions in vInfo
	int pos_sVertex, pos_neighbor;

	// iterator to scan the adjacency set of a vertex
	set<neighbor>::iterator adj;

	// alias to simplify access to the vector vInfo
	vector<vertexInfo<T> >& vlist = g.vInfo;

	// fetch the index for sVertex in vInfo; throw an exception
	// if the starting vertex is not in the graph
	pos_sVertex = g.getvInfoIndex(sVertex);

	if (pos_sVertex == -1)
		throw graphError("graph visit(): vertex not in the graph");

	// color vertex GRAY to note its discovery
	vlist[pos_sVertex].color = vertexInfo<T>::GRAY;

	// create an alias for the adjacency set of sVertex
	set<neighbor>& edgeSet = vlist[pos_sVertex].edges;

	// sequence through the adjacency set and look for vertices
	// that are not yet discovered (colored WHITE). recursively call
	// visit() for each such vertex. if a vertex in the adjacency
	// set is GRAY, the vertex was discovered during a previous
	// call and there is a cycle that begins and ends at the vertex

	for (adj = edgeSet.begin(); adj != edgeSet.end(); adj++)
	{
		pos_neighbor = (*adj).dest;
		if (vlist[pos_neighbor].color == vertexInfo<T>::WHITE)
			visit(g,(*(g.vInfo[pos_neighbor].vtxMapLoc)).first, outList);
		else if (vlist[pos_neighbor].color == vertexInfo<T>::GRAY)
			throw graphError("graph visit(): graph has a cycle");
	}

	// finished with vertex sVertex. make it BLACK and add it to
	// the front of outList
	vlist[pos_sVertex].color = vertexInfo<T>::BLACK;
	outList.push_front((*(g.vInfo[pos_sVertex].vtxMapLoc)).first);
}


#endif	// GRAPH_H
