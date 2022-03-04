//-----------------------------------------------------------------------------
// .ino
//-----------------------------------------------------------------------------

#define MAIN

//-----------------------------------------------------------------------------
// Command interpreter & i2c toolkit
#define MyPrintf printf // works for (at least) AVR & ESP32
#include <Wire.h>
#include "Commands.h"
void MyExecute(int Param[]); // cascades void TkExecute(int Param[]);
TCommand Command(MyExecute);
#define I2CMTK_INTERACTIVE
#include "I2CmTk.h"

//----------------------------------------------------------------------------------
#include "WiFi.h"
#include "C:\MyRobot\Arduino\SketchbookSTM32\credentials.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
WiFiClient client;

//----------------------------------------------------------------------------------
// Sensor stuff
#include "Wrappers/SensorWrappers.h"

TSensorList SensorList(20);

bool WifiEnable = true;
bool SensorTaktEnable = true;

//#include "AppKanaal1.h"
#include "AppTest.h"

//-----------------------------------------------------------------------------
// Setup -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void setup()
{
   Serial.begin(115200);
   printf("\n\n---------------------------------------------\nEnvLogger\n");

   RegisterSensors();  // App specific, call this early to adjust 'other' settings.

   if (WifiEnable) {
      connectWiFi();
      ThingSpeak.begin(client);  // Initialize connection to ThingSpeak
   } else {
      printf("Wifi disabled, skip wifi setup.\n");
   }

   //--------------------------------------------------------------------------
//   Wire.begin(21, 22);  // Initialize I2C Hardware -- TTGO V1.1 pins
   Wire.begin(14, 15);  // Initialize I2C Hardware - WROOM pins

   SensorList.SetupSensors();
   SensorList.ReadSensors();
   SensorList.DumpSensors(true);
}

//-----------------------------------------------------------------------------
// loop -
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void loop()
{ static int NextReadTakt, NextCommitTakt;

   Command.Takt(Serial);

   int ms = millis();

   if (SensorTaktEnable) {
      if ((ms - NextReadTakt) > 0) {
         NextReadTakt = ms + MeasureInterval;  // time for next interval
         printf("----------------------------------------------\n");
         SensorList.ReadSensors();
         SensorList.DumpSensors();
      }

      if ((ms - NextCommitTakt) > 0) {
         NextCommitTakt = ms + CommitInterval;  // time for next interval

         // Commit data if all sensors are stable
         if (SensorList.AreAllStable()) {
            printf("**********************************************\n");
            TsCommit();
         }

         // Clear sensor data to restart average calculation.
         //    Required even when not commiting, since this also triggers
         //    'Stable'-check. Stable-check (only) at commit-time assures
         //    an average of a full commit-interval...
         SensorList.ClearSensors();
      }
   }
}

//-----------------------------------------------------------------------------
// connectWifi - Attempt to connect to WiFi
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
void connectWiFi() {

   WiFi.mode(WIFI_STA);
   WiFi.disconnect();
   delay(100);

  byte led_status = 0;

  WiFi.mode(WIFI_STA);  // Set WiFi mode to station (client)
  WiFi.begin(ssid, password);  // Initiate connection with SSID and PSK

  // Signal LED / dots while we wait for WiFi connection
  while ( WiFi.status() != WL_CONNECTED ) {
//    digitalWrite(LED_PIN, led_status);
//    led_status ^= 0x01;
   Serial.print(".");
    delay(100);
  }

   // Use serial/print to properly print WiFi.LocalIP() result.
   Serial.printf("\nWifi connected, IP: ");
   Serial.println(WiFi.localIP());
   Serial.print("Wifi Setup done.\n");

//  // Turn LED off when we are connected
//  digitalWrite(LED_PIN, HIGH);
}

//-----------------------------------------------------------------------------
// MyExecute - execute commando
//-----------------------------------------------------------------------------
// Called via CmdTakt() when a command is received from the serial port.
//-----------------------------------------------------------------------------
void MyExecute(int Param[])
{
   if (Command.Match("?",              0)) Command.Help("EnvLogger + I2c toolkit command parser.");

   TkExecute(Param); // cascade

   if (Command.Match("SensorTaktEnable",      1)) SensorTaktEnable = Param[0];
}