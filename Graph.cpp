#include "Graph.h"
#include <raymath.h>
#include <queue>

class Compare
{
public:
    bool operator() (GraphNode node1, GraphNode node2)
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
        bool NotANeighboor=false;
        switch (i)
        {
        case TOP_LEFT:
            neighbor.index = GRAPHINDEX(NODE_ROW(index) - 1, NODE_COLUMN(index) - 1);
            if (NODE_COLUMN(index) == 0 || NODE_ROW(index) == 0)
                NotANeighboor=true;
            else
                neighbor.weight = sqrt(2);
            break;

        case TOP:
            neighbor.index = GRAPHINDEX(NODE_ROW(index) - 1, NODE_COLUMN(index));
            if (NODE_ROW(index) == 0)
                NotANeighboor=true;
            else
                neighbor.weight = 1;
            break;

        case TOP_RIGHT:
            neighbor.index = GRAPHINDEX(NODE_ROW(index) - 1, NODE_COLUMN(index) + 1);
            if (NODE_COLUMN(index) == GRAPH_WIDTH - 1 || NODE_ROW(index) == 0)
                NotANeighboor=true;
            else
                neighbor.weight = sqrt(2);
            break;

        case LEFT:
            neighbor.index = GRAPHINDEX(NODE_ROW(index), NODE_COLUMN(index) - 1);
            if (NODE_COLUMN(index) == 0)
                NotANeighboor=true;
            else
                neighbor.weight = 1;
            break;

        case RIGHT:
            neighbor.index = GRAPHINDEX(NODE_ROW(index), NODE_COLUMN(index) + 1);
            if (NODE_COLUMN(index) == GRAPH_WIDTH - 1) 
                NotANeighboor=true;
            else
                neighbor.weight = 1;
            break;

        case BOTTOM_LEFT:
            neighbor.index = GRAPHINDEX(NODE_ROW(index) + 1, NODE_COLUMN(index) - 1);
            if (NODE_COLUMN(index) == 0 || (NODE_ROW(index) == GRAPH_LENGTH - 1))
                NotANeighboor=true;
            else
                neighbor.weight = sqrt(2);
            break;

        case BOTTOM:
            neighbor.index = GRAPHINDEX(NODE_ROW(index) + 1, NODE_COLUMN(index));
            if (NODE_ROW(index) == GRAPH_LENGTH - 1)
                NotANeighboor=true;
            else
                neighbor.weight = 1;
            break;

        case BOTTOM_RIGHT:
            neighbor.index = GRAPHINDEX(NODE_ROW(index) + 1, NODE_COLUMN(index) + 1);
            if (NODE_COLUMN(index) == GRAPH_WIDTH - 1|| NODE_ROW(index) == GRAPH_LENGTH - 1)
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

/**
 * @brief Chequear, completar y verificar
 * 
 * @param source 
 * @param destination 
 * @return int 
 */
int Graph::searchPath(int source, int destination)
{
    priority_queue <GraphNode, vector<GraphNode>, Compare> openSet;

    while (!openSet.empty())
    {
        GraphNode currentNode = openSet.top();

        if(currentNode.index == destination)
        {
            return;
        }
        else
        {
            openSet.pop();
            for(auto neighbor : currentNode.neighbors)
            {

                float tentativeGScore = currentNode.gScore + neighbor.weight;
                if(tentativeGScore < nodes[neighbor.index].gScore )
                {
                    nodes[neighbor.index].cameFromIndex = currentNode.index;
                    nodes[neighbor.index].gScore = tentativeGScore;
                    nodes[neighbor.index].fScore = tentativeGScore +
                                                    hScore(neighbor.index, destination);
                    if(nodes[neighbor.index] not in)
                    {
                        openSet.push(nodes[neighbor.index]);
                    }
                }
            }
        }
    }

    return 0;
}

float Graph::hScore(int source, int destination)
{
    float x = nodes[source].x - nodes[destination].x;
    float y =  nodes[source].y - nodes[destination].y;
    return sqrt(x*x + y*y);
}

float reconstructPath()
{

}