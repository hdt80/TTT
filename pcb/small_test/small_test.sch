EESchema Schematic File Version 4
LIBS:small_test-cache
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L LED:WS2812B D0
U 1 1 5E3231BA
P 5000 1500
F 0 "D0" H 5100 1750 50  0000 L CNN
F 1 "WS2812B" H 5100 1850 50  0000 L CNN
F 2 "LED_SMD:LED_WS2812B_PLCC4_5.0x5.0mm_P3.2mm" H 5050 1200 50  0001 L TNN
F 3 "https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf" H 5100 1125 50  0001 L TNN
	1    5000 1500
	1    0    0    -1  
$EndComp
$Comp
L MCU_Module:ArduinoNano_r3.3 U1
U 1 1 5E2FA277
P 1450 1500
F 0 "U1" H 1450 2200 50  0000 C CNN
F 1 "ArduinoNano_r3.3" H 1450 600 50  0000 C CNN
F 2 "ARDUINO NANO 3:ArduinoNano.r3_3" H 1450 2200 50  0001 C CNN
F 3 "" H 1450 2200 50  0001 C CNN
	1    1450 1500
	1    0    0    -1  
$EndComp
Text GLabel 1900 2300 2    50   Input ~ 0
LED_DATA
Text GLabel 1000 900  0    50   Input ~ 0
USB_TR
Text GLabel 1000 1000 0    50   Input ~ 0
USB_RX
Text GLabel 4700 1500 0    50   Input ~ 0
LED_DATA
Text GLabel 1900 900  2    50   Input ~ 0
G_VCC
Text GLabel 1900 1000 2    50   Input ~ 0
G_GND
Text GLabel 5000 1200 0    50   Input ~ 0
G_VCC
Text GLabel 5000 1800 0    50   Input ~ 0
G_GND
$Comp
L LED:WS2812B D1
U 1 1 5E2FDCCC
P 6000 1500
F 0 "D1" H 6100 1750 50  0000 L CNN
F 1 "WS2812B" H 6100 1850 50  0000 L CNN
F 2 "LED_SMD:LED_WS2812B_PLCC4_5.0x5.0mm_P3.2mm" H 6050 1200 50  0001 L TNN
F 3 "https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf" H 6100 1125 50  0001 L TNN
	1    6000 1500
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 1500 5700 1500
Text GLabel 6000 1200 0    50   Input ~ 0
G_VCC
Text GLabel 6000 1800 0    50   Input ~ 0
G_GND
$Comp
L LED:WS2812B D2
U 1 1 5E2FFAA9
P 5000 2500
F 0 "D2" H 5100 2750 50  0000 L CNN
F 1 "WS2812B" H 5100 2850 50  0000 L CNN
F 2 "LED_SMD:LED_WS2812B_PLCC4_5.0x5.0mm_P3.2mm" H 5050 2200 50  0001 L TNN
F 3 "https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf" H 5100 2125 50  0001 L TNN
	1    5000 2500
	1    0    0    -1  
$EndComp
Text GLabel 5000 2200 0    50   Input ~ 0
G_VCC
Text GLabel 5000 2800 0    50   Input ~ 0
G_GND
$Comp
L LED:WS2812B D3
U 1 1 5E303945
P 6000 2500
F 0 "D3" H 6100 2750 50  0000 L CNN
F 1 "WS2812B" H 6100 2850 50  0000 L CNN
F 2 "LED_SMD:LED_WS2812B_PLCC4_5.0x5.0mm_P3.2mm" H 6050 2200 50  0001 L TNN
F 3 "https://cdn-shop.adafruit.com/datasheets/WS2812B.pdf" H 6100 2125 50  0001 L TNN
	1    6000 2500
	1    0    0    -1  
$EndComp
Text GLabel 6000 2200 0    50   Input ~ 0
G_VCC
Text GLabel 6000 2800 0    50   Input ~ 0
G_GND
Wire Wire Line
	5300 2500 5700 2500
NoConn ~ 1000 1100
NoConn ~ 1000 1200
NoConn ~ 1000 1300
NoConn ~ 1000 1400
NoConn ~ 1000 1500
NoConn ~ 1000 1600
NoConn ~ 1000 1700
NoConn ~ 1000 1800
NoConn ~ 1000 1900
NoConn ~ 1000 2000
NoConn ~ 1000 2100
NoConn ~ 1000 2200
NoConn ~ 1000 2300
NoConn ~ 1900 2200
NoConn ~ 1900 2100
NoConn ~ 1900 2000
NoConn ~ 1900 1900
NoConn ~ 1900 1800
NoConn ~ 1900 1700
NoConn ~ 1900 1600
NoConn ~ 1900 1500
NoConn ~ 1900 1400
NoConn ~ 1900 1300
NoConn ~ 1900 1200
NoConn ~ 1900 1100
Wire Wire Line
	6300 1500 6300 2000
Wire Wire Line
	6300 2000 4700 2000
Wire Wire Line
	4700 2000 4700 2500
$EndSCHEMATC
