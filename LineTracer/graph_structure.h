#ifndef GRAPH_STRUCTURE_H
#define GRAPH_STRUCTURE_H

// max node number
#define MAX_NODES 10

//typedef struct{
//    int id;
//    int edge_number;
//    int edge_absolute_angle[8];
//} Point;

//typedef struct {
//    Point from_point;
//    Point to_point;
//    int absolute_angle;
//} Edge;

typedef struct {
    int matrix[MAX_NODES][MAX_NODES];
    int num_nodes;
} Graph;

typedef struct {
    int absolute_angle[MAX_NODES][MAX_NODES];
    int num_nodes;
} AdjacencyMatrix;

void initializeMatrix(AdjacencyMatrix* matrix, int num_nodes);
void setEdge(AdjacencyMatrix* matrix, int i, int j, int absolute_angle);
int getEdgeAngle(const AdjacencyMatrix* matrix, int i, int j);

#endif // GRAPH_STRUCTURE_H
