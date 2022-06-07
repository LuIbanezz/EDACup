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
#include "Keeper.h"
#include "RightBack.h"
#include "LeftBack.h"
#include "Mid.h"
#include "LeftWing.h"
#include "RightWing.h"
/**
 * @brief Construct a new Controller:: Controller object
 * 
 */
Controller::Controller()
{
    elapsedTime = 0;
    removedPlayers = 0;
    receiverReady = false;
    referee = starting;
}

/**
 * @brief initializes the home team robots
 *
 */
void Controller::start()
{
    for (auto robot : homeTeam)
    {
        robot->startRobot();
    }
}
/**
 * @brief Destroy the Controller:: Controller object
 * 
 */
Controller::~Controller()
{
    for (auto robot : homeTeam)
    {
        delete robot;
    }
    for (auto robot : awayTeam)
    {
        delete robot;
    }
}

/**
 * @brief Adds new robot to the home team
 *
 * @param robot     Robot to add
 */
void Controller::addRobot(Robot *robot)
{
    homeTeam.push_back(robot);
}

/**
 * @brief Adds new robot to the away team
 *
 * @param robot
 */
void Controller::addAwayRobot(Enemy *robot)
{
    awayTeam.push_back(robot);
}

/**
 * @brief Creates a new Team of robots
 *
 * @param mqttClient2
 */
void Controller::createTeam1(MQTTClient2 *mqttClient2)
{
    addRobot(new Keeper("robot1.1", mqttClient2, this));
    addRobot(new LeftBack("robot1.2", mqttClient2, this));
    addRobot(new RightBack("robot1.3", mqttClient2, this));
    addRobot(new Mid("robot1.4", mqttClient2, this));
    addRobot(new LeftWing("robot1.5", mqttClient2, this));
    addRobot(new RightWing("robot1.6", mqttClient2, this));
}

/**
 * @brief Creates a new Team of robots
 *
 * @param mqttClient2
 */
void Controller::createTeam2(MQTTClient2 *mqttClient2)
{
    addRobot(new Keeper("robot2.1", mqttClient2, this));
    addRobot(new LeftBack("robot2.2", mqttClient2, this));
    addRobot(new RightBack("robot2.3", mqttClient2, this));
    addRobot(new Mid("robot2.4", mqttClient2, this));
    addRobot(new LeftWing("robot2.5", mqttClient2, this));
    addRobot(new RightWing("robot2.6", mqttClient2, this));
}

/**
 * @brief Creates a new away team with 6 robots
 *
 */
void Controller::createAwayTeam()
{
    for (int i = 0; i < 6; i++)
    {
        addAwayRobot(new Enemy);
    }
}

/**
 * @brief Assigns the message of the robot received to the corresponding robot.
 *
 * @param robotTeam     Team to which is added the new robot
 * @param robotIndex
 * @param message       Message to assign
 * @param topic         type of data to add
 */
void Controller::assignRobotMessage(int robotTeam,
                                    int robotIndex, vector<float> &message, string &topic)
{
    if (robotTeam == team)
    {
        homeTeam[robotIndex]->assignMessage(message, topic);
    }
    else
    {
        awayTeam[robotIndex]->assignMessage(message, topic);
    }
}

/**
 * @brief Updates each robot
 *
 */
void Controller::updateController()
{
    elapsedTime += DELTA_TIME;
    for (auto robot : homeTeam)
    {
        robot->updateRobot();
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