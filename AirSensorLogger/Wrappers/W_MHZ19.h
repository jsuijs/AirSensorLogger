
//-----------------------------------------------------------------------------
// MHZ19 wrapper - CO2
//-----------------------------------------------------------------------------
// MHZ19 I2C_ADDR: none (serial)
//-----------------------------------------------------------------------------

#include "Sensors/MHZ19.h"

class MHZ19_Wrapper : public TSensorWrapper {

public:

   MHZ19_Wrapper(const char *InName) : TSensorWrapper(InName) {}

   MHZ19 Sensor;
   int Temperature100;  // Temperature in degrees C * 100 (note *100 is for compatibility; resolution is in degrees)
   int CO2;             // CO2 level in ppm

   #define MHZ19_Port       Serial1    // Port used
   const int MHZ19_RX_PIN   =   25;    // Rx pin which the MHZ19 Tx pin is attached to
   const int MHZ19_TX_PIN   =   26;    // Tx pin which the MHZ19 Rx pin is attached to
   const int MHZ19_BAUDRATE = 9600;    // Device to MH-Z19 Serial baudrate (should not be changed)

   void Setup()
   {

      // ESP32 specific (well... at least specifying pins doesn't work an Uno)
      MHZ19_Port.begin(MHZ19_BAUDRATE, SERIAL_8N1, MHZ19_RX_PIN, MHZ19_TX_PIN) ;

      Sensor.begin(MHZ19_Port);  // connect sensor to serial1 stream

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

      CO2 = Sensor.getCO2();
      Temperature100 = 100 * Sensor.getTemperature();

      return true;   // no error handling, so assume all went well
   }

   void Dump(bool Full=false)
   {
      printf("MHZ19 Active: %d, CO2: %d ppm\n", IsActive(), CO2);

      if (Full && IsActive()) {
         printf("   Temperature %s C\n", FixedPoint100ToStr(Temperature100));
      }
   }
};
