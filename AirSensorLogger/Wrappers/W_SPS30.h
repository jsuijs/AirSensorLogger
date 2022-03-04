//-----------------------------------------------------------------------------
// SPS30 wrapper - Particulate Matter (PM)
//-----------------------------------------------------------------------------
// SPS30 I2C_ADDR: 0x69
//-----------------------------------------------------------------------------
#include "Sensors/sps30/sps30.h"

class SPS30_Wrapper : public TSensorWrapper {

public:

   SPS30_Wrapper(const char *InName) : TSensorWrapper(InName) {}

   struct sps30_measurement m;
   TAverage PM_1p0;
   TAverage PM_2p5;
   TAverage PM_10p0;

   const uint8_t auto_clean_days = 4;

   void Setup()
   {
      if (sps30_probe() != 0) {
         Serial.print("SPS30 setup failed: sensor probing failed\n");
         return;
      }

      int16_t ret = sps30_set_fan_auto_cleaning_interval_days(auto_clean_days);
      if (ret) {
         printf("SPS30 setup failed: error setting the auto-clean interval: %d\n", ret);
         return;
      }

      ret = sps30_start_measurement();
      if (ret < 0) {
         printf("SPS30 setup failed: error starting measurement\n");
         return;
      }

      #ifdef SPS30_LIMITED_I2C_BUFFER_SIZE
         Serial.print("Your Arduino hardware has a limitation that only\n");
         Serial.print("  allows reading the mass concentrations. For more\n");
         Serial.print("  information, please check\n");
         Serial.print("  https://github.com/Sensirion/arduino-sps#esp8266-partial-legacy-support\n");
         Serial.print("\n");
      #endif

      SetupSucces();
   }

   void Clear()
   {
      UpdateStable();

      PM_1p0.Clear();
      PM_2p5.Clear();
      PM_10p0.Clear();
   }

   bool Read()
   {
      if (IsReadEnabled() == false) return false; // no data read

      uint16_t data_ready;
      int16_t ret;

      ret = sps30_read_data_ready(&data_ready);
      if (ret < 0) {
         printf("SPS30 error reading data-ready flag: %d\n", ret);
         return false;
      }

      if (!data_ready) {
         printf("SPS30: data not ready, no new measurement available\n");
         return false;
      }

      ret = sps30_read_measurement(&m);
      if (ret < 0) {
         printf("SPS30 error reading measurement\n");
         return false;
      }

      PM_1p0.Add(m.mc_1p0);
      PM_2p5.Add(m.mc_2p5);
      PM_10p0.Add(m.mc_10p0);
      return true;   // all went well
   }

   void Dump(bool Full=false)
   {
      printf("SPS30 Active: %d\n", IsActive());

      printf("PM  1.0: %f\n", m.mc_1p0);
      printf("PM  2.5: %f\n", m.mc_2p5);
      printf("PM  4.0: %f\n", m.mc_4p0);
      printf("PM 10.0: %f\n", m.mc_10p0);

      printf("NC  0.5: %f\n", m.nc_0p5);
      printf("NC  1.0: %f\n", m.nc_1p0);
      printf("NC  2.5: %f\n", m.nc_2p5);
      printf("NC  4.0: %f\n", m.nc_4p0);
      printf("NC 10.0: %f\n", m.nc_10p0);

      printf("Typical partical size: %f\n", m.typical_particle_size);

      if (Full && IsActive()) {
//         printf("   Temperature %s C\n", FixedPoint100ToStr(Temperature100));
      }
   }
};
