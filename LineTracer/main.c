#include "msp.h"
#include "Clock.h"
#include "basic_function.h"
#include "motor_move.h"
#include "graph_structure.h"
#include <stdlib.h>

void dfs(Graph* graph, int v, int* path, int* path_index) {
    int u;
    for (u = 0; u < graph->num_nodes; u++) {
        while (graph->matrix[v][u] > 0) {
            graph->matrix[v][u]--;
            graph->matrix[u][v]--;
            dfs(graph, u, path, path_index);
        }
    }
    path[(*path_index)++] = v;
}

void set_matrix(AdjacencyMatrix* matrix){
    setEdge(matrix, 0, 1, 180);
    setEdge(matrix, 1, 0, 0);
    setEdge(matrix, 0, 3, 225);
    setEdge(matrix, 3, 0, 45);
    setEdge(matrix, 0, 5, 270);
    setEdge(matrix, 5, 0, 90);
    setEdge(matrix, 0, 7, 315);
    setEdge(matrix, 7, 0, 135);

    setEdge(matrix, 1, 2, 225);
    setEdge(matrix, 2, 1, 45);
    setEdge(matrix, 1, 4, 270);
    setEdge(matrix, 4, 1, 90);
    setEdge(matrix, 1, 6, 315);
    setEdge(matrix, 6, 1, 135);

    setEdge(matrix, 2, 3, 270);
    setEdge(matrix, 3, 2, 90);
    setEdge(matrix, 2, 5, 315);
    setEdge(matrix, 5, 2, 135);
    setEdge(matrix, 2, 7, 0);
    setEdge(matrix, 7, 2, 180);

    setEdge(matrix, 3, 4, 315);
    setEdge(matrix, 4, 3, 135);
    setEdge(matrix, 3, 6, 0);
    setEdge(matrix, 6, 3, 180);

    setEdge(matrix, 4, 5, 0);
    setEdge(matrix, 5, 4, 180);
    setEdge(matrix, 4, 7, 45);
    setEdge(matrix, 7, 4, 225);

    setEdge(matrix, 5, 6, 45);
    setEdge(matrix  , 6, 5, 225);

    setEdge(matrix, 6, 7, 90);
    setEdge(matrix, 7, 6, 270);
}

/**
 * main.c
 */
void main(void){
    Clock_Init48MHz();
    SysTick_Init();
    Motor_Init();
    Sensor_Init();
    Switch_Init();
    Led_Init();


    // test rotate time
//    start_from_start_line();
//    right_rotate_time(3000);


    // Record the current car angle
    int current_angle = 270;

    // setup AdjacencyMatrix
    int point_number = 8;
    AdjacencyMatrix matrix;
    initializeMatrix(&matrix, point_number);
    set_matrix(&matrix);

    Graph graph = {
        .matrix = {
            {0, 1, 0, 1, 0, 1, 0, 1},
            {1, 0, 1, 0, 1, 0, 1, 0},
            {0, 1, 0, 1, 0, 1, 0, 1},
            {1, 0, 1, 0, 1, 0, 1, 0},
            {0, 1, 0, 1, 0, 1, 0, 1},
            {1, 0, 1, 0, 1, 0, 1, 0},
            {0, 1, 0, 1, 0, 1, 0, 1},
            {1, 0, 1, 0, 1, 0, 1, 0},
        },
        .num_nodes = 8
    };

    int path[MAX_NODES * MAX_NODES];
    int path_index = 0;
    dfs(&graph, 0, path, &path_index);

    // ´òÓ¡Â·¾¶
    // [CORTEX_M4_0] 0 1 2 3 0 5 2 7 4 1 6 3 4 5 6 7 0
//    int i;
//    for (i = path_index - 1; i >= 0; i--) {
//        printf("%d ", path[i]);
//    }
//    printf("\n");

    start_from_start_line();
    int i = path_index - 1;
    int edge_angle;
    while(i > 0) {
        edge_angle = getEdgeAngle(&matrix, path[i], path[i-1]);
        i--;
        rotate_with_given_angle(current_angle, edge_angle);
        current_angle = edge_angle;
        move_straight_to_point();
    }
    // move to end line
    rotate_with_given_angle(current_angle, 90);
    start_to_end_line();



}
