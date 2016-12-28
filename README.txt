# WebServer
Arduino Web Server

Hardware:
  1. Arduino MEGA (prefered - more memory for temporary variables)
  2. Arduino Ethernet Shield
  

This paltform has only 16Mhz of CPU clock. Code is optimized to increase page loading time. 
This changes improved loading time about 20 times.

I had some problems with SD card -> If you have some problems with initialization => check File system of your SD card 
(it should be FAT system, 32KB - only this one works fine for me).


What to do?
Just download the code -> program your Arduino -> put web file (server checks if SD card has "index.html" file in main catalog) ->
open your browser -> open server 192.168.1.105:12345 -> and your page should work.

Code recognize "GET" request - reload page / ajax / 

Used SDFat library -> with standard SD.h lib it doesn't work.

