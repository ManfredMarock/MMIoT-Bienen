/* ___      ___    ___      ___    _______      ______    _______
  |   \    /   |  |   \    /   |  |__   __|    /  ___  \  |__   __|
  | |\ \  / /| |  | |\ \  / /| |     | |      |  /   \  |    | |
  | | \ \/ / | |  | | \ \/ / | |     | |      |  |    | |    | |
  | |  \__/  | |  | |  \__/  | |   __| |__    |  \__ /  /    | |
  |_|        |_|  |_|        |_|  |_______|    \_______/     |_|
  (c) 2019 MMsIoT
     Author: Manfred Marock
     Brief: Tuino1 Maker's Kit IoT Demo
     Version: 1.0
*/

#include "gmx_lr.h"
#include "Regexp.h"
#include "SeeedOLED.h"
#include "display_utils.h"

#include "MBusProzess.h"

#define MESSINTERVAL 9000 //x * ~2s
#define LORA
//#define SHOWDATAINTERVALL 10000

String haus1_1[4] = {"W11 W=", "    H=", "W12 W=", "    H="};
String haus1_2[4] = {"W13 W=", "    H=", "W14 W=", "    H="};
String haus2_1[4] = {"W21 W=", "    H=", "W22 W=", "    H="};
String haus2_2[4] = {"W23 W=", "    H=", "W24 W=", "    H="};
String haus3_1[8] = {"W31 W=", "    H=", "W32 W=", "    H="};
String haus3_2[4] = {"W33 W=", "    H=", "W34 W=", "    H="};
String haus3_3[8] = {"W34 W=", "    H=", "W35 W=", "    H="};

word dataArray[64];
int timeCounts = 0;
int dataHaus1_1[8];
int dataHaus1_2[8];
int dataHaus2_1[8];
int dataHaus2_2[8];
int dataHaus3_1[8];
int dataHaus3_2[8];
int dataHaus3_3[8];

bool loadNewLCD = true;
                 
 
/****************************************************
* Prototyp
****************************************************/
void fillLoRaData(String dataName,  int dataIndex, word data, byte tx_buf[]);

#include <Wire.h>

// NFC
#include "M24SR.h"
#include "NdefMessage.h"
#include "NdefRecord.h"


// RegExp Engine for NFC parsing
MatchState nfc_ms;
char regexp_buf[512];

#define gpo_pin TUINO_NFC_INTERRUPT


M24SR m24sr04(gpo_pin);
// END NFC

bool writing_nfc = false;
bool data_received = false;

int iGlobal = 0;
int ledState = 0;
int buttonPin = D4;
int relayPin = D5;
int tempSensorPin = A0;
int testInc =0;
int oledAnzeige = 0;

String oled_string;

 // TTN Projekt meterreading_01
String Device_EUI_Galenburst = "78:AF:58:03:00:00:08:16";
String App_EUI_Ziel = "70:B3:D5:7E:D0:03:37:9D";
String App_EUI_act;
String App_Key_Galenburst = "86:DD:0D:F3:2E:65:BD:0D:21:0E:69:3D:87:60:53:D5";
String App_Key_act;


/****************************************************
* Lora Daten empfangen
****************************************************/
void loraRx() {
  data_received = true;
}

/****************************************************
*
****************************************************/
void writeNFC() {
  char string[32];
  String tmpString;
  String nfc_data;

  gmxLR_getDevEui(tmpString);
  nfc_data = "DevEUI:" + tmpString;
  nfc_data = nfc_data + "\n\r";

//  nfc_data = nfc_data + "Tempef:" + String(readTemp());
//  nfc_data = nfc_data + "\n\r";
  nfc_data = nfc_data + "Text:" + oled_string;
  nfc_data = nfc_data + "\n\r";

  // WriteNFC data
  writing_nfc = true;
  NdefMessage message = NdefMessage();
  message.addTextRecord(nfc_data);
  m24sr04.writeNdefMessage(&message);
  delay(200);
  writing_nfc = false;
}

/****************************************************
* Button Oled Abzeige 
****************************************************/
bool waitButton() {
  if (digitalRead(buttonPin) == 0 )
     return false;
  else{
    oledAnzeige++;
    if (oledAnzeige > 6) oledAnzeige = 0;
    return true;
  }  
}
/************************************************************************
 *Messwerte auf OLED anzeigen                                                                     *
 ************************************************************************/
void showMeterData(String house[], int dataArray[])
{
  char data0[20];
  char flat0[16];
  house[0].toCharArray(flat0, 9);
  sprintf(data0, "%s %d %d",flat0, dataArray[0],dataArray[1]);

  char data1[30];
  char flat1[8];
  house[1].toCharArray(flat1, 9);
  sprintf(data1,"%s %d %d", flat1, dataArray[2], dataArray[3]);

  char data2[30];
  char flat2[8];
  house[2].toCharArray(flat2, 9);
  sprintf(data2,"%s %d %d", flat2, dataArray[4],dataArray[5]);

  char data3[30];
  char flat3[8];
  house[3].toCharArray(flat3, 9);
  sprintf(data3,"%s %d %d", flat3, dataArray[6],dataArray[7]);

  showOLEDText(data0," ", data1," ", data2," ", data3,"");
}

/************************************************************************
* SETUP
************************************************************************/
void setup() {
  // put your setup code here, to run once:
  String DevEui;
  String AppEui;
  String AppKey;
  
  String loraClass,LoRaWANClass;
  char string[64];

  String adr, dcs, dxrate;

  byte join_status;
  int join_wait;

  Wire.begin();
  Serial.begin(2400, SERIAL_8E1);
  Serial.println("Starting");
  Serial.println("Datei:           MBusauslesung_Ziel <V1.21./_ 20200513_TTN_tuini1_MBus_V01.ino");
  Serial.println("Prozessor:       Tuino1 - Gimasi Maker's Kit");
  Serial.println("Datum:           04.07.2020. -->  09.04.2020 08.20");
  Serial.println("Autor:           Manfred Marock MMIoT");
  Serial.print("Messinterval:    [s]");
  Serial.println(MESSINTERVAL * 2);
#ifdef LORA
  Serial.println("LoRa-Verbindung  EIN");
#else
  Serial.println("LoRa-Verbindung  AUS");
#endif
  // Startupp NFC
  m24sr04._setup();

  // Init Oled
  // GMX-LR init pass callback function
  gmxLR_init(&loraRx);
 
  // Set AppEui and AppKey
  // Uncomment these if you want to change the default keys
    
  LoRaWANClass = "C";


#ifdef MULTIREGION
  gmxLR_getRegion(Region);
  if (NewLoraRegion.length() > 0 )
  {
    Region.trim();    
    Serial.println("**** UPDATING Region ****");
  
    if ( !Region.equals(NewLoraRegion) )
    {
       Serial.println("Setting Region:--"+NewLoraRegion);
       gmxLR_setRegion(NewLoraRegion);
       // reboot GMX_LR1
       gmxLR_Reset();
    }
    else
    {
       Serial.println("Region is already:"+Region);
    }
  }
#endif

  gmxLR_getAppEui(AppEui);
   Serial.print(AppEui);
  if (AppEui.length() > 0 )
  {
    AppEui.trim();
        
    Serial.println("*-*** UPDATING AppEUI ***-*");
   // if ( !AppEui.equals(App_EUI_Ziel) )
   if (true)
    {
      Serial.println("\nSetting AppEui:--"+App_EUI_Ziel);
      gmxLR_setAppEui(App_EUI_Ziel);
      gmxLR_getAppEui(App_EUI_act);
      Serial.println("Set AppEui:    >>"+App_EUI_act);
    }
    else
    {
      Serial.println("AppEui is already:"+App_EUI_Ziel);
    }
  }

  gmxLR_getAppKey(AppKey);
  if (App_Key_Galenburst.length() > 0 )
  {
    AppKey.trim();
      
    Serial.println("**** UPDATING AppKey ****");
    if ( !AppKey.equals(App_Key_Galenburst) )
    {
      Serial.println("Setting AppKey:--"+App_Key_Galenburst);
      gmxLR_setAppKey(App_Key_Galenburst);
    }
    else
    {
      Serial.println("AppKey is already:"+AppKey);
    }
  }

  // Disable Duty Cycle  ONLY FOR DEBUG!
  gmxLR_setDutyCycle("0");

  // Set LoRaWAN Class
  gmxLR_setClass(LoRaWANClass);
  
  // Show Splash Screen on OLED
   char data[100];
   sprintf(data, "W11= %d - %d ",11, 3);
   showOLEDText("MMIoT MBus", "", "Ziel V0.1.21", "","04.07.2020","","20200513_TTN_","tuino1_MBus_V01");

 //  splashScreen();
  delay(500);

  // Show LoRaWAN Params on OLED
  gmxLR_getDevEui(DevEui);
  gmxLR_getAppKey(AppKey);
  gmxLR_getAppEui(AppEui);
//  displayLoraWanParams(DevEui, AppEui, AppKey);

  delay(1000);
  
  Serial.println("Joining...");
  join_wait = 0;

#ifdef LORA
  while ((join_status = gmxLR_isNetworkJoined()) != LORA_NETWORK_JOINED) {
    if ( join_wait == 0 )
    {
      Serial.println("LoRaWAN Params:");
      Serial.println("DevEui:" + DevEui);
      gmxLR_getAppEui(AppEui);
      Serial.println("AppEui:" + AppEui);
      gmxLR_getAppKey(AppKey);
      Serial.println("AppKey:" + AppKey);
      gmxLR_getClass(loraClass);
      Serial.println("Class:" + loraClass);
      adr = String( gmxLR_getADR() );
      Serial.println("ADR:" + adr);
      dcs = String( gmxLR_getDutyCycle() );
      Serial.println("DCS:" + dcs);
      gmxLR_getRX2DataRate(dxrate);
      Serial.println("RX2 DataRate:" + dxrate);

      gmxLR_Join();
    } //if

    sprintf(string, "Attempt: %d", join_wait);

    join_wait++;

    if (!( join_wait % 100 )) {
      gmxLR_Reset();
      join_wait = 0;
    }

    delay(1000);
  }; //while
#endif

  delay(1000);
  MBusProzess::setupMBus();
}

/************************************************************************
* LOOP
************************************************************************/
void loop() {
  String rx_data;
  String tx_data;
  String lora_string;

  char lora_data[120];
  char lora_rx_string[256];
  
  byte tx_buf[256];
  byte rx_buf[128];
  int buf_len;
  int port;
  long int delta_lora_tx;

//--------------------------------------------------------------

  if ((timeCounts > MESSINTERVAL ) ||(timeCounts < 1))
  { 
    timeCounts =0;
    MBusProzess::getMBusData(dataArray);

    setTransferData(dataArray,  0, tx_buf, lora_data);  // W11-W12 Wasser Wärmezähler   
    setTransferData(dataArray,  8, tx_buf, lora_data);  // W13-W14 Wasser Wärmezähler
 
    setTransferData(dataArray,  16, tx_buf, lora_data);  // W21-W22 Wasser Wärmezähler   
    setTransferData(dataArray,  24, tx_buf, lora_data);  // W23-W24 Wasser Wärmezähler 
   
    setTransferData(dataArray,  32, tx_buf, lora_data);  // W31-W32 Wasser Wärmezähler   
    setTransferData(dataArray,  40, tx_buf, lora_data);  // W33-W34 Wasser Wärmezähler 
    setTransferData(dataArray,  48, tx_buf, lora_data);  // W32-W35 Wasser Wärmezähler 

    for(int i = 0; i < 8; i++){
      dataHaus1_1[i] = dataArray[i];  // Daten  für OLED abfüllen
      dataHaus1_2[i] = dataArray[i+8];
      dataHaus2_1[i] = dataArray[i+16];
      dataHaus2_2[i] = dataArray[i+24];
      dataHaus3_1[i] = dataArray[i+32];
      dataHaus3_2[i] = dataArray[i+40];
      dataHaus3_3[i] = dataArray[i+48];
    }
  }

  if (waitButton()){
    switch (oledAnzeige){ 
    case 0: showMeterData(haus1_1, dataHaus1_1); break;    
    case 1: showMeterData(haus1_2, dataHaus1_2); break;   
    case 2: showMeterData(haus2_1, dataHaus2_1); break;    
    case 3: showMeterData(haus2_2, dataHaus2_2); break;   
    case 4: showMeterData(haus3_1, dataHaus3_1); break;    
    case 5: showMeterData(haus3_2, dataHaus3_2); break;  
    case 6: showMeterData(haus3_3, dataHaus3_3); break;  
    
    default:    Serial.println("Error 001"); oledAnzeige = -1;  break; 
    }
  }

  delay(100);
  timeCounts++;

  // End Transmission
/*
  if (data_received)
  {
    displayLoraRX(true);

    gmxLR_RXData(rx_data, &port);
    gmxLR_StringToHex(rx_data, rx_buf, &buf_len );

    Serial.println("LORA RX DATA:" + rx_data);
    Serial.print("LORA RX LEN:");
    Serial.println(buf_len);
    Serial.print("LORA RX PORT:");
    Serial.println(port);

    rx_buf[buf_len] = 0;
    sprintf(lora_rx_string,"%s",rx_buf);
    Serial.println( lora_rx_string );
    oled_string = String( lora_rx_string );

    data_received = false;
    displayLoraRX(false);
  }
  */

}



/****************************************************
*  Transfer Array abfüllen
************* ***************************************/
void fillLoRaData(int dataIndex, word data, byte tx_buf[])
{
  char str[80];
  sprintf(str, "\nNr %d, = %d", dataIndex, data);
  Serial.print(str);
  tx_buf[4*dataIndex] = dataIndex+1; // packet header - multiple data
  tx_buf[4*dataIndex+1] = 0x02;  // Analogmesswert
  tx_buf[4*dataIndex+2] = (data & 0xff00) >> 8;
  tx_buf[4*dataIndex+3] = (data & 0x00ff);      
}

/****************************************************
*  Aufbereiten von 8 Zähler daten für den Datentransfer
****************************************************/
void setTransferData(word dataArray[], int posArray, byte tx_buf[], char lora_data[])
{
  for (int i = posArray; i < posArray+8; i++)
  {
    fillLoRaData( i, dataArray[i], tx_buf);
  }

  
  getLoRaData(tx_buf, lora_data,posArray);  

  char str[200];
  sprintf(str,"Loradata %s", lora_data);
  Serial.println(str);
      
  writeNFC_Data(lora_data);
  delay(20000);
}

/****************************************************
*  LoRa Daten bereitstellen für Transfer
****************************************************/
void getLoRaData(byte tx_buf[], char lora_data[], int offset)
{
  offset = offset * 4;
  sprintf(lora_data,"%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",  
  tx_buf[0 + offset], tx_buf[1 + offset], tx_buf[2 + offset], tx_buf[3 + offset], 
  tx_buf[4 + offset], tx_buf[5 + offset], tx_buf[6 + offset], tx_buf[7 + offset],
  tx_buf[8 + offset], tx_buf[9 + offset], tx_buf[10 + offset], tx_buf[11 + offset], 
  tx_buf[12 + offset], tx_buf[13 + offset], tx_buf[14 + offset], tx_buf[15 + offset],
    
  tx_buf[16 + offset], tx_buf[17 + offset], tx_buf[18 + offset], tx_buf[19 + offset],
  tx_buf[20 + offset], tx_buf[21 + offset], tx_buf[22 + offset], tx_buf[23 + offset],
  tx_buf[24 + offset], tx_buf[25 + offset], tx_buf[26 + offset], tx_buf[27 + offset],
  tx_buf[28 + offset], tx_buf[29 + offset], tx_buf[30 + offset], tx_buf[31 + offset]);  
 
  Serial.println("********** LORA DATA *********");
  Serial.println(lora_data);
}


/****************************************************
*  LoRa Daten bereitstellen für Transfer
****************************************************/

/****************************************************
*  Daten via LoRa Kommunication versenden
****************************************************/
void writeNFC_Data(char lora_data[])
{  
  String tx_data;
  displayLoraTX(true);
  tx_data = String(lora_data);
//  Serial.println("TX DATA:" + tx_data);
  gmxLR_TXData(tx_data);

  // Update NFC
  writeNFC();

  displayLoraTX(false);
}
