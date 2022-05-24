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


/**
 * @brief Chequear, completar y verificar
 * 
 * @param source 
 * @param destination 
 * @return int 
 */
void Graph::searchPath(int source, int destination, vector<int> &path)
{
    priority_queue <Node, vector<Node>, Compare> openSet;
    vector <float> gScore (nodes.size(), 10E10f);
    vector <float> fScore (nodes.size(), 10E10f);
    vector <int> cameFrom (nodes.size(), -1);

    gScore[source] = 0;
    fScore[source] = heuristic(source, destination);

    openSet.push({source, gScore[source]});


    while (!openSet.empty())
    {
        Node currentNode = openSet.top();
        int currentIndex = currentNode.index;

        if(currentIndex == destination)
        {
            reconstructPath(cameFrom, currentIndex, path);
        }

        openSet.pop();

        for(auto neighbor : nodes[currentIndex].neighbors)
        {
            float tentativeGScore = gScore[currentIndex] + neighbor.weight;
            if(tentativeGScore < gScore[neighbor.index] )
            {
                cameFrom[neighbor.index] = currentIndex;
                gScore[neighbor.index] = tentativeGScore;
                fScore[neighbor.index] = tentativeGScore +
                                                heuristic(neighbor.index, destination);
            }
        }
        
    }
}

float Graph::heuristic(int source, int destination)
{
    float x = nodes[source].x - nodes[destination].x;
    float y =  nodes[source].y - nodes[destination].y;
    return (float) sqrt(x*x + y*y);
}

void Graph::reconstructPath(vector <int> &cameFrom, int currentIndex, vector<int> &path)
{
    path.clear();
    path.push_back(currentIndex);
    int current = currentIndex;
    while (cameFrom[current] != -1)
    {
        current = cameFrom[current];
        path.push_back(current);
    }

    return reverse(path.begin(), path.end());
}