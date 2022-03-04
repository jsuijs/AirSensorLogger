//-----------------------------------------------------------------------------
// SensorWrapper.h
//-----------------------------------------------------------------------------

// quick hack for systems with printf lacking float support
const char *FixedPoint100ToStr(int In)
{  static char String[100];   // The real (dirty) hack: static so it can be used by caller...

   sprintf(String, "%d.%02d", In / 100, abs(In % 100));
   return String;
}

#define MyPrintf printf
#ifndef HEXDUMP_DEFINED
#define HEXDUMP_DEFINED
/*=====================================================================
 HexDump -
 ---------------------------------------------------------------------*/
// Parameters:
//    Data   - data to be dumped
//    Length - nr of bytes to be dumped
//    Offset - offset of address (from 0), displayed at the start of each line.
//-----------------------------------------------------------------------------
void HexDump( const void *Data, unsigned int Length, unsigned int Offset)
{
   unsigned char *data  = (unsigned char *)Data;
   unsigned int Track1  = 0;
   unsigned int Track2  = 0;
   // HEX part
   for (unsigned int Index=0; Index < Length; Index = Index+16) {
      MyPrintf( "%04x: ", Offset + Index );
      for(unsigned int j=0; j<16; j++) {
         if (Track1 < Length ) {
            MyPrintf( "%02x", data[ Index+j ] );
         } else {
            MyPrintf( "  " );
         }
         MyPrintf( " " );
         Track1++;
      }
      MyPrintf( " "  );
      // ASCII part
      for(unsigned int j=0; j<16; j++) {
         if (Track2 < Length) {
            if (data[Index+j] < 32 ) {
               MyPrintf( "." );
            } else {
               if (data[Index+j] < 127) MyPrintf("%c", data[Index+j]);
               else MyPrintf(".");
            }
         }
         else MyPrintf( " " );
         Track2++;
      }
      MyPrintf( "\n" );
   }
}
//-----------------------------------------------------------------------------
// HexDump - Dump Data in hex format
//-----------------------------------------------------------------------------
// No offset, so address displayed at the start of each line starts at 0.
//-----------------------------------------------------------------------------
void HexDump(const void *Data, int Length)
{
   HexDump(Data, Length, 0);
}
#endif

//-----------------------------------------------------------------------------
// TAverage - Collect samples and provide average on request.
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class TAverage {

public:
   TAverage() { Clear(); }

   //--------------------------------------------------------------------------
   void Clear() {
      Sum = 0;
      Count = 0;
   }

   //--------------------------------------------------------------------------
   void Add(float InValue) {
      Sum += InValue;
      Count ++;
   }

   //--------------------------------------------------------------------------
   float Get() {
      if (Count == 0) return 0.0;
      return Sum / Count;
   }

private:
   float Sum;
   int Count;
};

//-----------------------------------------------------------------------------
// TSensorWrapper - master-class to wrap sensors
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class TSensorWrapper {
public:

   virtual void Setup() = 0;
   virtual bool Read() = 0;
   virtual void Clear() = 0;
   virtual void Dump(bool Full=false) = 0;

   bool IsActive() { return Active; }
   bool IsStable() { return Stable; }

   long StabilizeDelay;  // ms before using readings
   const char *Name;

   //--------------------------------------------------------------------------
   TSensorWrapper(const char *InName)
   {
      Name = InName;
      Active         = false;

      Stable         = false;
      StabilizeDelay = 0;
      StartMillis    = millis();
   }

   //--------------------------------------------------------------------------
   void SetupSucces()
   {
      printf("Setup succes of sensor %s\n", Name);
      Active = true;
   }

   //--------------------------------------------------------------------------
   // Update Stable flag - call on clear
   void UpdateStable()
   {
      if (Stable) return;
      long Now    = millis();
      long Target = StartMillis + StabilizeDelay;
      long Delta  = Now - Target;
      if (Delta > 0) Stable = true;
   }

   //--------------------------------------------------------------------------
   // Check if we're ready to read sensor
   bool IsReadEnabled()
   {
      if (Active == false) return false; // no data read
      return true; // ready to sample sensor
   }

private:
   bool Active;
   bool Stable;
   long StartMillis;
};

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class TSensorList {
public:

   //--------------------------------------------------------------------------
   TSensorList(int InLen)
   {
      ListLen  = 0;
      ListIx   = 0;
      Sensors  = (TSensorWrapper **) malloc(sizeof(TSensorWrapper *) * InLen);

      if (Sensors != NULL) ListLen = InLen;
   }

   //--------------------------------------------------------------------------
   void Add(TSensorWrapper &In)
   {
      if (ListIx < ListLen) {
         Sensors[ListIx] = &In;
         printf("Sensor %d - %s\n", ListIx, In.Name);
         ListIx ++;
      }
   }

   //--------------------------------------------------------------------------
   void SetupSensors()
   {
      for (int i=0; i<ListIx; i++) Sensors[i]->Setup();
   }

   //--------------------------------------------------------------------------
   void ReadSensors()
   {
      for (int i=0; i<ListIx; i++) Sensors[i]->Read();
   }

   //--------------------------------------------------------------------------
   void ClearSensors()
   {
      for (int i=0; i<ListIx; i++) Sensors[i]->Clear();
   }

   //--------------------------------------------------------------------------
   void DumpSensors(bool Full=false)
   {
      for (int i=0; i<ListIx; i++) Sensors[i]->Dump(Full);
   }

   //--------------------------------------------------------------------------
   bool AreAllStable()
   {
      for (int i=0; i<ListIx; i++) {
         if (Sensors[i]->IsActive() == false) {
            printf("Sensor %s not active\n", Sensors[i]->Name);
            return false;  // Note: this means one failing sensor prevents 'AllStable' to become true.
         }
         if (Sensors[i]->IsStable() == false) {
            printf("Sensor %s not yet stable\n", Sensors[i]->Name);
            return false;
         }
      }
      return true;
   }

private:
   int ListLen;
   int ListIx;   // first free entry
   TSensorWrapper **Sensors;
};

#include "W_CCS811.h"
#include "W_HDC1080.h"
#include "W_HTU21D.h"
#include "W_MCP9808.h"
#include "W_MHZ19.h"
#include "W_SCD30.h"
#include "W_SGP30.h"
#include "W_SPS30.h"
