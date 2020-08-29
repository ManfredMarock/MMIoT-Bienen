

/* ___      ___    ___      ___    ________                 ________
  |   \    /   |  |   \    /   |  |__    __|     ______    |__    __|
  | |\ \  / /| |  | |\ \  / /| |     |  |       /  __   \     |  |
  | | \ \/ / | |  | | \ \/ / | |     |  |      |  /  \  |     |  |
  | |  \__/  | |  | |  \__/  | |   __|  |_     |  \__/  /     |  |
  |_|        |_|  |_|        |_|  | ___ __ |    \______/      |__|
  
  (c) 2019 MMIoT
     Author:         Manfred Marock
     Brief:          TheThingsUno Tuino1 Maker's Kit IoT Demo
     Application:    20200706_MMIot_Bienenwaage_8fach_2C.ino
     Version:        1.0
     Datum:          12.04.2020
*/

#define LORA
#define WAITTIME 1
//=================================================================
long receivedPayloadData = WAITTIME; //5 Minuten warten als Standardwert nach init
//=================================================================


//   Application EUI TheTingsNetwork
const char *appKey = "960B630BB0A260DD02AA32672750A807";
const char *appEui = "70B3D57ED00318BE";

//------------ Serial Interface -------------------------
#define loraSerial Serial1
#define debugSerial Serial

//---------------- Temperatur Sensor ---------------------
#define DS_WIRE_BUS 4

const int LOADCELL_1_DATA_PIN = 5;
const int LOADCELL_2_DATA_PIN = 6;
const int LOADCELL_3_DATA_PIN = 7;
const int LOADCELL_4_DATA_PIN = 9;
const int LOADCELL_5_DATA_PIN = 10;
const int LOADCELL_6_DATA_PIN = 11;
const int LOADCELL_7_DATA_PIN = 12;
const int LOADCELL_8_DATA_PIN = 2;

const int LOADCELL_CLK_PIN = 3;


int  values[10];
bool initArray;

//-------------- TTN -----------------------
#include <TheThingsNetwork.h>
#define freqPlan TTN_FP_EU868
TheThingsNetwork ttn(loraSerial, debugSerial, freqPlan);

//-----------------DS18B20 Temperatur --------------------
// Setup a oneWire instance to communicate with any OneWire devices  
// Data wire is plugged into pin 2 on the Arduino 
#include <OneWire.h> 
#include <DallasTemperature.h>

OneWire oneWire(DS_WIRE_BUS); 
//Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
uint16_t temperaturSensor_DS18B20;


//-------------- Waage -----------------------
//HX711 constructor (dout pin, sck pin)
#include <HX711.h>

HX711 scale_1;
HX711 scale_2;
HX711 scale_3;
HX711 scale_4;
HX711 scale_5;
HX711 scale_6;
HX711 scale_7;
HX711 scale_8;


int16_t fWaage_1 = 0.0;
int16_t fWaage_2 = 0.0;
int16_t fWaage_3 = 0.0;
int16_t fWaage_4 = 0.0;
int16_t fWaage_5 = 0.0;
int16_t fWaage_6 = 0.0;
int16_t fWaage_7 = 0.0;
int16_t fWaage_8 = 0.0;
int16_t fGewichtTotal;

bool bToggle = 0;

/*====================================================================================================*/
int16_t getScale(int channel, HX711 scale){
  scale.power_up();              // put the ADC in up mode
  delay(1000);
  int16_t weight = scale.get_units() ;

  Serial.print("\tZelle: ");
  Serial.print(channel);
  Serial.print(": ");
  Serial.println(weight);

  scale.power_down();              // put the ADC in sleep mode
  delay(1000);
  return  weight;
}

/******************************************************************/
int16_t getLigthScale(int channel, HX711 scale, int16_t faktor, int16_t offset)
{
  scale.power_up();              // put the ADC in up mode
  delay(1000);
  int32_t weight = (scale.get_units() * faktor) +  offset ;
  if (weight < 0)
     weight = 0;
  Serial.print("   Zelle: ");
  Serial.print(channel);
  Serial.print(" = ");
  Serial.print(weight);

  scale.power_down();              // put the ADC in sleep mode
  delay(1000);
  return  weight;
}

/*======================== SETUP ============================================================================*/
void setup()
{
  initArray = true;
  loraSerial.begin(57600);
  debugSerial.begin(9600);

   // Wait a maximum of 10s for Serial Monitor
  while (!debugSerial && millis() < 10000)
    ;
  debugSerial.println("Build:      06.07.2020");
  debugSerial.println("Programm:   20200706_MMIot_Bienenwaage_8fach_2C");
  debugSerial.println("Print:      MMIoT TTN LoRa Bienenwaage 8fach");
  debugSerial.println("            ohne MUX");
  debugSerial.print  ("Interval:   ");
  debugSerial.println( receivedPayloadData);
  debugSerial.println("Board:      Linino One");
  debugSerial.println("Device:     TheThingsUno SerNr ....2C");
  delay(100);
  
#ifdef LORA
  ttn.showStatus();

  // Set callback for incoming messages
  ttn.onMessage(message);
  debugSerial.println("-- JOIN TTN");
  
  ttn.join(appEui, appKey);
  debugSerial.println("-- JOIN -- OK --");
#endif

// Temperaturmessungen initialisieren

  debugSerial.println("--- init Bienenwaage 1 ---");
  scale_1.begin(LOADCELL_1_DATA_PIN, LOADCELL_CLK_PIN);
  scale_1.set_scale(100000.f); 


  debugSerial.println("--- init Bienenwaage 2 ---");
  scale_2.begin(LOADCELL_2_DATA_PIN, LOADCELL_CLK_PIN);
  scale_2.set_scale(100000.f); 
  
  debugSerial.println("--- init Bienenwaage 3 ---");
  scale_3.begin(LOADCELL_3_DATA_PIN, LOADCELL_CLK_PIN);
  scale_3.set_scale(100000.f); 

  debugSerial.println("--- init Bienenwaage 4 ---");
  scale_4.begin(LOADCELL_4_DATA_PIN, LOADCELL_CLK_PIN);
  scale_4.set_scale(100000.f); 

  debugSerial.println("--- init Bienenwaage 5 ---");
  scale_5.begin(LOADCELL_5_DATA_PIN, LOADCELL_CLK_PIN);
  scale_5.set_scale(100000.f); 

  debugSerial.println("--- init Bienenwaage 6 ---");
  scale_6.begin(LOADCELL_6_DATA_PIN, LOADCELL_CLK_PIN);
  scale_6.set_scale(100000.f); 


  debugSerial.println("--- init Bienenwaage 7 ---");
  scale_7.begin(LOADCELL_7_DATA_PIN, LOADCELL_CLK_PIN);
  scale_7.set_scale(10000.f); 

  debugSerial.println("--- init Bienenwaage 8 ---");
  scale_8.begin(LOADCELL_8_DATA_PIN, LOADCELL_CLK_PIN);
  scale_8.set_scale(10000.f); 


  debugSerial.println("Startup abgeschlossen");
}

/*================================ LOOP ====================================================================*/
void loop()
{

  getDS18B20Temp();

  fWaage_1 = getLigthScale(1, scale_1, 3281, -4526);
 // Layoutfehler fWaage_2 =  getLigthScale(2, scale_2, 3281, -000); 
  fWaage_3 = 100+getLigthScale(3, scale_3,-4900, 20000 );  //Geeicht am 02.08.20 80kg
  fWaage_4 = getLigthScale(4, scale_4, 5200, -400);  // Geeicht am 02.08.20 80kg
  fWaage_5 =  getLigthScale(5, scale_5, -3200, -4100); // Geeicht am 20.08.20 80kg
// fWaage_6 =  getLigthScale(6, scale_6, 3900, -0); 
// fWaage_7 =  getLigthScale(7, scale_7, 3900, 0); 
// fWaage_8 =  getLigthScale(8, scale_8, 390000, -0); 

  fGewichtTotal = fWaage_1 + fWaage_3 + fWaage_4 + fWaage_5 + fWaage_6 + fWaage_7 + fWaage_8;
  
  debugSerial.println();

 
#ifdef LORA
   writeTTN();
#endif  

  waitMin(receivedPayloadData);

}

// ================ warte Minuten ========================================================
void waitMin(int min)
{
  for (int i = 0; i < min; i++){ 
    debugSerial.print(".");
    for(int sec = 0; sec < 60; sec++)
      delay(1000);
  }     
}

// ================ Temperatur messen ====================================================
//  DS18B20 is 1-Wire digital temperature senso
uint16_t getDS18B20Temp(){

   sensors.requestTemperatures(); // Send the command to get temperature readings
   debugSerial.print("getDS18B20Temp: \t");
   temperaturSensor_DS18B20  =  sensors.getTempCByIndex(0)*100; 
   debugSerial.print(temperaturSensor_DS18B20);
   debugSerial.println("°C");
   return temperaturSensor_DS18B20;
}

// ===================== WRITE TTN DATA ===================================
void writeTTN(){
  // Read sensor values and multiply by 100 to effictively have 2 decimals
  // Split both words (16 bits) into 2 bytes of 8
  char payload[36]; //1
  payload[0] = 0x01;
  payload[1] = 0x02;
  payload[2] = highByte(temperaturSensor_DS18B20);
  payload[3] = lowByte(temperaturSensor_DS18B20);
  
  payload[4] = 0x02;
  payload[5] = 0x02;
  payload[6] = highByte(fWaage_1);
  payload[7] = lowByte(fWaage_1);
  
//  payload[8] = 0x03;
//  payload[9] = 0x02;
//  payload[10] = highByte(fWaage_2);
//  payload[11] = lowByte(fWaage_2);
  
  payload[8] = 0x04;
  payload[9] = 0x02;
  payload[10] = highByte(fWaage_3);
  payload[11] = lowByte(fWaage_3);

  payload[12] = 0x05;
  payload[13] = 0x02;
  payload[14] = highByte(fWaage_4);
  payload[15] = lowByte(fWaage_4);

  payload[16] = 0x06;
  payload[17] = 0x02;
  payload[18] = highByte(fWaage_5);
  payload[19] = lowByte(fWaage_5);

  payload[20] = 0x07;
  payload[21] = 0x02;
  payload[22] = highByte(fWaage_6);
  payload[23] = lowByte(fWaage_6);

  payload[24] = 0x08;
  payload[25] = 0x02;
  payload[26] = highByte(fWaage_7);
  payload[27] = lowByte(fWaage_7);

  payload[28] = 0x09;
  payload[29] = 0x02;
  payload[30] = highByte(fWaage_8);
  payload[31] = lowByte(fWaage_8);

 /* 
  *  payload[36] = 0x09;
  payload[37] = 0x02;
  payload[38] = highByte(temperaturSensor_DS18B20);
  payload[39] = lowByte(temperaturSensor_DS18B20);
  */
  
  bToggle = !bToggle;
  digitalWrite(LED_BUILTIN, bToggle);
  ttn.sendBytes(payload, sizeof(payload));
}


// ================================== ONEMESSAGE ===================================
void message(const uint8_t *payload, size_t size, port_t port)
{
 // receivedPayloadData = (payload[1] >> 4)*1000 + (payload[1] & 0x0F) * 100 + (payload[0] >> 4)*10 + (payload[0] & 0x0F);7
  receivedPayloadData =  (payload[0] >> 4)*10 + (payload[0] & 0x0F);
  debugSerial.print("Payload Message Data = ");
  debugSerial.println(receivedPayloadData);
}
