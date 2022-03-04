//-----------------------------------------------------------------------------
// HTU21D wrapper - Humidity (RH) & Temperature sensor
//-----------------------------------------------------------------------------
// HTU21D I2C_ADDR 0x40
//-----------------------------------------------------------------------------

#include "Sensors/SparkFunHTU21D.h"

class HTU21D_Wrapper : public TSensorWrapper {

public:
   HTU21D_Wrapper(const char *InName) : TSensorWrapper(InName) {}

   HTU21D Sensor;

   TAverage Temperature;   // Temperature in degrees C
   TAverage Humidity;      // Humidity in percent

   //--------------------------------------------------------------------------
   void Setup()
   {
      Sensor.begin();
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

      float Value;

      Value = Sensor.readHumidity();
      if (Value > 990) return false;   // error
      Humidity.Add(Value);

      Value = Sensor.readTemperature();
      if (Value > 990) return false;   // error
      Temperature.Add(Value);

      return true;   // Success
   }

   void show_yes_no(const char *prefix, int val)
   {
      if (val)  {
         printf("%s yes\n", prefix);
      } else {
         printf("%s no\n", prefix);
      }
   }

   //--------------------------------------------------------------------------
   void Dump(bool Full=false)
   {
      printf("%s Active: %d, Humidity: %.2f %%, Temperature %.2f C\n",
            Name, IsActive(), Humidity.Get(), Temperature.Get());

      if (Full && IsActive()) {
         byte reg = Sensor.readUserRegister();
         printf("Resolution (Humidity, Temperature): ");
         switch (reg & USER_REGISTER_RESOLUTION_MASK) {
            case USER_REGISTER_RESOLUTION_RH12_TEMP14: printf("12, 14\n"); break;
            case USER_REGISTER_RESOLUTION_RH8_TEMP12 : printf("8, 12\n");  break;
            case USER_REGISTER_RESOLUTION_RH10_TEMP13: printf("10, 13\n"); break;
            case USER_REGISTER_RESOLUTION_RH11_TEMP11: printf("11, 11\n"); break;
         }
         show_yes_no("End of battery: ", reg & USER_REGISTER_END_OF_BATTERY);
         show_yes_no("Heater enabled: ", reg & USER_REGISTER_HEATER_ENABLED);
         show_yes_no("Disable OTP reload: ", reg & USER_REGISTER_DISABLE_OTP_RELOAD);

      }
   }
};
