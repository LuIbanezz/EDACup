/**
 * @file Controller.h
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief Controller class declaration
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
#include "Graph.h"

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
    void addRobot(Robot* robot,int teamNum);
    void createTeam1(MQTTClient2 * mqttClient2);
    
    void assignRobotMessage(int robotTeam, int robotIndex, vector<float> &message, string& topic);
    void updateController();
    void updateBall();
    float getTime();
	void start();
    void updateBall(vector<float>& ballInfo);
    
    Ball ball;
    Graph graph;
    
private:
	vector<Robot *> team1;
    vector<Robot *> team2;
    float elapsedTime;
    
};

#endif