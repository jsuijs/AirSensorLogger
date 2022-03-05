//-----------------------------------------------------------------------------
// AppTest.h
//-----------------------------------------------------------------------------

const int MeasureInterval =  2 * 1000;
const int CommitInterval  = 10 * 1000;

HTU21D_Wrapper    SensorHTU21D("HTU21D");
SCD30_Wrapper     SensorSCD30("SCD30");

void SetupSensors()
{
   // Add sensors to list
   SensorList.Add(SensorHTU21D);
   SensorList.Add(SensorSCD30);

   // Call setup of sensors in list
   SensorList.SetupSensors();

   // Post-configuration (sensors & general)
   WifiEnable = false;
}

bool TsCommit()
{
   return false;
}
