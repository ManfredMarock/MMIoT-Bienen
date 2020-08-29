/* ___      ___    ___      ___    _____               _____
  |   \    /   |  |   \    /   |  |_   _|     ____    |_   _|
  | |\ \  / /| |  | |\ \  / /| |    | |      /    \     | |
  | | \ \/ / | |  | | \ \/ / | |    | |     | /  \ |    | |
  | |  \__/  | |  | |  \__/  | |   _| |_    | \__/ /    | |
  |_|        |_|  |_|        |_|  |_____|    \____/     |_|
  
  (c) 2019 MMsIoT
*/
//#include <ESP8266WiFi.h>
#include "MBus.h"

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

//#define TEST
#define LIVE

byte req_MBus_data[] = {0x10, 0x7B, 0x01, 0x7C, 0x16}; 
byte sendSerNrRequest[5] = {0x10, 0x7B, 0xFD, 0x78, 0x16};
//byte req_Command[5] = {107BFD7816};



 //680B0B6873FD5242971469FFFFFFFF1416  // Zähler via Seriennummer adressieren  Wärmezähler
 //680B0B6873FD5224396117FFFFFFFF6216  Wasserzähler
 //107B017C16                          // Request

#ifdef TEST
byte W11_MB11_H2O[17] =   {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145
byte W11_MB15_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x24,0x39,0x61,0x17,0xFF,0xFF,0xFF,0xFF,0x93,0x16};  // GWF Wasserzähler 17613924
byte W12_MB12_H2O[17] =   {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x24,0x39,0x61,0x17,0xFF,0xFF,0xFF,0xFF,0x93,0x16};  // GWF Wasserzähler 17613924
byte W12_MB16_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x24,0x39,0x61,0x17,0xFF,0xFF,0xFF,0xFF,0x93,0x16};  // GWF Wasserzähler 17613924
byte W13_MB13_H2O [17] =   {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145
byte W13_MB17_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145
byte W14_MB14_H2O[17] =   {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145 
byte W14_MB18_Heat[17] ={0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x24,0x39,0x61,0x17,0xFF,0xFF,0xFF,0xFF,0x93,0x16};  // GWF Wasserzähler 17613924
 
byte W21_MB21_H2O[17] =   {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145
byte W21_MB25_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145
byte W22_MB22_H2O[17] =   {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x24,0x39,0x61,0x17,0xFF,0xFF,0xFF,0xFF,0x93,0x16};  // GWF Wasserzähler 17613924
byte W22_MB26_Heat[17] ={0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145
byte W23_MB23_H2O[17] =   {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145
byte W23_MB27_Heat[17] =  {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145
byte W24_MB24_H2O[17] =   {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145 
byte W24_MB28_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145

byte W31_MB31_H2O[17] =   {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145
byte W31_MB36_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145
byte W32_MB32_H2O[17] =   {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x24,0x39,0x61,0x17,0xFF,0xFF,0xFF,0xFF,0x93,0x16};  // GWF Wasserzähler 17613924
byte W32_MB37_Heat[17] =  {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145
byte W33_MB33_H2O[17] =   {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145
byte W33_MB38_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145 
byte W34_MB34_H2O[17] =   {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145 
byte W34_MB39_Heat[17] ={0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145 
byte W35_MB35_H2O[17] =   {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145 
byte W35_MB40_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x45,0x91,0x71,0x14,0xFF,0xFF,0xFF,0xFF,0x19,0x16};  // GWF Wasserzähler 14719145

#endif

#ifdef LIVE
 byte W11_MB11_H2O[17] =   {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x14,0x31,0x45,0x19,0xFF,0xFF,0xFF,0xFF,0x61,0x16};  // Gampelutz5 Wasser 
 byte W12_MB12_H2O[17] =   {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x15,0x31,0x45,0x19,0xFF,0xFF,0xFF,0xFF,0x62,0x16};
 byte W13_MB13_H2O[17] =   {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x16,0x31,0x45,0x19,0xFF,0xFF,0xFF,0xFF,0x63,0x16};
 byte W14_MB14_H2O[17] =   {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x17,0x31,0x45,0x19,0xFF,0xFF,0xFF,0xFF,0x64,0x16};                

 byte W21_MB21_H2O[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x18,0x31,0x45,0x19,0xFF,0xFF,0xFF,0xFF,0x65,0x16};  // Gampelutz5 Wasser 
 byte W22_MB22_H2O[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x19,0x31,0x45,0x19,0xFF,0xFF,0xFF,0xFF,0x66,0x16};
 byte W23_MB23_H2O[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x20,0x31,0x45,0x19,0xFF,0xFF,0xFF,0xFF,0x67,0x16};
 byte W24_MB24_H2O[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x21,0x31,0x45,0x19,0xFF,0xFF,0xFF,0xFF,0x6E,0x16};                

 byte W31_MB31_H2O[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x22,0x31,0x45,0x19,0xFF,0xFF,0xFF,0xFF,0x6F,0x16};  // Galenburst 23 Wasser 
 byte W32_MB32_H2O[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x23,0x31,0x45,0x19,0xFF,0xFF,0xFF,0xFF,0x70,0x16};
 byte W33_MB33_H2O[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x24,0x31,0x45,0x19,0xFF,0xFF,0xFF,0xFF,0x71,0x16};
 byte W34_MB34_H2O[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x25,0x31,0x45,0x19,0xFF,0xFF,0xFF,0xFF,0x72,0x16};                
 byte W35_MB35_H2O[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x26,0x31,0x45,0x19,0xFF,0xFF,0xFF,0xFF,0x73,0x16};   
                            
 byte W11_MB15_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x26,0x01,0x15,0x70,0xFF,0xFF,0xFF,0xFF,0x6A,0x16}; // Gampelutz 5 Heizung
 byte W12_MB16_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x27,0x01,0x15,0x70,0xFF,0xFF,0xFF,0xFF,0x6B,0x16};
 byte W13_MB17_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x28,0x01,0x15,0x70,0xFF,0xFF,0xFF,0xFF,0x6C,0x16};
 byte W14_MB18_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x29,0x01,0x15,0x70,0xFF,0xFF,0xFF,0xFF,0x6D,0x16};                

 byte W21_MB25_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x30,0x01,0x15,0x70,0xFF,0xFF,0xFF,0xFF,0x74,0x16};  // Gampelutz 7 Heizung
 byte W22_MB26_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x31,0x01,0x15,0x70,0xFF,0xFF,0xFF,0xFF,0x75,0x16};
 byte W23_MB27_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x32,0x01,0x15,0x70,0xFF,0xFF,0xFF,0xFF,0x76,0x16};
 byte W24_MB28_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x33,0x01,0x15,0x70,0xFF,0xFF,0xFF,0xFF,0x77,0x16};                

 byte W31_MB36_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x34,0x01,0x15,0x70,0xFF,0xFF,0xFF,0xFF,0x78,0x16};  // Galenburst 23 Heizung
 byte W32_MB37_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x35,0x01,0x15,0x70,0xFF,0xFF,0xFF,0xFF,0x79,0x16};
 byte W33_MB38_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x36,0x01,0x15,0x70,0xFF,0xFF,0xFF,0xFF,0x7A,0x16};
 byte W34_MB39_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x37,0x01,0x15,0x70,0xFF,0xFF,0xFF,0xFF,0x7B,0x16};
 byte W35_MB40_Heat[17] = {0x68,0x0B,0x0B,0x68,0x73,0xFD,0x52,0x38,0x01,0x15,0x70,0xFF,0xFF,0xFF,0xFF,0x7C,0x16};
#endif

MBus::MBus(int pin)
{
// to do
}

///=====================  9  ============================ 
//  Fordert Daten vom MBus-Gerät an
//=======================================================request_MBusSerialNumber
void MBus::request_MBusSerialNumber(int meterNumber){
  String requestMBusSerNum;
  switch (meterNumber){
// case 0: requestMBusSerNum = (char*)wasser;break;  //Serial.println("case 0"); break;
// case 1: requestMBusSerNum = (char*)waerme;break;  // Serial.println("case 1"); break;

 // Gapelutzstrasse 5
   case 0: requestMBusSerNum = (char*)W11_MB11_H2O;break;  // Serial.println("case 0"); break;
   case 1: requestMBusSerNum = (char*)W11_MB15_Heat;break;  // Serial.println("case 1"); break;
   case 2: requestMBusSerNum = (char*)W12_MB12_H2O;  break;  // Serial.println("case 2"); break;
   case 3: requestMBusSerNum = (char*)W12_MB16_Heat; break;  // Serial.println("case 3"); break;
   case 4: requestMBusSerNum = (char*)W13_MB13_H2O;  break;  // Serial.println("case 4"); break;
   case 5: requestMBusSerNum = (char*)W13_MB17_Heat; break;  // Serial.println("case 5"); break;
   case 6: requestMBusSerNum = (char*)W14_MB14_H2O;  break;  // Serial.println("case 6"); break;
   case 7: requestMBusSerNum = (char*)W14_MB18_Heat; break;  // Serial.println("case /"); break;

 
 // Gapelutzstrasse 7
   case 8: requestMBusSerNum = (char*)W21_MB21_H2O;  break;  // Serial.println("case 8"); break;
   case 9: requestMBusSerNum = (char*)W21_MB25_Heat; break;  // Serial.println("case 9"); break;
   case 10: requestMBusSerNum = (char*)W22_MB22_H2O; break;  // Serial.println("case 10"); break;
   case 11: requestMBusSerNum = (char*)W22_MB26_Heat;break;  // Serial.println("case 11"); break;
   case 12: requestMBusSerNum = (char*)W23_MB23_H2O; break;  // Serial.println("case 12"); break;
   case 13: requestMBusSerNum = (char*)W23_MB27_Heat;break;  // Serial.println("case 13"); break;
   case 14: requestMBusSerNum = (char*)W24_MB24_H2O; break;  // Serial.println("case 14"); break;
   case 15: requestMBusSerNum = (char*)W24_MB28_Heat;break;  // Serial.println("case 15"); break;

  // Galenburst 23
   case 16: requestMBusSerNum = (char*)W31_MB31_H2O; break;  // Serial.println("case 16"); break;
   case 17: requestMBusSerNum = (char*)W31_MB36_Heat;break;  // Serial.println("case 17"); break;
   case 18: requestMBusSerNum = (char*)W32_MB32_H2O; break;  // Serial.println("case 18"); break;
   case 19: requestMBusSerNum = (char*)W32_MB37_Heat;break;  // Serial.println("case 19"); break;
   case 20: requestMBusSerNum = (char*)W33_MB33_H2O; break;  // Serial.println("case 20"); break;
   case 21: requestMBusSerNum = (char*)W33_MB38_Heat;break;  // Serial.println("case 21"); break;
   case 22: requestMBusSerNum = (char*)W34_MB34_H2O; break;  // Serial.println("case 22"); break;
   case 23: requestMBusSerNum = (char*)W34_MB39_Heat;break;  // Serial.println("case 23"); break;
   case 24: requestMBusSerNum = (char*)W35_MB35_H2O; break;  // Serial.println("case 24"); break;
   case 25: requestMBusSerNum = (char*)W35_MB40_Heat;break;  // Serial.println("case 25"); break;
   default: //Serial.println("default"); 
   break;
  }
  
  delay(200);
 //  requestMBusSerNum = (char*)adrMBus2; 
  Serial.print(requestMBusSerNum.substring(0,17));
  delay(500);
  
  String requestCommand = (char*)sendSerNrRequest;
  Serial.print(requestCommand.substring(0,5));
  delay(500);
 // Serial.print("\n");
//  delay(500);
}

///===================  1  ============================== 
// Seriennummer aus String suchen
//======================================================= 
void searchSerNr(String strMBusData, String meterType){
   int iPos = strMBusData.indexOf("6808")+8; 
   String  strRet = strMBusData.substring(iPos+6, iPos+8) + 
                    strMBusData.substring(iPos+4, iPos+6) + 
                    strMBusData.substring(iPos+2, iPos+4) + 
                    strMBusData.substring(iPos+0, iPos+2); 
   Serial.print("SerNr "+meterType);Serial.print(strRet);
} 

//======================  0  ============================
//   search MBus-Register
//=======================================================
void MBus::search_MBusParameter(String strMBusData, int nMeter, word dataArray[], String flats[]){
  String sBusAddr;

//------- MBusAdresse ----------"); 
  sBusAddr = getMBusAddress(strMBusData);   // -- 2 --
  
//------- MBusStatus -----------"); 
  sMBus_State[nMeter] = getMBusState(strMBusData);   // -- 3 --
 
//------- Medium ---------------"); 
  medium = getMedium(strMBusData) ;    // -- 4 --

char buf[30];
//------------------ H2O Wasser ------------------------- 
  if ((medium == "<07> Cold Water")||(medium == "<06> Hot Water")){
   //"------- Seriennummer -----------------------------------------"); 
    searchSerNr(strMBusData, "Wasser ");
 
   //"------- Zählerstand ---------");     
    v2s = getMeterValue(strMBusData, 0, "0413")+ "\n";   // -- 5 --
    long res = calcHexToLong(v2s); 
      
    dataArray[2*nMeter] = res/10000;    // oberer Zählerstand
    dataArray[2*nMeter+1] = res%10000;  // unterer Zählerstand
  }  
// -------------------------- HEAT --------------------------- 
  else if (medium == "<04> Heat"){ 

    //"------- Seriennummer ---------");
    searchSerNr(strMBusData,"Heizung ");

     //"------- Arbeit kWh  ---"); 
    String strTemp = getMeterValue(strMBusData, 0,"0C06"); 

    int lHeat = strTemp.toInt();
    dataArray[2*nMeter] = word(lHeat)/10000;    // oberer Zählerstand
    dataArray[2*nMeter+1] = word(lHeat)%10000;  // unterer Zählerstand
 /* 
     v2s = getMeterValue(strMBusData, 0,"0F72");   // -- 5 -- //OK  22.03.2020
     Serial.println(v2s);
     fMBus_Address[nMeter] =v2s.toInt() / 1.0;
     delay(iDelay); 
 
//"------- Flow Temp °C ---------"); 
     String strTemp;
     float fTemp1 = con_6digBCD2Float(strMBusData,38,"0B5A");   // -- 7 --   //OK  22.03.2020
      Serial.println(fTemp1);
      fMBus_tr[nMeter] = fTemp1;//convertIEEE754toFloat(strTemp); 
      fMBus_Messung_2[nMeter][T_VORLAUF] = fTemp1 + fMBusApp;//fMBus_tFlow[nMeter];
      delay(iDelay);
 
//"------- Return Temp °C -------"); 
   // L&G T330    Serial.println(fTemp1);
      float fTemp2 = con_6digBCD2Float(strMBusData,38,"0B5E");   // -- 7 -- //OK  22.03.2020
      Serial.println(fTemp2);
      fMBus_tr[nMeter] = fTemp2;//convertIEEE754toFloat(strTemp); 
      fMBus_Messung_2[nMeter][T_RUECKLAUF] = fTemp2 + fMBusApp;//fMBus_tr[nMeter];
      delay(iDelay);  
 
//"------- Volume Flow m^3 --------------"); 
   // L&G T330
      float fTemp3 =con_6digBCD2Float(strMBusData,38,"0B3E");   // -- 7 --
      fMBus_tr[nMeter] = fTemp3;//convertIEEE754toFloat(strTemp);    // -- 8 --
      fMBus_Messung_2[nMeter][VOLUMEN_FLOW] = fTemp3;//fMBus_vFlow[nMeter];
       
//"------- Flow extern m^3/min --"); 
      strTemp = getMeterValue(strMBusData, 38,"0407");    // -- 5 --
      fMBus_FlowEx[nMeter] = convertIEEE754toFloat(strTemp);   // -- 8 --
      fMBus_Messung_2[nMeter][FLOW_EXTERN] = fMBus_FlowEx[nMeter];
       
//"------- Flow m^3 10^-3 ---------------");     
      strTemp = getMeterValue(strMBusData, 38,"0413");    // -- 5 --
      fMBus_Flow[nMeter] = convertIEEE754toFloat(strTemp);   // -- 8 --
      fMBus_Messung_2[nMeter][FLOW] = fMBus_Flow[nMeter];

//"------- Power  W------"); 
      strTemp = (getMeterValue(strMBusData, 0,"052D"));    // -- 5 -- //OK  22.03.2020
      fMBus_P[nMeter] = convertIEEE754toFloat(strTemp);   // -- 8 --
      fMBus_Messung_2[nMeter][LEISTUNG]=  fMBus_P[nMeter];
    */
    }
    sprintf(buf,"  Nr: %d, Wert = %d %d",nMeter, dataArray[2*nMeter], dataArray[2*nMeter+1]);
    Serial.println(buf);
  
}

long MBus::calcHexToLong(String str){
  String s1 = str.substring(0,4);
  long ls1 = strtol(s1.c_str(),NULL,16);
  String s2 = str.substring(4,8);
  long ls2 = strtol(s2.c_str(),NULL,16);
  long result = ls1 * 65535 + ls2;
  return result;
}
///===================  1  ============================== 
// Konvertiert die Hex-Zahl in Stringformat
//======================================================= 
long MBus::convertHEX2String(String hex){
  long m = 0;
/*  char msg[8];
  hex.toCharArray(msg, 9);

  char temp[3];
  for (int i = 0; i < 8; i += 1)
  {
    strncpy(temp, &msg[i], 2);
    // Convert hex string to numeric:
    m *= 16;
    m += (temp[0] <= '9') ? (temp[0] - '0') : (temp[0] - 'A' + 10);
  }
  */
  return m;
}

///=====================  2  ============================ 
//  MBus Adresse
//======================================================= 
  String MBus::getMBusAddress(String strData) { 
  String s = strData.substring(10,12);
  char buf[2];
  s.toCharArray(buf, 2); 
  return strData.substring(10,12); 
} 

//====================== 3  =============================
// MBus State
//======================================================= 
String MBus::getMBusState(String strData) { 
  return strData.substring(32,34); 
} 

  ///====================  4  =========================== 
//  Medium 07=Water, 06=Gas, 04=Heat
//======================================================= 
String MBus::getMedium(String strData) { 
  String strMedium = "???"; 
  int iPos = strData.indexOf("6808")+22;  
   
 //Serial.print("medium ");
  strMedium = strData.substring(iPos, iPos+2); 

  if ((strMedium == "97") ||(strMedium == "04")) //04
    strMedium = "<04> Heat"; 
  
  else if (strMedium == "06") 
      strMedium = "<06> Hot Water"; 
  
  else if (strMedium == "07") 
      strMedium = "<07> Cold Water"; 
  
  Serial.println(strMedium);
  return strMedium; 
} 

//======================  5  ============================= 
//Liesst den Zählerstand 
//example 0C13  00112233 
// Bedingung: 
// String muss ab der Stringposition "iPos" den Code "strSearch" enthalten 
//======================================================= 
String MBus::getMeterValue(String strData, int iPos_A, String strSearch){ 
  delay(100);
  String strRet="9999999";
  int iPos = strData.indexOf(strSearch); 
  if (iPos > 0){
    String strValue = strData.substring(iPos, iPos+12); 
    if (iPos > 9){ 
      strRet = strValue.substring(10, 12) + strValue.substring(8, 10) + strValue.substring(6, 8) + strValue.substring(4, 6); 
    } 
  }
  return strRet; 
}

  
///====================  6  ============================= 
//  Converts 2 Ascii Char into a Integernumber
//=======================================================  
int MBus::MBus2Int(String str){
  int i = 0;
  int summe = 0;
  char ch = str.charAt(0);
  i = ch-'0';
  if (i > 9)
    i -= 7;
  summe = i *16; 
    
  ch = str.charAt(1);
  i = ch-'0';
  if (i > 9)
     i -= 7;
  summe += i;   
  return summe;
 }


  //*****************************************************************************
///===================  7  ============================== 
//  Convertiert 8digit BCD auf float 
//=======================================================

float MBus::con_6digBCD2Float(String strData, int iPost, String strSearch){
  float fRet;
  String sRet;
  int iPos = strData.indexOf(strSearch); 

  if (iPos > 20){
    String strValue = strData.substring(iPos+4, iPos+10); 
    sRet = strValue.substring(4, 6) + strValue.substring(2, 4) + strValue.substring(0, 2); 
  } else
  {
      sRet = "000000";  
  }
  fRet = sRet.toFloat() / 10.0;
  return fRet; 
  }

  ///===================  8  ============================ 
//https://www.h-schmidt.net/FloatConverter/IEEE754de.html 
// Convert auf Realdarstellung 
//======================================================= 
float MBus::convertIEEE754toFloat(String str){ 
// Prepare the character array (the buffer) 
  char char_array[9]; 
//  Copy it over 
  str.toCharArray(char_array, 9); 
  String ret; 
  unsigned long x = strtoul( char_array, nullptr, 16); 
  float y = *(float*)&x; 
  return y; 
} 



///=====================  9  ============================ 
//  Fordert Daten vom MBus-Gerät an
//=======================================================
void MBus::request_MBusData(int iMBusNum){
  req_MBus_data[2] = char(iMBusNum); //0x01; 
  req_MBus_data[3] = char(0x7b + iMBusNum); 
 
// Anforderung 
  String requestMBusString = (char*)req_MBus_data; 
  
// visuelle Kontrolle 
  for(int n =0; n<5;n++)
     Serial.print(hextoDoubleChar(req_MBus_data[n]));  //-- 11 --
  Serial.println("\n");
  
  delay(500);
  Serial.print(requestMBusString); // Datenauslesung des MBus-Registers anstossen
  delay(500);   
}

// =====================  10  ============================= 
// Empfang der MBuszählerdaten
//======================================================= 
String MBus::receive_MBusData() { 
  String readStringw = ""; 
 delay(500);
  while (Serial.available()) { 
    if (Serial.available() > 0) { 
       readStringw += hextoDoubleChar(Serial.read()); //-- 11 --
       delay(10);   
    } 
    delay(20);
   } 
  return readStringw; 
}

//=======================  11  =========================== 
// Example 0x1A   ==> 0x3141 ==> "1A" 
//======================================================= 
String MBus::hextoDoubleChar(char c) { 
  String ret; 
  char b  = ((c >> 4) & 0x0F) + 0x30; // Hex numbers 0-9 
  if (b > '9')                       // Hex Character A-F 
    b  += 7; 
  ret = b; // Makes the string readString 
  // lower nipple 
  b  = (c & 0x0F) + 0x30; 
  if (b > '9') 
    b  += 7; 
  ret += b; 
  return ret; 
} 
