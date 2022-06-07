/**
 * @file RightWing.cpp
 * @author Agrippino, Cilfone, Di Sanzo, Hertter, Ibañez
 * @brief Definition for RightBack class methods
 * @version 0.1
 * @date 2022-06-06
 *
 * @copyright Copyright (c) 2022
 *
 */
#include "RightWing.h"
/**
 * @brief Construct a new Right Wing:: Right Wing object. Positions it in a base position
 * 
 * @param robotID 
 * @param client 
 * @param controller 
 */
RightWing::RightWing(string robotID, MQTTClient2 *client, Controller *controller) 
: Robot(robotID, client, controller)
{
    basePosition = {-0.7f * sign, -1.5f * sign, 90.0f * sign};
    outPosition = {-2.0f * sign, -4.0f, 0};
}
/**
 * @brief Updates Right Wing robot
 *
 */
void RightWing::updateRobot()
{
    if (!removed)
    {
        switch (controller->referee)
        {
        case playing:
            playingRightWing();
            break;

        case preKickOff1:
            withBall = false;
            if (team == 2)
            {
                moveRobot(basePosition, PAUSE_SPEED);
                readyToKick = false;
            }
            else
            {
                direction = runUpDestination({controller->homeTeam[4]->coordinates.x,
                                              controller->homeTeam[4]->coordinates.y});
                Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
                readyToKick = moveRobot(newPath, PAUSE_SPEED);
            }
            break;

        case preKickOff2:
            withBall = false;
            if (team == 2)
            {
                direction = runUpDestination({controller->homeTeam[4]->coordinates.x,
                                              controller->homeTeam[4]->coordinates.y});
                Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
                readyToKick = moveRobot(newPath, PAUSE_SPEED);
            }
            else
            {
                moveRobot(basePosition, PAUSE_SPEED);
                readyToKick = false;
            }
            break;

        case kickOff1:
            if (team == 2)
            {
                if (!readyToKick)
                {
                    direction = runUpDestination({controller->homeTeam[4]->coordinates.x,
                                                  controller->homeTeam[4]->coordinates.y});
                    Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
                    readyToKick = moveRobot(newPath, PAUSE_SPEED);
                }

                else
                {
                    if (passToRobot(5))
                    {
                        controller->referee = playing;
                    }
                }
            }
            else
            {
                if (passToRobot(5))
                {
                    controller->referee = playing;
                }
            }
            break;

        case kickOff2:
            if (team == 1)
            {
                if (!readyToKick)
                {
                    direction = runUpDestination({controller->homeTeam[4]->coordinates.x,
                                                  controller->homeTeam[4]->coordinates.y});
                    Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
                    readyToKick = moveRobot(newPath, PAUSE_SPEED);
                }

                else
                {
                    if (passToRobot(5))
                    {
                        controller->referee = playing;
                    }
                }
            }
            else
            {
                if (passToRobot(5))
                {
                    controller->referee = playing;
                }
            }
            break;

        case preFreeKick1:
            withBall = false;
            if (team == 2)
            {

                Vector2 ballPosition = {controller->ball.position.x, controller->ball.position.y};
                Vector2 baseToBall = {basePosition.position.x - ballPosition.x,
                                      basePosition.position.y - ballPosition.y};

                if (Vector2Length(baseToBall) < 0.6f)
                {
                    Vector2 newBasePosition = Vector2Add({basePosition.position.x,
                                                basePosition.position.y},
                                                Vector2Scale(Vector2Normalize(baseToBall), 0.6f));

                    moveRobot({{newBasePosition}, 90.0f}, PAUSE_SPEED);
                }
                else
                {
                    moveRobot(basePosition, PAUSE_SPEED);
                }
                readyToKick = false;
            }
            else
            {
                if (controller->ball.position.y > 0)
                {
                    direction = runUpDestination({goal2.x, goal2.y + 0.4f});
                }
                else
                {
                    direction = runUpDestination({goal2.x, goal2.y - 0.4f});
                }
                Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
                readyToKick = moveRobot(newPath, PAUSE_SPEED);
            }
            break;
        case preFreeKick2:
            withBall = false;
            if (team == 2)
            {
                if (controller->ball.position.y > 0)
                {
                    direction = runUpDestination({goal1.x, goal1.y + 0.4f});
                }
                else
                {
                    direction = runUpDestination({goal1.x, goal1.y - 0.4f});
                }
                Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
                readyToKick = moveRobot(newPath, PAUSE_SPEED);
            }
            else
            {
                Vector2 ballPosition = {controller->ball.position.x, controller->ball.position.y};
                Vector2 baseToBall = {basePosition.position.x - ballPosition.x,
                                      basePosition.position.y - ballPosition.y};

                if (Vector2Length(baseToBall) < 0.6f)
                {
                    Vector2 newBasePosition = Vector2Add({basePosition.position.x,
                                                basePosition.position.y},
                                                Vector2Scale(Vector2Normalize(baseToBall), 0.6f));

                    moveRobot({{newBasePosition}, 90.0f}, PAUSE_SPEED);
                }
                else
                {
                    moveRobot(basePosition, PAUSE_SPEED);
                }
                readyToKick = false;
            }
            break;
        case freeKick1:
            if (team == 1)
            {
                if (readyToKick)
                {
                    if (controller->ball.position.y > 0)
                    {
                        readyToKick = !kickToGoal({goal2.x, goal2.y + 0.4f});
                    }
                    else
                    {
                        readyToKick = !kickToGoal({goal2.x, goal2.y - 0.4f});
                    }
                }
                else
                {
                    if (moveRobot(basePosition, PAUSE_SPEED))
                    {
                        controller->referee = playing;
                    }
                }
            }
            else
            {
                if (!readyToKick)
                {
                    direction = runUpDestination({controller->homeTeam[4]->coordinates.x,
                                                  controller->homeTeam[4]->coordinates.y});
                    Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
                    readyToKick = moveRobot(newPath, PAUSE_SPEED);
                }

                else
                {
                    if (passToRobot(5))
                    {
                        controller->referee = playing;
                    }
                }
            }
            break;
        case freeKick2:
            if (team == 2)
            {
                if (readyToKick)
                {
                    if (controller->ball.position.y > 0)
                    {
                        readyToKick = !kickToGoal({goal1.x, goal1.y + 0.4f});
                    }
                    else
                    {
                        readyToKick = !kickToGoal({goal1.x, goal1.y - 0.4f});
                    }
                }
                else
                {
                    if (moveRobot(basePosition, PAUSE_SPEED))
                    {
                        controller->referee = playing;
                    }
                }
            }
            else
            {
                if (!readyToKick)
                {
                    direction = runUpDestination({controller->homeTeam[4]->coordinates.x,
                                                  controller->homeTeam[4]->coordinates.y});
                    Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
                    readyToKick = moveRobot(newPath, PAUSE_SPEED);
                }

                else
                {
                    if (passToRobot(5))
                    {
                        controller->referee = playing;
                    }
                }
            }
            break;
        case prePenaltyKick1:
            withBall = false;
            if (team == 2)
            {
                moveRobot({-4, +2, 90}, PAUSE_SPEED);
            }
            else
            {
                direction = runUpDestination({goal2.x, goal2.y - 0.4f});
                Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
                readyToKick = moveRobot(newPath, PAUSE_SPEED);
            }
            break;
        case prePenaltyKick2:
            withBall = false;
            if (team == 2)
            {
                direction = runUpDestination({goal1.x, goal1.y - 0.4f});
                Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
                readyToKick = moveRobot(newPath, PAUSE_SPEED);
            }
            else
            {
                moveRobot({4, -2, -90}, PAUSE_SPEED);
            }
            break;
        case penaltyKick1:
            if (team == 1)
            {
                if (readyToKick)
                {
                    readyToKick = !kickToGoal({goal2.x, goal2.y - 0.4f});
                }
            }
            break;
        case penaltyKick2:
            if (team == 2)
            {
                if (readyToKick)
                {
                    readyToKick = !kickToGoal({goal1.x, goal1.y - 0.4f});
                }
            }
            break;

        case continueGame:
            break;
        case pauseGame:
            Vector2 ballPosition = {controller->ball.position.x, controller->ball.position.y};
            Vector2 baseToBall = {basePosition.position.x - ballPosition.x,
                                  basePosition.position.y - ballPosition.y};

            if (Vector2Length(baseToBall) < 0.6f)
            {
                Vector2 newBasePosition = Vector2Add({basePosition.position.x,
                                            basePosition.position.y},
                                            Vector2Scale(Vector2Normalize(baseToBall), 0.6f));

                moveRobot({{newBasePosition}, 90.0f}, PAUSE_SPEED);
            }
            else
            {
                moveRobot(basePosition, PAUSE_SPEED);
            }
            break;
        }
    }
    else if (controller->removedPlayers < RightWingRemoval)
    {
        removed = false;
        returnRobot();
    }
    else if (controller->removedPlayers >= RightWingRemoval)
    {
        removed = true;
        removeRobot();
    }
}

/**
 * @brief
 *
 */
void RightWing::playingRightWing()
{
    if (controller->receiver == robotID[7] - '0')
    {
        if (receivePass())
        {
            auxTime = controller->getTime();
        }
    }
    else if (!withBall)
    {
        moveRobot({sign * 2.0f, sign * 1.0f, sign * 90.0f}, MAX_SPEED);
    }
    else if (withBall)
    {
        Vector2 goalDestination = {goal2.x * sign, (goal2.y - 0.3f) * sign};
        if (controller->getTime() - auxTime < 1.0f)
        {
        }
        else if (controller->getTime() - auxTime < 3.0f)
        {
            startDribble();
            Vector2 goalDestination = calculateGoalDestination();
            Vector2 robotToGoal = {goalDestination.x - coordinates.x,
                                   goalDestination.y - coordinates.y};

            float rotationAngle = 90.0f - Vector2Angle({0, 0}, robotToGoal);
            if (rotationAngle < 0)
            {
                rotationAngle += 360;
            }
            float currentRotation = rotation.z;
            if (rotation.z < 0)
            {
                currentRotation += 360;
            }

            float angleDifference = rotationAngle - currentRotation;
            if (angleDifference < 0)
            {
                angleDifference += 360;
            }
            float sign = 1;
            if (angleDifference > 180)
            {
                sign = -1;
            }

            if (abs(rotationAngle - currentRotation) < 20.0f)
            {
                setSetpoint({coordinates.x, coordinates.y, rotationAngle});
            }
            else
            {
                setSetpoint({coordinates.x, coordinates.y, 
                            (float)(currentRotation + 20.0f * sign)});
            }
        }
        else if (controller->getTime() - auxTime > 3.0f)
        {
            calculateGoalDestination();
            if (kickToGoal(goalDestination))
            {
                withBall = false;
            }
        }
    }
}

Vector2 RightWing::calculateGoalDestination()
{
    Vector2 goalDestination;
    if (coordinates.y > 0)
    {
        goalDestination = {goal2.x * sign, goal2.y * sign + 0.4f};
    }
    else
    {
        goalDestination = {goal2.x * sign, goal2.y * sign - 0.4f};
    }
    return goalDestination;
}