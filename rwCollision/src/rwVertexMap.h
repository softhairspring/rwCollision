#ifndef VERTEXMAP_H
#define VERTEXMAP_H
#include <vector>

//zmapowane wierzcholki ktore sasiaduja ze soba.
//uzywane przy szuakniu ekstremalnych wierzcholków przez Hill-Climbing algo.
//niezaimplementowane do koñca
typedef vector< int > VertexNeighbours;
typedef vector< VertexNeighbours > VertexMap;


#endif