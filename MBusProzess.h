/*
  Released into the public domain.
*/

#include "MBus.h"
#ifndef MBusProzess_h
#define MBusProzess_h

class MBusProzess
{
  public:

 static  String mBusDataString[8];
  
  //constructor
  MBusProzess(int num);
  static void setupMBus(void);
  static void getMBusData(word dataArray[]);
  static void get_ThingSpeakData(word dataArray[], int anzahlMeter);
  static void writeThinkSpeak(String sApiKey, String sPost);
  static String getFunkAdr(String strData);
    
  private:

};

#endif //MBusProzess_h
