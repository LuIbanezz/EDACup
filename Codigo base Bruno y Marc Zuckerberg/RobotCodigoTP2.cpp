/**
 * @file Robot.cpp
 * @authors Bruno Di Sanzo, Tomás Castro
 *
 * @brief Clase para el manejo de los controles del robot (cuerpo)
 *
 * @copyright Copyright (c) 2022 ~ EDA ~ ITBA
 *
 */

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "Robot.h"
#include <vector>
#include <string>
using namespace std;

/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define ROBOT_ID (string) "robot1"
#define SENSIBILIDAD 0.1f // cuán sensible es la deteccion de movimientos distintos

/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/

static float vecToFloat(std::vector<char> &);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
Robot::Robot(string clientId, string hostname, int port, string user, string password)
    : cliente(clientId), direccion(0, 0, 0)
{
  nitro = false;
  temperaturaLimiteAlcanzada = false;

  cliente.connect(hostname, port, user, password);
  for (char c = '1'; c < '1' + CANT_MOTORES; c++)
  {
    cliente.subscribe(ROBOT_ID + "/motor" + c + "/voltage/set");
    cliente.subscribe(ROBOT_ID + "/motor" + c + "/current/set");
    cliente.subscribe(ROBOT_ID + "/motor" + c + "/voltage");
    cliente.subscribe(ROBOT_ID + "/motor" + c + "/current");
    cliente.subscribe(ROBOT_ID + "/motor" + c + "/rpm");
    cliente.subscribe(ROBOT_ID + "/motor" + c + "/temperature");
  }
  cliente.subscribe(ROBOT_ID + "/display/leftEye/set");
  cliente.subscribe(ROBOT_ID + "/display/rightEye/set");
  
}

Robot::~Robot()
{
  updateMotorTension(0, 0, 0, false);
  for (char c = '1'; c < '1' + CANT_MOTORES; c++)
  {
    cliente.unsubscribe(ROBOT_ID + "/motor" + c + "/voltage/set");
    cliente.unsubscribe(ROBOT_ID + "/motor" + c + "/current/set");
    cliente.unsubscribe(ROBOT_ID + "/motor" + c + "/voltage");
    cliente.unsubscribe(ROBOT_ID + "/motor" + c + "/current");
    cliente.unsubscribe(ROBOT_ID + "/motor" + c + "/rpm");
    cliente.unsubscribe(ROBOT_ID + "/motor" + c + "/temperature");
  }
  cliente.unsubscribe(ROBOT_ID + "/display/leftEye/set");
  cliente.unsubscribe(ROBOT_ID + "/display/rightEye/set");

  cliente.disconnect();
}

/**
 * @brief Recibe informcación proveniente del Robot y mueve los motores segun la dirección
 * que se recibe, en forma vectorial.
 *
 * @param x dirección en x
 * @param y dirección en y
 * @param r rotación // > 0 rotacion antihoraria / < 0 rotacion horaria / = 0 no rotacion
 */
void Robot::actualizar(float x, float y, float r, bool n)
{
  saveMessages();
  updateMotorTension(x, y, r, n);
}

/**
 * @brief Setea los colores de los LEDS
 *
 * @param str string con el TOPIC
 * @param colores color a setear
 * 
 */
void Robot::publishColor(std::string str, vector<char> colores)
{
  cliente.publish(str, colores);
}

float Robot::isRobotConnected()
{
  return cliente.isConnected();
}

/****************************************
 * GETTERS
 *****************************************/

float Robot::getMotorTemperature(int i)
{
  return tempMotores[i];
}

float Robot::getMotorTension(int i)
{
  return tensionMotoresRead[i];
}

float Robot::getMotorCurrent(int i)
{
  return corrienteMotoresRead[i];
}

float Robot::getMotorRPM(int i)
{
  return rpmMotores[i];
}

bool Robot::seAlcanzoTempLimite()
{
  return temperaturaLimiteAlcanzada;
}

raylib::Vector3 Robot::getDireccion()
{
  return direccion;
}

/*******************************************************************************
 *******************************************************************************
                        MÉTODOS PRIVADOS
 *******************************************************************************
 ******************************************************************************/

/**
 * @brief Guarda en memoria los valores provenientes de la comunicación con el EDAbot
 */
void Robot::saveMessages()
{
  vector<MQTTMessage> mensajes;
  mensajes = cliente.getMessages();

  for (auto i : mensajes)
  {
    size_t indice;
    if ((indice = i.topic.find("motor")) != -1)
    {
      int motor = i.topic.at(indice + 5) - '1'; // se obtiene el número de motor

      if (i.topic.find("voltage") != -1)
        tensionMotoresRead[motor] = vecToFloat(i.payload);
      if (i.topic.find("current") != -1)
        corrienteMotoresRead[motor] = vecToFloat(i.payload);
      if (i.topic.find("rpm") != -1)
        rpmMotores[motor] = vecToFloat(i.payload);
      if (i.topic.find("temperature") != -1)
        tempMotores[motor] = vecToFloat(i.payload);
    }
  }
}

/**
 * @brief Actualiza (si es necesario) los nuevos valores de tensión en los motores.
 * A traves de una sencilla cuenta (que es una transformacion lineal), se transforma
 * el vector direccion en los valores correspondientes de tension de los motores
 *
 * @param x dirección en x
 * @param y dirección en y
 * @param r rotación
 * @param n nitro
 */
void Robot::updateMotorTension(float x, float y, float r, bool n)
{
  raylib::Vector3 vector;
  bool flagTemperatura = false;

  for(int i = 0; i < CANT_MOTORES && !flagTemperatura ; i++)
    flagTemperatura = tempMotores[i] > TEMP_LIMITE;

  temperaturaLimiteAlcanzada = flagTemperatura;
  
  if(flagTemperatura) //si la temperatura excede el límite permitido, se detienen los motores
  {
    vector = Vector3Zero();
    n = false;
  }
  else //sino, se toman los valores de entrada, normalizándolos
  {
    if(r > SENSIBILIDAD)
      r = 1;
    else if(r < -SENSIBILIDAD)
      r = -1;
    else
      r = 0;
    vector = {x, y, 0};
    vector = vector.Normalize(); // se normaliza para poder analizar si hubo repeticion
    vector.SetZ(r);
  }

   // si no hay repetición, actualiza los nuevos valores de tensión
  if ((direccion - vector).Length() > SENSIBILIDAD || n != nitro)
  {
    direccion = vector;
    nitro = n;

    raylib::Vector2 vec;

    // transformación lineal
    vec.SetX(direccion.GetX() + direccion.GetY());
    vec.SetY(direccion.GetY() - direccion.GetX());

    vec = vec.Normalize() * (n ? MOTOR_NITRO : MOTOR_VEL);

    float rot = direccion.GetZ() * MOTOR_ROT;

    setMotorTension(0, vec.GetY() + rot);
    setMotorTension(1, -vec.GetX() + rot);
    setMotorTension(2, -vec.GetY() + rot);
    setMotorTension(3, +vec.GetX() + rot);
  }
}

/**
 * @brief Cambia la tensión del motor indicado
 *
 * @param motor número de motor
 * @param v nuevo valor de tensión (debe estar entre -20V y 20V)
 */
void Robot::setMotorTension(int motor, float v)
{
  if (v < -MOTOR_NITRO || v > MOTOR_NITRO)
    return;

  vector<char> msg(sizeof(v));
  char c = motor + '1';
  memcpy(msg.data(), &v, sizeof(v));
  cliente.publish(ROBOT_ID + "/motor" + c + "/voltage/set", msg);
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/

/**
 * @brief Realiza un casteo de vector<char> a float
 *
 * @param numChar puntero al vector
 * @return valor que resulta del casteo
 */
static float vecToFloat(std::vector<char> &numChar)
{
  float x;
  memcpy(&x, numChar.data(), sizeof(float));
  return x;
}