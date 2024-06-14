#include "graph_structure.h"

// 初始化 AdjacencyMatrix
void initializeMatrix(AdjacencyMatrix* matrix, int num_nodes) {
    matrix->num_nodes = num_nodes;
    int i, j;
    for (i = 0; i < MAX_NODES; i++) {
        for (j = 0; j < MAX_NODES; j++) {
            matrix->absolute_angle[i][j] = -1;
        }
    }
}

// 设置 AdjacencyMatrix 中的边
void setEdge(AdjacencyMatrix* matrix, int i, int j, int absolute_angle) {
    if (i < matrix->num_nodes && j < matrix->num_nodes) {
        matrix->absolute_angle[i][j] = absolute_angle;
    }
}

// 获取 AdjacencyMatrix 中的边
int getEdgeAngle(const AdjacencyMatrix* matrix, int i, int j) {
    if (i < matrix->num_nodes && j < matrix->num_nodes) {
        return matrix->absolute_angle[i][j];
    } else {
        return -1;
    }
}
