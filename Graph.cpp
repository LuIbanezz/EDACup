#include "Graph.h"
#include <raymath.h>

GraphNode::GraphNode(int index)
{
    neighbors.clear();
    for (int i = BOTTOM_RIGHT; i >= TOP_LEFT; i--)          
    //Se va restando por el forward_list, para que quede *arriba a la izquierda* primero y *abajo a la derecha* ultimo
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
            if (NODE_COLUMN(index) == GRAPH_WIDTH || NODE_ROW(index) == 0)
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
            if (NODE_COLUMN(index) == GRAPH_WIDTH) 
                neighbor.weight = -1;
            else
                neighbor.weight = 1;
            break;

        case BOTTOM_LEFT:
            neighbor.index = GRAPHINDEX(NODE_ROW(index) + 1, NODE_COLUMN(index) - 1);
            if (NODE_COLUMN(index) == 0 || (NODE_ROW(index) == GRAPH_LENGTH))
                neighbor.weight = -1;
            else
                neighbor.weight = sqrt(2);
            break;

        case BOTTOM:
            neighbor.index = GRAPHINDEX(NODE_ROW(index) + 1, NODE_COLUMN(index));
            if (NODE_ROW(index) == GRAPH_LENGTH)
                neighbor.weight = -1;
            else
                neighbor.weight = 1;
            break;

        case BOTTOM_RIGHT:
            neighbor.index = GRAPHINDEX(NODE_ROW(index) + 1, NODE_COLUMN(index) + 1);
            if (NODE_COLUMN(index) == GRAPH_WIDTH || NODE_ROW(index) == GRAPH_LENGTH)
                neighbor.weight = -1;
            else
                neighbor.weight = sqrt(2);
            break;
        }
        neighbors.push_front(neighbor);
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