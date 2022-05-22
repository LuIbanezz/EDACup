#include <raylib.h>
#include <forward_list>
#include <vector>

#define GRAPH_WIDTH 20
#define GRAPH_LENGTH 50
#define GRAPHINDEX(x,y) (int)((y)*GRAPH_WIDTH+(x))
#define GRAPH_TOTAL_SIZE (int)(GRAPH_WIDTH*GRAPH_LENGTH)
#define NODE_COLUMN(i) ((i)%GRAPH_WIDTH)
#define NODE_ROW(i) ((i)/(GRAPH_WIDTH))


enum direction
{
    TOP_LEFT,
    TOP,
    TOP_RIGHT,
    LEFT,
    RIGHT,
    BOTTOM_LEFT,
    BOTTOM,
    BOTTOM_RIGHT
};

using namespace std;

struct WeightedNode{
    int index;
    float weight;
};

class GraphNode
{
public:
GraphNode(int index);

int value;
bool mark;
forward_list<WeightedNode> neighbors;
};

class Graph
{
public:
    Graph();
    vector<GraphNode> nodes;
};