#include "RightWing.h"

RightWing::RightWing(string robotID, MQTTClient2 *client, Controller *controller) : Robot(robotID, client, controller)
{
    basePosition = {-0.7f * sign, -1.5f * sign, 90.0f * sign};
    outPosition = {-2.0f * sign, -4.0f, 0};
}

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
            }
            break;

        case kickOff:
            if (team == 2)
            {
                moveRobot(basePosition, PAUSE_SPEED);
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
            }
            else
            {
                direction = runUpDestination({goal2.x, goal2.y - 0.4f});
                Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
                readyToKick = moveRobot(newPath, PAUSE_SPEED);
            }
            break;
        case preFreeKick2:
            withBall = false;
            if (team == 2)
            {
                direction = runUpDestination({goal1.x, goal1.y - 0.4f});
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
            }
            break;
        case freeKick1:
            if (team == 1)
            {
                if (kickToGoal({goal2.x, goal2.y - 0.4f}))
                {
                    controller->referee = playing;
                }
            }
            break;
        case freeKick2:
            if (team == 2)
            {
                if (kickToGoal({goal1.x, goal1.y - 0.4f}))
                {
                    controller->referee = playing;
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
                /* if (coordinates.y>0)
                */
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
                kickToGoal({goal2.x, goal2.y - 0.4f});
            }
            break;
        case penaltyKick2:
            if (team == 2)
            {
                kickToGoal({goal1.x, goal1.y - 0.4f});
            }
            break;

        case continueGame:
            break;
        case pauseGame:
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
        moveRobot({2.0f, 1.0f, 90.0f}, MAX_SPEED);
    }
    else if (withBall)
    {
        Vector2 goalDestination = {goal2.x * sign, (goal2.y - 0.3f) * sign};
        if (controller->getTime() - auxTime < 1.0f)
        {
            stopDribble();
        }
        else if (controller->getTime() - auxTime < 3.0f)
        {
            // startDribble();
            // Vector2 ballPosition = {controller->ball.position.x, controller->ball.position.y};
            // Vector2 ballToRobot = {coordinates.x - ballPosition.x, coordinates.y - ballPosition.y};
            // Vector2 normalizedBallToRobot = Vector2Normalize(ballToRobot);

            // Vector2 awayFromBall = Vector2Add({coordinates.x, coordinates.y},
            //                         Vector2Scale(normalizedBallToRobot, 0.5f));

            // float rotationAngle = 90.0f - Vector2Angle(ballToRobot, {0, 0});

            // moveRobot({awayFromBall, rotationAngle}, PAUSE_SPEED);
            startDribble();
            Vector2 goalDestination = {goal2.x * sign, (goal2.y - 0.3f) * sign};
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
            if (abs(rotationAngle - currentRotation) < 20.0f)
            {
                setSetpoint({coordinates.x, coordinates.y, rotationAngle});
            }
            else
            {
                setSetpoint({coordinates.x, coordinates.y, (float)(currentRotation+20.0f)});
            }
        }
        else if (controller->getTime() - auxTime > 3.0f)
        {
            // stopDribble();
            // Vector2 goalDestination = {goal2.x * sign, (goal2.y - 0.3f) * sign};
            // Vector2 robotToGoal = {goalDestination.x - coordinates.x,
            //                        goalDestination.y - coordinates.y};

            // float rotationAngle = 90.0f - Vector2Angle({0, 0}, robotToGoal);
            // setSetpoint({coordinates.x, coordinates.y, rotationAngle});

            //     if (!readyToKick)
            //     {
            //         controller->receiver = 6;
            //         direction = runUpDestination(goalDestination);
            //         Setpoint newPath = getPath(BALL_RADIUS + ROBOT_RADIUS + 0.1f);
            //         readyToKick = moveRobot(newPath, PAUSE_SPEED);
            //     }

            //     else
            //     {
            //         if(kickToGoal(goalDestination))
            //         {
            //             withBall = false;
            //             stopDribble();
            //         }
            //     }
            // }
            if (kickToGoal(goalDestination))
            {
                stopDribble();
                withBall = false;
            }
        }
    }
}