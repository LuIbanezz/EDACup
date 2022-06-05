/**
 * @file Controller.cpp
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief Controller class definition. Contains information about the game state.
 * @version 0.1
 * @date 2022-05-14
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "Controller.h"
#include <unordered_map>
#include "aStarAlgorithm.h"
#include "Keeper.h"
#include "RightBack.h"
#include "LeftBack.h"
#include "Mid.h"
#include "LeftWing.h"
#include "RightWing.h"

#define X2GRAPH(X) (((X) + 4.5f) * 10)
#define Y2GRAPH(Y) ((-(Y) + 3f) * 10)

Controller::Controller()
{
    elapsedTime = 0;
}

void Controller::start()
{
    for (auto robot : team1)
    {
        robot->startRobot();
    }
}

Controller::~Controller()
{
    for (auto robot : team1)
    {
        delete robot;
    }
}

/**
 * @brief Adds new robot to the indicated team
 *
 * @param robot
 * @param teamNum
 */
void Controller::addRobot(Robot *robot, int teamNum)
{
    switch (teamNum)
    {
    case 1:
        team1.push_back(robot);
        break;
    case 2:
        team2.push_back(robot);
    default:
        break;
    }
}

/**
 * @brief Creates a new Team of robots
 *
 * @param mqttClient2
 */
void Controller::createTeam1(MQTTClient2 *mqttClient2)
{
    addRobot(new Keeper("robot1.1", mqttClient2, this), 1);
    addRobot(new RightBack("robot1.2", mqttClient2, this), 1);
    addRobot(new LeftBack("robot1.3", mqttClient2, this), 1);
    addRobot(new Mid("robot1.4", mqttClient2, this), 1);
    addRobot(new LeftWing("robot1.5", mqttClient2, this), 1);
    addRobot(new RightWing("robot1.6", mqttClient2, this), 1);
}

/**
 * @brief Creates a new Team of robots
 *
 * @param mqttClient2
 */
void Controller::createTeam2(MQTTClient2 *mqttClient2)
{
    addRobot(new Keeper("robot2.1", mqttClient2, this), 2);
    addRobot(new RightBack("robot2.2", mqttClient2, this), 2);
    addRobot(new LeftBack("robot2.3", mqttClient2, this), 2);
    addRobot(new Mid("robot2.4", mqttClient2, this), 2);
    addRobot(new LeftWing("robot2.5", mqttClient2, this), 2);
    addRobot(new RightWing("robot2.6", mqttClient2, this), 2);
}


/**
 * @brief Assigns the message of the robot received to the corresponding robot.
 *
 * @param robotTeam
 * @param robotIndex
 * @param message
 * @param topic
 */
void Controller::assignRobotMessage(int robotTeam,
                                    int robotIndex, vector<float> &message, string &topic)
{
    if (robotTeam == 1)
    {
        team1[robotIndex]->assignMessage(message, topic);
    }
    else if (robotTeam == 2)
    {
        team2[robotIndex]->assignMessage(message, topic);
    }
}

/**
 * @brief Updates each robot
 *
 */
void Controller::updateController()
{
    elapsedTime += DELTA_TIME;

    for(int i = 0; i < GRAPH_TOTAL_SIZE; i++)
        {
            graph.nodes[i].weight = 1;
        }

    for(auto robot : team1)
    {
        Vector3 coordinates = robot->coordinates;
        int x, y;
        x = int ((coordinates.x + 4.5f) * 10);
        y = int ((-coordinates.y + 3.0f) * 10);

        for(int i = 0; i < GRAPH_TOTAL_SIZE; i++)
        {
            int xCoordinate = NODE_COLUMN(i);
            int yCoordinate = NODE_ROW(i);

            graph.nodes[i].weight += 1E5f * expf(((xCoordinate - x)*(xCoordinate - x)
            + (yCoordinate - y) * (yCoordinate - y)) * (-2));
        }

    }

    for(auto robot : team2)
    {
        Vector3 coordinates = robot->coordinates;
        int x, y;
        x = int ((coordinates.x + 4.5f) * 10);
        y = int ((-coordinates.y + 3.0f) * 10);

        for(int i = 0; i < GRAPH_TOTAL_SIZE; i++)
        {
            int xCoordinate = NODE_COLUMN(i);
            int yCoordinate = NODE_ROW(i);

            graph.nodes[i].weight += 1E5f * expf(((xCoordinate - x)*(xCoordinate - x)
            + (yCoordinate - y) * (yCoordinate - y)) * (-2));
        }
    }

    Vector3 position = team1[0]->coordinates;
    int x = int ((position.x + 4.5f) * 10);
    int y = int ((-position.y + 3.0f) * 10);

    int graphPosition = GRAPHINDEX(x,y);

    if(graphPosition >= 0 && graphPosition < GRAPH_TOTAL_SIZE)
    {
        int destinationIndex = 90 * 30 + 89;
        
       if(graphPosition != destinationIndex)
       {
            std::unordered_map<int, int> came_from;
            std::unordered_map<int, float> cost_so_far;
            a_star_search<int, Graph>(graph, graphPosition, destinationIndex, came_from, cost_so_far);
            vector<int> path = reconstruct_path <int> (graphPosition, destinationIndex , came_from);

            float xDestination = graph.nodes[path[1]].x;
            float yDestination = graph.nodes[path[1]].y;

            team1[0]->moveRobot({{xDestination, yDestination}, 90} ,MAX_SPEED);
       }
    }
    else
    {
        team1[0]->moveRobot({goal1, 90.0f }, MAX_SPEED);
    }


}

/**
 * @brief Saves the message received with the ball information
 *
 * @param ballInfo
 */
void Controller::updateBall(vector<float> &ballInfo)
{
    ball.position.x = ballInfo[0];
    ball.position.y = ballInfo[2];
    ball.position.z = ballInfo[1];

    ball.speed.x = ballInfo[3];
    ball.speed.y = ballInfo[5];
    ball.speed.z = ballInfo[4];

    ball.rotation.x = ballInfo[6];
    ball.rotation.x = ballInfo[8];
    ball.rotation.x = ballInfo[7];

    ball.angularSpeed.x = ballInfo[9];
    ball.angularSpeed.x = ballInfo[11];
    ball.angularSpeed.x = ballInfo[10];
}

/**
 * @brief Getters
 *
 */
float Controller::getTime()
{
    return elapsedTime;
}