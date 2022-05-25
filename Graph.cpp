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
    mark = false;

    int column = NODE_COLUMN(index);
    int row = NODE_ROW(index);

    this->index = index;

    x = - ((GRAPH_WIDTH / 2 - column) / 10.0f - 0.05f);
    y = ((GRAPH_LENGTH / 2 - row) / 10.0f - 0.05f);

    weight = 1.0f;

    for (int i = LEFT; i <= BOTTOM_LEFT; i++)          
    //Se recorren los vecinos arrancando por izquierda, en sentido horario
    {
        WeightedNode neighbor;
        bool NotANeighboor=false;
        switch (i)
        {
        case TOP_LEFT:
            neighbor.index = GRAPHINDEX(column - 1, row - 1);
            if (column == 0 || row == 0)
                NotANeighboor = true;
            else
                neighbor.weight = sqrt(2);
            break;

        case TOP:
            neighbor.index = GRAPHINDEX(column, row - 1);
            if (row == 0)
                NotANeighboor = true;
            else
                neighbor.weight = 1;
            break;

        case TOP_RIGHT:
            neighbor.index = GRAPHINDEX(column + 1, row - 1);
            if (column == GRAPH_WIDTH - 1 || row == 0)
                NotANeighboor=true;
            else
                neighbor.weight = sqrt(2);
            break;

        case LEFT:
            neighbor.index = GRAPHINDEX(column - 1, row);
            if (column == 0)
                NotANeighboor=true;
            else
                neighbor.weight = 1;
            break;

        case RIGHT:
            neighbor.index = GRAPHINDEX(column + 1, row);
            if (column == GRAPH_WIDTH - 1) 
                NotANeighboor=true;
            else
                neighbor.weight = 1;
            break;

        case BOTTOM_LEFT:
            neighbor.index = GRAPHINDEX(column - 1, row + 1);
            if (column == 0 || (row == GRAPH_LENGTH - 1))
                NotANeighboor=true;
            else
                neighbor.weight = sqrt(2);
            break;

        case BOTTOM:
            neighbor.index = GRAPHINDEX(column, row + 1);
            if (NODE_ROW(index) == GRAPH_LENGTH - 1)
                NotANeighboor=true;
            else
                neighbor.weight = 1;
            break;

        case BOTTOM_RIGHT:
            neighbor.index = GRAPHINDEX(column + 1, row + 1);
            if (column == GRAPH_WIDTH - 1 || row == GRAPH_LENGTH - 1)
                NotANeighboor=true;
            else
                neighbor.weight = sqrt(2);
            break;
        }
        if (!NotANeighboor)
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
    return (nodes[next].weight - nodes[current].weight);
}

float Graph::heuristic(int current, int next)
{
    float x = nodes[current].x - nodes[next].x;
    float y = nodes[current].y - nodes[next].y;

    return sqrt(x*x + y*y);
}