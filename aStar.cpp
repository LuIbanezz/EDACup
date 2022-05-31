#include "Graph.h"

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