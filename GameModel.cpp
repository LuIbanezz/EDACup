/**
 * @file GameModel.cpp
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief 
 * @version 0.1
 * @date 2022-05-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "GameModel.h"

//TODO: completar constructor de GameModel
GameModel::GameModel()
{
    
}

GameModel::~GameModel()
{
    for(auto robot : team1)
    {
        delete robot;
    }
}

void GameModel::addRobot(Robot *robot)
{
    team1.push_back(robot);
}

void GameModel::assignRobotMessage(int robotTeam,
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
        //team2[robotIndex]->assignMessage(message, topic);
}