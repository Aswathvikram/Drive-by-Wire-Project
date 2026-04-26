#include <WiFi.h>
#include <esp_now.h>
#include <ESP32Servo.h>

// ================= SERVO =================
#define SERVO_PIN 33
Servo steeringServo;

#define TRUE_STEERING_CENTER 76
#define TX_CENTER 90
#define STEERING_MIN 45
#define STEERING_MAX 135

// ================= MOTOR =================
#define ENA 25
#define IN1 26
#define IN2 27

#define ENB 14
#define IN3 12
#define IN4 13

// ================= TRANSMITTER MAC =================
uint8_t allowedMAC[] = {0x84,0x1F,0xE8,0x2B,0x65,0x44};

// ================= DATA =================
typedef struct __attribute__((packed)) {
  float accelPercent;
  float brakePercent;   // kept but ignored
  int steeringDeg;
} DBW_Packet;

DBW_Packet rxData;

// ================= STATE =================
bool transmitterConnected=false;
unsigned long lastReceiveTime=0;

const int SIGNAL_LOSS_TIME=3000;

int currentPWM=0;

// ================= DIRECTION =================
void setForward()
{
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
}

// ================= CALLBACK =================
void onDataRecv(const uint8_t *mac,const uint8_t *incomingData,int len)
{

  if(memcmp(mac,allowedMAC,6)!=0) return;

  memcpy(&rxData,incomingData,sizeof(rxData));

  transmitterConnected=true;
  lastReceiveTime=millis();

  // ===== STEERING =====
  int correctedSteering =
  rxData.steeringDeg - TX_CENTER + TRUE_STEERING_CENTER;

  correctedSteering =
  constrain(correctedSteering,STEERING_MIN,STEERING_MAX);

  steeringServo.write(correctedSteering);

  // ===== MOTOR CONTROL =====

  float accel = rxData.accelPercent;

  int targetPWM =
  map(accel,0,100,0,255);

  currentPWM = targetPWM;

  analogWrite(ENA,currentPWM);
  analogWrite(ENB,currentPWM);

  // ===== DEBUG =====
  Serial.println("----------------------------");
  Serial.print("Accel % : ");
  Serial.println(accel,1);

  Serial.print("PWM : ");
  Serial.println(currentPWM);
}

// ================= SETUP =================
void setup()
{
  Serial.begin(115200);

  steeringServo.attach(SERVO_PIN);
  steeringServo.write(TRUE_STEERING_CENTER);

  pinMode(ENA,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);

  pinMode(ENB,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

  setForward();

  WiFi.mode(WIFI_STA);

  esp_now_init();
  esp_now_register_recv_cb(onDataRecv);

  Serial.println("RECEIVER READY");
}

// ================= LOOP =================
void loop()
{

  if(transmitterConnected &&
     millis()-lastReceiveTime > SIGNAL_LOSS_TIME)
  {
    if(currentPWM>0) currentPWM--;

    analogWrite(ENA,currentPWM);
    analogWrite(ENB,currentPWM);

    if(currentPWM==0)
    {
      steeringServo.write(TRUE_STEERING_CENTER);
      transmitterConnected=false;
    }
  }

}