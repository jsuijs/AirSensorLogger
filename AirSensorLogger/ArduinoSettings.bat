@echo off

rem zie compiler output, folder van 'arduino-builder'
set arduino_path=C:\MyRobot\Arduino\Arduino-1.8.19

rem zie compiler output, zoek op -fqbn (vanaf 'fqbn' tot 1e spatie overnemen)
set fqbn=esp32:esp32:ttgo-t1:PartitionScheme=default,CPUFreq=240,FlashMode=qio,FlashFreq=80,FlashSize=4M,UploadSpeed=921600,DebugLevel=none

rem stel configuratie in
"%arduino_path%\arduino_debug" --board %fqbn% --save-prefs
pause