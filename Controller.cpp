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

void Controller::updateController(vector<float>& ballInfo)
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



    team1[0]->updateRobot(ballInfo);
}

float Controller::getTime()
{
    return elapsedTime;
}