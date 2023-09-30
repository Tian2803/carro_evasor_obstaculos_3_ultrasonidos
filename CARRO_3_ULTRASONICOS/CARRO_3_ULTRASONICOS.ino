#include <Servo.h>
#include <NewPing.h>
#include <AFMotor.h>

//Configuracion de la libreria NewPing para cada uno de los sensores
NewPing DistFront(A0, A1, 200);  //(trig,echo,dist max)  Este es el sensor de ultrasonido frontal
NewPing DistLeft(A2, A3, 200);  //(trig,echo,dist max) Este es el sensor de ultrasonido izquierdo
NewPing DistRight(A4, A5, 200);  //(trig,echo,dist max) Este es el sensor de ultrasonido derecho

//Declaracion de las variables de distancia para ada uno de los sensores
int distance = 0;
int distanceLeft = 0;
int distanceRight = 0;

int distanceD = 0;
int distanceIz = 0;
boolean Booleano = false;

AF_DCMotor MotorLeft1(1, MOTOR12_1KHZ);
AF_DCMotor MotorLeft2(2, MOTOR12_1KHZ);
AF_DCMotor MotorRight1(3, MOTOR34_1KHZ);
AF_DCMotor MotorRight2(4, MOTOR34_1KHZ);

void setup() {

}

void loop() {
  driveTruck();
}

//Metodo para medir la distancia
int medirDistancia(int Dist)  //Medir la distancia
{
  delay(10);
  int distanceCM = Dist;
  if (distanceCM <= 0 || distanceCM >= 250) {
    distanceCM = 250;
  }
  return distanceCM;
}

//Metodo de accionar el carro
void driveTruck() {
  distance = medirDistancia(DistFront.ping_cm());
  distanceLeft = medirDistancia(DistLeft.ping_cm());
  distanceRight = medirDistancia(DistRight.ping_cm());

  if ((distanceLeft >= 15 && distanceRight >= 15) && distance >= 25) {
    Forward();
  } else if (distanceLeft || distanceRight < 15) {
    if (distanceLeft >= distanceRight) {
      turnLeft(200);
      delay(200);
      Forward();
    } else if (distanceRight >= distanceLeft) {
      turnRight(200);
      delay(200);
      Forward();
    }
  } else if (distance < 25) {
    stop();
    delay(150);
    Backward();
    delay(250);
    stop();
    delay(250);
    distanceD = medirDistancia(DistLeft.ping_cm());
    delay(250);
    distanceIz = medirDistancia(DistRight.ping_cm());
    delay(250);

    if (distanceD >= distanceIz) {
      turnRight(300);
      stop();
    } else if (distanceIz >= distanceD) {
      turnLeft(300);
      stop();
    }
  }
}

//Metodo para frenar
void stop() {
  MotorLeft1.run(RELEASE);
  MotorLeft2.run(RELEASE);
  MotorRight1.run(RELEASE);
  MotorRight2.run(RELEASE);
}

//Metodo para ir adelante
void Forward() {
  if (Booleano == false) {
    Booleano = true;
    MotorLeft1.run(FORWARD);
    MotorLeft2.run(FORWARD);
    MotorRight1.run(FORWARD);
    MotorRight2.run(FORWARD);
    speedControl();
  }
}

//Metodo para ir en reversa o hacia atras
void Backward() {
  Booleano = false;
  MotorLeft1.run(BACKWARD);
  MotorLeft2.run(BACKWARD);
  MotorRight1.run(BACKWARD);
  MotorRight2.run(BACKWARD);
  speedControl();
}

//Metdo para girar a la derecha
void turnRight(int tiempo) {
  Booleano = false;
  MotorLeft1.run(FORWARD);
  MotorLeft2.run(FORWARD);
  MotorRight1.run(BACKWARD);
  MotorRight2.run(BACKWARD);
  delay(tiempo);
}

//Metodo para girar a la izquierda
void turnLeft(int tiempo) {
  Booleano = false;
  MotorLeft1.run(BACKWARD);
  MotorLeft2.run(BACKWARD);
  MotorRight1.run(FORWARD);
  MotorRight2.run(FORWARD);
  delay(tiempo);
}

//Metodo para el control de la velocidad
void speedControl() {
  for (int velocidad = 0; velocidad < 160; velocidad += 2) {
    MotorLeft1.setSpeed(velocidad);
    MotorLeft2.setSpeed(velocidad);
    MotorRight1.setSpeed(velocidad);
    MotorRight2.setSpeed(velocidad);
    delay(3);
  }
}