/**
 * @file GameModel.h
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief 
 * @version 0.1
 * @date 2022-05-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _GAMEMODEL_H
#define _GAMEMODEL_H

#include "Robot.h"
#include <vector>

using namespace std;

class GameModel
{
public:
    GameModel();
    ~GameModel();
    void addRobot(Robot*);

    /**
     * @brief Assigns the received message to the robot
     * 
     * @param robotTeam 
     * @param robotIndex 
     * @param message 
     * @param topic 
     */
    void assignRobotMessage(int robotTeam, int robotIndex, vector<float> &message, string& topic);
    
	
private:
	vector<Robot *> team1;
};

#endif