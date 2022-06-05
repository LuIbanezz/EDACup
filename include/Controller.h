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

using namespace std;

class Enemy
{
    public:
        Vector3 coordinates;
        Vector3 speed;
        Vector3 rotation;
        Vector3 angularSpeed;

        void assignMessage(vector<float> &message, string &topic)
        {
            if (topic.find("motion") != -1)
            {
                coordinates.x = message[0];
                coordinates.y = message[2];
                coordinates.z = message[1];
                speed.x = message[3];
                speed.y = message[5];
                speed.z = message[4];
                rotation.x = message[6];
                rotation.y = message[8];
                rotation.z = message[7];
                angularSpeed.x = message[9];
                angularSpeed.y = message[11];
                angularSpeed.z = message[10];
            }
        }

};

enum GameStates
{
    preKickOff1, preKickOff2,
    kickOff1, kickOff2,
    preFreeKick1, preFreeKick2,
    freeKick1, freeKick2,
    prePenaltyKick1, prePenaltyKick2,
    penaltyKick1, penaltyKick2,
    pauseGame,
    continueGame,
    removeRobot1, removeRobot2,
    addRobot1,addRobot2
};

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
    void addAwayRobot(Enemy * robot);
    void createTeam1(MQTTClient2 * mqttClient2);
    void createTeam2(MQTTClient2 *mqttClient2);
    void createAwayTeam();
    
    void assignRobotMessage(int robotTeam, int robotIndex, vector<float> &message, string& topic);
    void updateController();
    void updateBall();
    float getTime();
	void start();
    void updateBall(vector<float>& ballInfo);
    
    int team;

    GameStates referee;

    Ball ball;
    
    vector<Robot *> homeTeam;
    vector<Enemy *> awayTeam;
private:
	
    float elapsedTime;
    
};

#endif