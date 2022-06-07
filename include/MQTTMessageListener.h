/**
 * @file MQTTMessageListener.h
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief MQTTListener sub-class declaration
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
#include "Controller.h"
#include <vector>

class MQTTMessageListener : public MQTTListener
{
public:
    MQTTMessageListener(Controller *controller);
    virtual void onMessage(std::string topic, std::vector<char> payload);

private:
    Controller *controller;
    uint8_t decodeByte(vector<char> vecChar);
    vector<float> decode(vector<char> vecChar);
    void assign(vector<float> &message, Robot *robot, string topic);
};

#endif //_MYLISTENER_H