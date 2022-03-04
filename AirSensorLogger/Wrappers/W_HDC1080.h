//-----------------------------------------------------------------------------
// HDC1080 wrapper - Humidity (RH) & Temperature sensor
//-----------------------------------------------------------------------------
// HDC1080 I2C_ADDR 0x40
// Notes:
// * ClosedCube_HDC1080 doesn't seem to have any error handling...
// * Temperature seems a bit high (+1C?)
// * Temperature increases when the CCS811 - on the same breakout board - is enabled.
//   This also affects humidity reading...
//-----------------------------------------------------------------------------

#include "Sensors/ClosedCube_HDC1080.h"

class HDC1080_Wrapper : public TSensorWrapper {

public:
   HDC1080_Wrapper(const char *InName) : TSensorWrapper(InName) {}

   ClosedCube_HDC1080 Sensor;

   TAverage Temperature;   // Temperature in degrees C
   TAverage Humidity;      // Humidity in percent

   //--------------------------------------------------------------------------
   void Setup()
   {
      // Default settings:
      //  - Heater off
      //  - 14 bit Temperature and Humidity Measurement Resolutions
      Sensor.begin(0x40);
      if (Sensor.readManufacturerId() == 0xFFFF ) {
         printf("Sensor %s setup error.\n", Name);
         return;
      }

      StabilizeDelay = 5000;
      SetupSucces();
   }

   //--------------------------------------------------------------------------
   void Clear()
   {
      UpdateStable();

      Temperature.Clear();
      Humidity.Clear();
   }

   //--------------------------------------------------------------------------
   bool Read()
   {
      if (IsReadEnabled() == false) return false; // no data read

      Humidity.Add(Sensor.readHumidity());
      Temperature.Add(Sensor.readTemperature());

      return true;   // no error handling, so assume all went well
   }

   //--------------------------------------------------------------------------
   void Dump(bool Full=false)
   {
      printf("%s Active: %d, Humidity: %.2f %%, Temperature %.2f C\n",
            Name, IsActive(), Humidity.Get(), Temperature.Get());

      if (Full && IsActive()) {
         printf("   Manufacturer ID=0x%x\n", Sensor.readManufacturerId());   // 0x5449 ID of Texas Instruments
         printf("   Device ID=0x%x\n", Sensor.readDeviceId()); // 0x1050 ID of the device

         HDC1080_SerialNumber sernum = Sensor.readSerialNumber(); // union with 6 byte array
         printf("   Device Serial Number=%02X-%04X-%04X\n",
               sernum.serialFirst, sernum.serialMid, sernum.serialLast);
      }
   }
};
