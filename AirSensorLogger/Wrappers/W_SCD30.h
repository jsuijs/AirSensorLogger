//-----------------------------------------------------------------------------
// SCD30 wrapper - CO2, Temperature & Humidity sensor
//-----------------------------------------------------------------------------
// SCD30 I2C_ADDR 0x61
//-----------------------------------------------------------------------------

#include "Sensors/SparkFun_SCD30.h"

class SCD30_Wrapper : public TSensorWrapper {

public:
   SCD30_Wrapper(const char *InName) : TSensorWrapper(InName) {}

   SCD30 Sensor;

   TAverage CO2;           // Temperature in ppm
   TAverage Temperature;   // Temperature in degrees C
   TAverage Humidity;      // Humidity in percent

   //--------------------------------------------------------------------------
   void Setup()
   {
      if (Sensor.begin() == false) {
         printf("Sensor %s not detected, check connections.", Name);
         return;
      }

      // The SCD30 has data ready every two seconds
// ToDo: setting for auto calibratie

//--------------------------------------------------------------------------
// SetOptions
//   // Change number of seconds between measurements: 2 to 1800 (30 minutes), stored in non-volatile memory of SCD30
//   airSensor.setMeasurementInterval(16);
//   delay(200); // required for proper reporting this setting
//
//   //Set altitude of the sensor in m, stored in non-volatile memory of SCD30
//   airSensor.setAltitudeCompensation(1600);
//
//   //Current ambient pressure in mBar: 700 to 1200, will overwrite altitude compensation
//   airSensor.setAmbientPressure(835);
//
//   //Optionally we can set temperature offset to 5°C, stored in non-volatile memory of SCD30
//   airSensor.setTemperatureOffset(5);

      SetupSucces();
   }
   //--------------------------------------------------------------------------
   void Clear()
   {
      UpdateStable();

      CO2.Clear();
      Humidity.Clear();
   }

   //--------------------------------------------------------------------------
   bool Read()
   {
      if (IsReadEnabled() == false) return false; // no data read


      if (Sensor.dataAvailable()) {
         CO2.Add(Sensor.getCO2());
         Temperature.Add(Sensor.getTemperature());
         Humidity.Add(Sensor.getHumidity());
         return true;   // Success
      }

      return false;  // no data read (available)
   }

   //--------------------------------------------------------------------------
   void Dump(bool Full=false)
   {
      printf("%s Active: %d, CO2: %d, Humidity: %.2f %%, Temperature %.2f C\n",
            Name, IsActive(), (int)CO2.Get(), Humidity.Get(), Temperature.Get());

      if (Full && IsActive()) {




//    uint16_t settingVal; // The settings will be returned in settingVal
//
//    if (airSensor.getForcedRecalibration(&settingVal) == true) // Get the setting
//    {
//      Serial.print("Forced recalibration factor (ppm) is ");
//      Serial.println(settingVal);
//    }
//    else
//    {
//      Serial.print("getForcedRecalibration failed! Freezing...");
//      while (1)
//        ; // Do nothing more
//    }
//
//    if (airSensor.getMeasurementInterval(&settingVal) == true) // Get the setting
//    {
//      Serial.print("Measurement interval (s) is ");
//      Serial.println(settingVal);
//    }
//    else
//    {
//      Serial.print("getMeasurementInterval failed! Freezing...");
//      while (1)
//        ; // Do nothing more
//    }
//
//    if (airSensor.getTemperatureOffset(&settingVal) == true) // Get the setting
//    {
//      Serial.print("Temperature offset (C) is ");
//      Serial.println(((float)settingVal) / 100.0, 2);
//    }
//    else
//    {
//      Serial.print("getTemperatureOffset failed! Freezing...");
//      while (1)
//        ; // Do nothing more
//    }
//
//    if (airSensor.getAltitudeCompensation(&settingVal) == true) // Get the setting
//    {
//      Serial.print("Altitude offset (m) is ");
//      Serial.println(settingVal);
//    }
//    else
//    {
//      Serial.print("getAltitudeCompensation failed! Freezing...");
//      while (1)
//        ; // Do nothing more
//    }
//
//    if (airSensor.getFirmwareVersion(&settingVal) == true) // Get the setting
//    {
//      Serial.print("Firmware version is 0x");
//      Serial.println(settingVal, HEX);
//    }
//    else
//    {
//      Serial.print("getFirmwareVersion! Freezing...");
//      while (1)
//        ; // Do nothing more
//    }
//
//    Serial.print("Auto calibration set to ");
//    if (airSensor.getAutoSelfCalibration() == true)
//        Serial.println("true");
//    else
//        Serial.println("false");
//
//





      }
   }
};
