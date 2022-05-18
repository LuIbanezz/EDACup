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

//TODO: completar constructor de Controller
Controller::Controller()
{
    elapsedTime = 0;
}

Controller::~Controller()
{
    for(auto robot : team1)
    {
        delete robot;
    }
}

void Controller::addRobot(Robot *robot)
{
    team1.push_back(robot);
}

void Controller::assignRobotMessage(int robotTeam,
                                    int robotIndex, vector<float> &message, string& topic)
{
    if(robotTeam == 1)
    {
        team1[robotIndex]->assignMessage(message, topic);
    }
    else if(robotTeam == 2)
    {
        //team2[robotIndex]->assignMessage(message, topic);
    }
}

void Controller::updateController()
{
    elapsedTime += DELTATIME;
    //acá seteamos los "flags"


    //acá actualizamos
    // for(auto robot : team1)
    // {
    //     robot->updateRobot();
    // }

    /*
    Position
    velocidad
    rotacion
    vel angular
    */

    

    team1[0]->updateRobot();
}

float Controller::getTime()
{
    return elapsedTime;
}

void Controller::start()
{
    for(auto robot : team1)
    {
        robot->startRobot();
    }
}

void Controller::updateBall(vector<float>& ballInfo)
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