/**
 * @file MyListener.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef _MYLISTENER_H
#define _MYLISTENER_H


#include "MQTTClient2.h"

class MyListener : public MQTTListener
{
public:
    virtual void onMessage(std::string topic, std::vector<char> payload);
};

#endif //_MYLISTENER_H