//-----------------------------------------------------------------------------
// AppTest.h
//-----------------------------------------------------------------------------

#define NO_WIFI

const int MeasureInterval =  2 * 1000;
const int CommitInterval  = 10 * 1000;

HTU21D_Wrapper    SensorHTU21D("HTU21D");
SCD30_Wrapper     SensorSCD30("SCD30");

void RegisterSensors()
{
   SensorList.Add(SensorHTU21D);
   SensorList.Add(SensorSCD30);

   WifiEnable = false;
}

bool TsCommit()
{
   return false;
}
