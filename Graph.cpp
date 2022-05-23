#include "Graph.h"
#include <raymath.h>

GraphNode::GraphNode(int index)
{
    neighbors.clear();
    for (int i = LEFT; i <= BOTTOM_LEFT; i++)          
    //Se recorren los vecinos arrancando por izquierda, en sentido horario
    {
        WeightedNode neighbor;
        switch (i)
        {
        case TOP_LEFT:
            neighbor.index = GRAPHINDEX(NODE_ROW(index) - 1, NODE_COLUMN(index) - 1);
            if (NODE_COLUMN(index) == 0 || NODE_ROW(index) == 0)
                neighbor.weight = -1;
            else
                neighbor.weight = sqrt(2);
            break;

        case TOP:
            neighbor.index = GRAPHINDEX(NODE_ROW(index) - 1, NODE_COLUMN(index));
            if (NODE_ROW(index) == 0)
                neighbor.weight = -1;
            else
                neighbor.weight = 1;
            break;

        case TOP_RIGHT:
            neighbor.index = GRAPHINDEX(NODE_ROW(index) - 1, NODE_COLUMN(index) + 1);
            if (NODE_COLUMN(index) == GRAPH_WIDTH - 1 || NODE_ROW(index) == 0)
                neighbor.weight = -1;
            else
                neighbor.weight = sqrt(2);
            break;

        case LEFT:
            neighbor.index = GRAPHINDEX(NODE_ROW(index), NODE_COLUMN(index) - 1);
            if (NODE_COLUMN(index) == 0)
                neighbor.weight = -1;
            else
                neighbor.weight = 1;
            break;

        case RIGHT:
            neighbor.index = GRAPHINDEX(NODE_ROW(index), NODE_COLUMN(index) + 1);
            if (NODE_COLUMN(index) == GRAPH_WIDTH - 1) 
                neighbor.weight = -1;
            else
                neighbor.weight = 1;
            break;

        case BOTTOM_LEFT:
            neighbor.index = GRAPHINDEX(NODE_ROW(index) + 1, NODE_COLUMN(index) - 1);
            if (NODE_COLUMN(index) == 0 || (NODE_ROW(index) == GRAPH_LENGTH - 1))
                neighbor.weight = -1;
            else
                neighbor.weight = sqrt(2);
            break;

        case BOTTOM:
            neighbor.index = GRAPHINDEX(NODE_ROW(index) + 1, NODE_COLUMN(index));
            if (NODE_ROW(index) == GRAPH_LENGTH - 1)
                neighbor.weight = -1;
            else
                neighbor.weight = 1;
            break;

        case BOTTOM_RIGHT:
            neighbor.index = GRAPHINDEX(NODE_ROW(index) + 1, NODE_COLUMN(index) + 1);
            if (NODE_COLUMN(index) == GRAPH_WIDTH - 1|| NODE_ROW(index) == GRAPH_LENGTH - 1)
                neighbor.weight = -1;
            else
                neighbor.weight = sqrt(2);
            break;
        }
        neighbors.push_back(neighbor);
    }
}

Graph::Graph()
{
    for (int i = 0; i < GRAPH_TOTAL_SIZE; i++)
    {
        GraphNode newNode(i);
        nodes.push_back(newNode);
    }
};

int Graph::searchPath(int source, int destination)
{
    return 0;
}

float Graph::hScore(int source, int destination)
{
    float x = nodes[source].x - nodes[destination].x;
    float y =  nodes[source].y - nodes[destination].y;
    return sqrt(x*x + y*y);
}