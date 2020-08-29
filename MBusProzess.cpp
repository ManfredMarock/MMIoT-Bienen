/* ___      ___    ___      ___    _____               _____
  |   \    /   |  |   \    /   |  |_   _|     ____    |_   _|
  | |\ \  / /| |  | |\ \  / /| |    | |      /    \     | |
  | | \ \/ / | |  | | \ \/ / | |    | |     | /  \ |    | |
  | |  \__/  | |  | |  \__/  | |   _| |_    | \__/ /    | |
  |_|        |_|  |_|        |_|  |_____|    \____/     |_|
  
  (c) 2019 MMsIoT#include <Wire.h> 
*/
#include "MBus.h"
#include "MBusProzess.h"

MBus mbus(1);

#define ANZAHL_METER 26

#define MBUS_ADDRESS 0
#define MESSUNG 1
#define SERIEN_NUMMER 2
#define T_VORLAUF 3
#define T_RUECKLAUF 4
#define VOLUMEN_FLOW 5
#define FLOW_EXTERN 6
#define FLOW 7
#define ARBEIT 8
#define LEISTUNG 9


String  strMBusData;
String readString; 
String sMMSERNr;
String sToInt;
String sCom2Winmos32;
String flats[26] = {"W11","W11","W12","W12","W13","W13","W14","W14","W21","W21","W22","W22","W23","W23","W24","W24","W31","W31","W32","W32","W33","W33",
                    "W34","W34","W35","W35"};
String incomingData;
String sWData;

int iCounter; 
int nLoops = 0;
int iMBusData;
int nMeter;
//int iThingSpeak;



//*****************************************************************************

#define DEBUG  

//======================================================= 
//==           SETUP
//======================================================= 
void MBusProzess::setupMBus() {   
  Serial.println("\nMBusProzess-Klassen Erweiterung für LoRa Projekt"); 
  Serial.println("Gampelutz 5+7, Gallenburst 23, Mauren"); 
  Serial.println("25.12.2019"); 
  delay(1000); 
} 

//======================================================= 
//==             getMBusData
//======================================================= 
void MBusProzess::getMBusData(word dataArray[]) {
   Serial.println("getMBusData");
   
// -----------  Zaehler auslesen------------------------
  for (int meterNum = 0; meterNum < ANZAHL_METER ; meterNum++){
    delay(300);

// ------------  REQUEST DATA Send Command to Meter -----------
    mbus.request_MBusSerialNumber(meterNum);

   
//-----------   RECEIVE DATA FROM MBUS  -------------- 
    String  sMBus = mbus.receive_MBusData();   // -- 10 --
    Serial.println("--- "+sMBus);    
    strMBusData =sMBus;// sMBus.substring(iMBusData);   
    delay(100);

// ----------    Any data from UART1 received --------------
    if (strMBusData.length() > 30){ 
// --------  Interpret MBus Data     -------- 
       mbus.search_MBusParameter(strMBusData, meterNum, dataArray, flats);  // -- 0 --
    } 
    else{
      Serial.println("<<!! -------------- Error no MBus-Data -------------- !! ");
      Serial.println(strMBusData);
      dataArray[meterNum] = 0;
    }
    delay(500); //5000
  }  
}


 
