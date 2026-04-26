#include <WiFi.h>
#include <esp_now.h>

// ================= INPUTS =================
#define POT_ACCEL   34
#define POT_BRAKE   35
#define ENC_CLK     32
#define ENC_DT      33

// ================= STEERING =================
const int MAX_ENCODER  = 20;
const int STEER_CENTER = 90;
const int STEER_RANGE  = 45;

long encoderValue = 0;
int lastCLK = HIGH;
int steeringAngle = STEER_CENTER;

// ================= PEDAL CALIBRATION =================
const float ACCEL_MIN = 46.6;
const float ACCEL_MAX = 77.5;

const float BRAKE_MIN = 0.0;
const float BRAKE_MAX = 30.0;

// ================= TIMING =================
unsigned long lastSendTime  = 0;
unsigned long lastPrintTime = 0;

const int SEND_INTERVAL  = 10;
const int PRINT_INTERVAL = 200;

// ================= RECEIVER MAC =================
uint8_t receiverMAC[] = {0x84,0x1F,0xE8,0x2F,0x97,0x84};

// ================= DATA PACKET =================
typedef struct __attribute__((packed)) {
  float accelPercent;
  float brakePercent;
  int steeringDeg;
} DBW_Packet;

DBW_Packet txData;

bool sendStatus = false;

// ================= SEND CALLBACK =================
void onDataSent(const uint8_t *mac, esp_now_send_status_t status)
{
  sendStatus = (status == ESP_NOW_SEND_SUCCESS);
}

// ================= PEDAL MAP =================
float mapPedal(float value,float minVal,float maxVal)
{
  if(value < minVal) return 0;
  if(value > maxVal) return 100;
  return ((value-minVal)/(maxVal-minVal))*100.0;
}

void setup()
{
  Serial.begin(115200);

  pinMode(ENC_CLK,INPUT_PULLUP);
  pinMode(ENC_DT,INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  Serial.print("Transmitter MAC: ");
  Serial.println(WiFi.macAddress());

  esp_now_init();
  esp_now_register_send_cb(onDataSent);

  esp_now_peer_info_t peerInfo={};
  memcpy(peerInfo.peer_addr,receiverMAC,6);
  peerInfo.channel=0;
  peerInfo.encrypt=false;

  esp_now_add_peer(&peerInfo);

  Serial.println("TRANSMITTER READY");
}

void loop()
{

  // ===== ENCODER =====
  int clkState = digitalRead(ENC_CLK);

  if(clkState != lastCLK && clkState==LOW)
  {
    if(digitalRead(ENC_DT)) encoderValue++;
    else encoderValue--;

    encoderValue = constrain(encoderValue,-MAX_ENCODER,MAX_ENCODER);

    float ratio = (float)encoderValue / MAX_ENCODER;
    steeringAngle = STEER_CENTER + ratio*STEER_RANGE;
  }

  lastCLK = clkState;

  // ===== SEND =====
  if(millis()-lastSendTime >= SEND_INTERVAL)
  {
    lastSendTime = millis();

    float accelRaw = analogRead(POT_ACCEL)*100.0/4095.0;
    float brakeRaw = analogRead(POT_BRAKE)*100.0/4095.0;

    txData.accelPercent = mapPedal(accelRaw,ACCEL_MIN,ACCEL_MAX);
    txData.brakePercent = mapPedal(brakeRaw,BRAKE_MIN,BRAKE_MAX);
    txData.steeringDeg  = steeringAngle;

    esp_now_send(receiverMAC,(uint8_t*)&txData,sizeof(txData));
  }

  // ===== PRINT =====
  if(millis()-lastPrintTime >= PRINT_INTERVAL)
  {
    lastPrintTime = millis();

    Serial.println("----------------------------");
    Serial.print("Connection : ");
    Serial.println(sendStatus ? "SUCCESS":"FAIL");

    Serial.print("Encoder    : ");
    Serial.println(encoderValue);

    Serial.print("Steering   : ");
    Serial.println(steeringAngle);

    Serial.print("Accel %    : ");
    Serial.println(txData.accelPercent,1);

    Serial.print("Brake %    : ");
    Serial.println(txData.brakePercent,1);
  }

}