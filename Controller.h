/**
 * @file Controller.h
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief 
 * @version 0.1
 * @date 2022-05-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include "Robot.h"
#include <vector>

using namespace std;

class Robot;

class Controller
{
public:
    Controller();
    ~Controller();
    void addRobot(Robot* robot);
    
    /**
     * @brief Assigns the received message to the robot
     * 
     * @param robotTeam 
     * @param robotIndex 
     * @param message 
     * @param topic 
     */
    void assignRobotMessage(int robotTeam, int robotIndex, vector<float> &message, string& topic);
    void updateController(vector<float>& ballInfo);
    float getTime();
	void start();
    
private:
	vector<Robot *> team1;
    float elapsedTime;
};

#endif