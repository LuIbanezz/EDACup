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

struct Ball 
{
    Vector3 position;
    Vector3 speed;
    Vector3 rotation;
    Vector3 angularSpeed;
};

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
    void updateController();
    void updateBall();
    float getTime();
	void start();
    void updateBall(vector<float>& ballInfo);

    Ball ball;
    
private:
	vector<Robot *> team1;
    float elapsedTime;
    
};

#endif