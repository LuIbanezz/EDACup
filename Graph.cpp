/**
 * @file Graph.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "Graph.h"
#include <raymath.h>
#include <vector>
#include <queue>
#include <algorithm>

struct Node
{
    int index;
    float fScore;
};

class Compare
{
public:
    bool operator() (Node node1, Node node2)
    {
        if(node1.fScore <  node2.fScore)
            return true;
        else
            return false;
    }
};

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
                neighbor.weight = (float)sqrt(2);
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
                neighbor.weight = (float)sqrt(2);
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
                neighbor.weight = (float)sqrt(2);
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
                neighbor.weight = (float)sqrt(2);
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

vector<int> Graph::neighbors(int current)
{
    vector<int> vec;
    for (auto neighbor : nodes[current].neighbors)
    {
        vec.push_back(neighbor.index);
    }
    return vec;
}

float Graph::cost(int current, int next)
{
    return (nodes[current].neighbors[next].weight * (nodes[next].weight - nodes[current].weight));
}

float Graph::heuristic(int current, int next)
{
    float x = nodes[current].x - nodes[next].x;
    float y = nodes[current].y - nodes[next].y;

    return sqrt(x*x + y*y);
}