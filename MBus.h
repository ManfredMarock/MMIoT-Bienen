/*
  Released into the public domain.
*/

#include "Arduino.h"
#ifndef MBus_h
#define MBus_h

class MBus
{
  public:
  //constructor
    MBus(int pin);
    
  // functions  
    long convertHEX2String(String hex);
    void search_MBusParameter(String strMBusData, int nMeter, word dataArray[],String flats[]);
    String getMBusAddress(String strMBusData);
    String getMBusState(String strData);
    String getMedium(String strData);
    String getMeterValue(String strData, int iPos_A, String strSearch);
    int MBus2Int(String str);
    float con_6digBCD2Float(String strData, int iPost, String strSearch);
    float convertIEEE754toFloat(String str);
    long calcHexToLong(String str);
    void request_MBusData(int iMBusNum);
    void request_MBusSerialNumber(int meterNumber);
    String receive_MBusData();
    String hextoDoubleChar(char c);

    
//--------------------------------------------------------
 // gobal variable
    float fMBus_Messung_2[60][10];
 //--------------------------------------------------------
 
  private:
  // variable

    int iDelay = 500;
    int iMBus_MeterReading[50];
    float ffSerNr;
    String medium;
    String v2s;
    String sFunk;
    String  sMBus_State[50];
    String sFunkAdr;
    int iFunkAdr; 
};

#endif
