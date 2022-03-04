
//-----------------------------------------------------------------------------
// MCP9808 wrapper - temperature sensor
//-----------------------------------------------------------------------------
// MCP9808 I2C_ADDR 0x18 (base);
// Notes:
// * Library can't just read or write the required data, it's all or nothing.
//-----------------------------------------------------------------------------
#include "Sensors/MCP9808.h"      // http://github.com/JChristensen/MCP9808

class MCP9808_Wrapper  : public TSensorWrapper {

public:

   MCP9808_Wrapper(const char *InName) : TSensorWrapper(InName) {}

   MCP9808 Sensor;

   int Temperature100;  // Temperature in degrees C * 100

   void Setup()
   {
      delay(1000);   // time for the sensor to perform an initial temperature conversion

      // Initialize the hardware
      uint8_t status = Sensor.begin();
      if ( status != 0 ) {
         printf("Error reading %s during setup, status= %d, sensor disabled.\n", Name, status);
         return;
      }

      // Configure
      Sensor.tUpper = 25 * 4;          // 25C (77F)
      Sensor.tLower = -10 * 4;         // -10C (14F)
      Sensor.tCritical = 30 * 4;       // 30C (86F)
      Sensor.write();                  // write the changed values

      SetupSucces();
   }

   void Clear()
   {
      UpdateStable();

//      Temperature.Clear();
//      Humidity.Clear();
   }

   bool Read()
   {
      if (IsReadEnabled() == false) return false; // no data read

      uint8_t  status = Sensor.read();
      if (status == 0 ) {
         Temperature100 = Sensor.tAmbient * 100.0 / 16;
         return true;
      }

      // Other status => report
      printf("Error reading MCP9808, status=%d\n", status);
      return false;
   }

   void Dump(bool Full=false)
   {
      printf("MCP9808 Active: %d, Temperature: %s C\n",
            IsActive(), FixedPoint100ToStr(Temperature100));

      if (Full && IsActive()) {
         printf("   tUpper:    %s C\n", FixedPoint100ToStr(Sensor.tUpper    * 100.0 / 16));
         printf("   tLower:    %s C\n", FixedPoint100ToStr(Sensor.tLower    * 100.0 / 16));
         printf("   tCritical: %s C\n", FixedPoint100ToStr(Sensor.tCritical * 100.0 / 16));
         printf("   Config %x, Resolution: %d\n", Sensor.config, Sensor.resolution);
         printf("   Mfr Id %x, Device ID: %x, Rev: %x\n", Sensor.mfrID, Sensor.deviceID, Sensor.deviceRev);
      }
   }
};
