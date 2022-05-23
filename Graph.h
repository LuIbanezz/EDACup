#include <raylib.h>
#include <vector>

#define GRAPH_WIDTH 90
#define GRAPH_LENGTH 60
#define GRAPHINDEX(x, y) (int)((y)*GRAPH_WIDTH + (x))
#define GRAPH_TOTAL_SIZE (int)(GRAPH_WIDTH * GRAPH_LENGTH)
#define NODE_COLUMN(i) ((i) % GRAPH_WIDTH)
#define NODE_ROW(i) ((i) / (GRAPH_WIDTH))

enum direction
{
    LEFT,
    TOP_LEFT,
    TOP,
    TOP_RIGHT,
    RIGHT,
    BOTTOM_RIGHT,
    BOTTOM,
    BOTTOM_LEFT
};

using namespace std;

struct WeightedNode
{
    int index;
    float weight;
};

class GraphNode
{
public:
    GraphNode(int index);

    int value;
    bool mark;
    vector<WeightedNode> neighbors;
};

class Graph
{
public:
    Graph();
    vector<GraphNode> nodes;
};