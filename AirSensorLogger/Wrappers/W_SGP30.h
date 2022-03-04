//-----------------------------------------------------------------------------
// SGP30 wrapper - Ethanol, H2 -> TVOC, CO2eq
//-----------------------------------------------------------------------------
// SGP30 I2C_ADDR 0x58
//-----------------------------------------------------------------------------

#include "Sensors/SparkFun_SGP30.h"
// -- todo --

class SGP30_Wrapper : public TSensorWrapper {

public:
   bool Active;
   SGP30 Sensor;
//   int Temperature100;  // Temperature in degrees C * 100
//   int Humidity100;     // Temperature in percent * 100 (2 decimals, eg 5912 => 59.12%)

   void Setup()
   {
      if (Sensor.begin() == false) {
         printf("Sensor %s not detected, check connections.", Name);
         return;
      }

      //Initializes sensor for air quality readings
      //measureAirQuality should be called in one second increments after a call to initAirQuality
      Sensor.initAirQuality();


//First fifteen readings will be
//CO2: 400 ppm  TVOC: 0 ppb

      // Setup succes
      printf("SGP30 setup succes\n");
      Active = true;
   }

   bool Read()
   {
      if (!Active) return false; // no data read

      //Temperature100 = 100 * Sensor.readTemperature();
      //Humidity100 = 100 * Sensor.readHumidity();


//  uint16_t CO2;
//  uint16_t TVOC;
//  uint16_t baselineCO2;
//  uint16_t baselineTVOC;
//  uint16_t featureSetVersion;
//  uint16_t H2;
//  uint16_t ethanol;
//  uint64_t serialID;

//>    //measure CO2 and TVOC levels
//>    mySensor.measureAirQuality();
//>    Serial.print("CO2: ");
//>    Serial.print(mySensor.CO2);
//>    Serial.print(" ppm\tTVOC: ");
//>    Serial.print(mySensor.TVOC);
//>    Serial.println(" ppb");
//>    //get raw values for H2 and Ethanol
//>    mySensor.measureRawSignals();
//>    Serial.print("Raw H2: ");
//>    Serial.print(mySensor.H2);
//>    Serial.print(" \tRaw Ethanol: ");
//>    Serial.println(mySensor.ethanol);


      return true;   // no error handling, so assume all went well
   }

   void Dump(bool Full=false)
   {
//      printf("HDC1080 Active: %d, Humidity: %s %%\n",
//            Active, FixedPoint100ToStr(Humidity100));
//      printf("   Temperature %s C\n", FixedPoint100ToStr(Temperature100));
//
//      if (Full && Active) {
//
//         printf("   Manufacturer ID=0x%x\n", Sensor.readManufacturerId());   // 0x5449 ID of Texas Instruments
//         printf("   Device ID=0x%x\n", Sensor.readDeviceId()); // 0x1050 ID of the device
//
//         HDC1080_SerialNumber sernum = Sensor.readSerialNumber(); // union with 6 byte array
//         printf("   Device Serial Number=%02X-%04X-%04X\n",
//               sernum.serialFirst, sernum.serialMid, sernum.serialLast);
//
//      }
   }
};


