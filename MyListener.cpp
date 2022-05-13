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

void onMessage(std::string topic, std::vector<char> payload)
{
    std::cout << "holis" << std::endl;
}