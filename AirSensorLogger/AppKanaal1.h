//-----------------------------------------------------------------------------
// AppKanaal1.h - code for specific sensors & for commit
//-----------------------------------------------------------------------------

const int MeasureInterval =  2 * 1000;
const int CommitInterval  = 10 * 1000;

CCS811_Wrapper    SensorCCS811("CCS811");
HDC1080_Wrapper   SensorHDC1080("HDC1080-1");
HTU21D_Wrapper    SensorHTU21D("HTU21D");
MCP9808_Wrapper   SensorMCP9808("MCP9808");
MHZ19_Wrapper     SensorMHZ19("MHZ19");
SPS30_Wrapper     SensorSPS30("SPS30");

void SetupSensors()
{
   // Add sensors to list
   SensorList.Add(SensorCCS811);
   SensorList.Add(SensorHDC1080);
   SensorList.Add(SensorHTU21D);
   SensorList.Add(SensorMCP9808);
   SensorList.Add(SensorMHZ19);
   SensorList.Add(SensorSPS30);

   // Call setup of sensors in list
   SensorList.SetupSensors();

   // Post-configuration (sensors & general)

}

//-----------------------------------------------------------------------------
// TsCommit - Commit data to ThingSpeak channel
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
bool TsCommit()
{  int r = 999;

   printf("SPS30 commit values: %f %f %f\n",
      SensorSPS30.PM_1p0.Get(), SensorSPS30.PM_2p5.Get(), SensorSPS30.PM_10p0.Get());

   ThingSpeak.setField(1, SensorHDC1080.Humidity.Get());
   ThingSpeak.setField(2, SensorMHZ19.CO2);
   ThingSpeak.setField(3, 0 );
   ThingSpeak.setField(4, SensorMCP9808.Temperature100 / (float) 100.0);

   ThingSpeak.setField(5, 0);

   ThingSpeak.setField(6, SensorSPS30.PM_1p0.Get());
   ThingSpeak.setField(7, SensorSPS30.PM_2p5.Get());
   ThingSpeak.setField(8, SensorSPS30.PM_10p0.Get());

   int r = ThingSpeak.writeFields(CHANNEL1_ID, CHANNEL1_WRITE_API_KEY);

   if (r == 200) {
      printf("Channel update successful.\n");
      return true;
   }

   if (r == 999) {
      printf("Channel update uitgeschakeld\n");
   } else {
      printf("Channel update error %d.\n", r);
   }
   return false;  // update error)
}

