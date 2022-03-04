//-----------------------------------------------------------------------------
// CCS811 wrapper - VOC
//-----------------------------------------------------------------------------
// CCS811 I2C_ADDR 0x5A (0x5B)
//-----------------------------------------------------------------------------

#include "Sensors/SparkFunCCS811.h"

//#define CCS811_ADDR 0x5B   // Default I2C Address
#define CCS811_ADDR 0x5A   // Alternate I2C Address

class CCS811_Wrapper : public TSensorWrapper {

public:
   CCS811_Wrapper(const char *InName) : TSensorWrapper(InName), Sensor(CCS811_ADDR) {}

   CCS811 Sensor;

   TAverage TVOC;    // Total Volatile Organic Compounds in ppb (parts per billion)
   TAverage CO2;     // eCO2, not real CO2 measurement, in ppm

   //--------------------------------------------------------------------------
   void Setup()
   {
      if (Sensor.begin() == false) {
         printf("Sensor %s setup error.\n", Name);
         return;
      }

      SetupSucces();
   }

   //--------------------------------------------------------------------------
   void Clear()
   {
      UpdateStable();

      CO2.Clear();
      TVOC.Clear();
   }

   //--------------------------------------------------------------------------
   bool Read()
   {
      if (IsReadEnabled() == false) return false; // no data read

      if (Sensor.dataAvailable() == false) return false;

      // read sensor, calculate the results & add to average
      Sensor.readAlgorithmResults();
      CO2.Add(Sensor.getCO2());
      TVOC.Add(Sensor.getTVOC());

      return true;   // no error handling, so assume all went well
   }

   //--------------------------------------------------------------------------
   void Dump(bool Full=false)
   {
      printf("%s Active: %d, TVOC: %s ppb\n",
            Name, IsActive(), FixedPoint100ToStr(TVOC.Get() * 100));

      if (Full && IsActive()) {
         printf("   eCO2 %d ppm\n", (int) CO2.Get() );
      }
   }
};
