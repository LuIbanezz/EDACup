/**
 * @file MyListener.h
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief 
 * @version 0.1
 * @date 2022-05-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _MYLISTENER_H
#define _MYLISTENER_H

#include "Robot.h"
#include "MQTTClient2.h"
#include "GameModel.h"
#include <vector>

class MyListener : public MQTTListener
{
public:
    MyListener(GameModel* model);
    virtual void onMessage(std::string topic, std::vector<char> payload);
    
private:
    GameModel* model;

    vector<float> decode(vector<char> vecChar);
    void assign(vector<float>& message, Robot* robot, string topic);
    
};

#endif //_MYLISTENER_H