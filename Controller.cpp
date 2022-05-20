/**
 * @file Controller.cpp
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief 
 * @version 0.1
 * @date 2022-05-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "Controller.h"


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
    addRobot(new Robot("robot1.1", mqttClient2, this),1);
    addRobot(new Robot("robot1.2", mqttClient2, this),1);
    addRobot(new Robot("robot1.3", mqttClient2, this),1);
    addRobot(new Robot("robot1.4", mqttClient2, this),1);
    addRobot(new Robot("robot1.5", mqttClient2, this),1);
    addRobot(new Robot("robot1.6", mqttClient2, this),1);
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

    team1[0]->updateRobot();
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