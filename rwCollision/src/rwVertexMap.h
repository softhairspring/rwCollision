#ifndef VERTEXMAP_H
#define VERTEXMAP_H
#include <vector>

//zmapowane wierzcholki ktore sasiaduja ze soba.
//uzywane przy szuakniu ekstremalnych wierzcholk�w przez Hill-Climbing algo.
//niezaimplementowane do ko�ca
typedef vector< int > VertexNeighbours;
typedef vector< VertexNeighbours > VertexMap;


#endif