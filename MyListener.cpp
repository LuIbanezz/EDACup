/**
 * @file MyListener.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-05-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include <iostream>

#include "MyListener.h"

using namespace std;

void MyListener::onMessage(string topic, vector<char> payload)
{
    cout << "holis" << endl;
}