#include "LeftBack.h"

LeftBack::LeftBack(string robotID, MQTTClient2 *client, Controller *controller) : Robot(robotID, client, controller)
{
    basePosition = {-2.5f * sign, 1.0f * sign, 90.0f * sign};
    outPosition = {-4.0f * sign, -4.0f, 0};
}

void LeftBack::updateRobot()
{
    switch(controller->referee)
    {
        case preKickOff1:
            moveRobot(basePosition, PAUSE_SPEED);
            cout << "speed: " << Vector3Length(speed) << endl;
           break;

        case preKickOff2:
            moveRobot(basePosition, PAUSE_SPEED);
           break;

        case kickOff:
          break;

        case preFreeKick1:
        {
            Vector2 ballPosition = {controller->ball.position.x, controller->ball.position.y};
            Vector2 baseToBall = {basePosition.position.x - ballPosition.x, 
                                    basePosition.position.y - ballPosition.y};

            if(Vector2Length(baseToBall) < 0.6f)
            {
                    Vector2 newBasePosition = Vector2Add({basePosition.position.x, 
                    basePosition.position.y}, Vector2Scale(Vector2Normalize(baseToBall), 0.6f));

                    moveRobot({{newBasePosition}, 90.0f}, PAUSE_SPEED);
            }
            else
            {
                moveRobot(basePosition, PAUSE_SPEED);
            }
        }
            break;

        case preFreeKick2:
        {
            Vector2 ballPosition = {controller->ball.position.x, controller->ball.position.y};
            Vector2 baseToBall = {basePosition.position.x - ballPosition.x, 
                                    basePosition.position.y - ballPosition.y};

            if(Vector2Length(baseToBall) < 0.6f)
            {
                    Vector2 newBasePosition = Vector2Add({basePosition.position.x, 
                    basePosition.position.y}, Vector2Scale(Vector2Normalize(baseToBall), 0.6f));

                    moveRobot({{newBasePosition}, 90.0f}, PAUSE_SPEED);
            }
            else
            {
                moveRobot(basePosition, PAUSE_SPEED);
            }
        }
            break;

        case freeKick1:
            break;
        case freeKick2:
            break;
        case prePenaltyKick1:
            if(team == 2)
            {
                moveRobot({-4, -1.5, 90}, PAUSE_SPEED);
            }
            else
            {
                moveRobot({-4, 1.5, 90}, PAUSE_SPEED);
            }
            break;
        case prePenaltyKick2:
            if(team == 2)
            {
                moveRobot({4, -1.5, -90}, PAUSE_SPEED);
            }
            else
            {
                moveRobot({4, 1.5, -90}, PAUSE_SPEED);
            }
            break;
        case penaltyKick1:
            break;
        case penaltyKick2:
            break;
        case continueGame:
            break;
    }
}