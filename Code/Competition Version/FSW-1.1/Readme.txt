/*
 Connect the following OpenLog to Arduino:
 RXI of OpenLog to pin 2 on the Arduino
 TXO to 3
 GRN to 4
 VCC to 5V
 GND to GND
 
 XBee Xplorer
 GND GND
 3V 3V
 DOUT D0
 DIN D1
 
 10 DOF IMU
 GND GND
 3V 3V
 SDA A4
 SCL A5
 
 TMP36
 3V 3V
 A2 
 A0
 GND GND
 
 GPS 
 5V 5V
 RX D8
 TX D9
 GND GND1
 
 *********************NOTE**************************
 Pre-FLight Checklist:
 1.File in SD card exists named T.csv
 2.File in SD card exists named D.csv with a single int value "0" in it.
*/