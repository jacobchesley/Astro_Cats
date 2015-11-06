EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:Shared
LIBS:tracking-cache
EELAYER 25 0
EELAYER END
$Descr User 9000 8000
encoding utf-8
Sheet 1 1
Title "Rocket Tracking"
Date "2015-10-31"
Rev "1"
Comp "UC Astro Cats"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ARDUINO_MEGA U?
U 1 1 562FAB6D
P 2750 2700
F 0 "U?" H 1850 3950 60  0000 C CNN
F 1 "ARDUINO_MEGA" V 2750 2650 60  0000 C CNN
F 2 "" H 2750 2700 60  0000 C CNN
F 3 "" H 2750 2700 60  0000 C CNN
	1    2750 2700
	1    0    0    -1  
$EndComp
$Comp
L SparkFun_Venus_GPS GPS1
U 1 1 562FADE7
P 6200 1900
F 0 "GPS1" H 6200 1900 60  0000 C CNN
F 1 "SparkFun_Venus_GPS" H 6200 2200 60  0000 C CNN
F 2 "" H 6200 1900 60  0000 C CNN
F 3 "" H 6200 1900 60  0000 C CNN
	1    6200 1900
	1    0    0    -1  
$EndComp
$Comp
L XTEND_900 XT1
U 1 1 562FAE8B
P 6800 3400
F 0 "XT1" H 6800 3400 60  0000 C CNN
F 1 "XTEND_900" H 6850 3500 60  0000 C CNN
F 2 "" H 6800 3400 60  0000 C CNN
F 3 "" H 6800 3400 60  0000 C CNN
	1    6800 3400
	1    0    0    -1  
$EndComp
Wire Wire Line
	1650 2100 1050 2100
Wire Wire Line
	1050 700  1050 2100
Wire Wire Line
	1050 2100 1050 5550
Wire Wire Line
	1050 700  5400 700 
Wire Wire Line
	5400 700  5400 1750
Wire Wire Line
	5400 1850 5200 1850
Wire Wire Line
	5200 1850 5200 850 
Wire Wire Line
	5200 850  1250 850 
Wire Wire Line
	1250 850  1250 1900
Wire Wire Line
	1250 1900 1650 1900
Wire Wire Line
	5400 2050 3850 2050
Wire Wire Line
	5400 1950 5250 1950
Wire Wire Line
	5250 1950 5250 2150
Wire Wire Line
	5250 2150 3850 2150
Wire Wire Line
	5400 1750 5000 1750
Wire Wire Line
	5000 1750 5000 2900
Wire Wire Line
	5000 2900 5400 2900
Connection ~ 5400 1750
Wire Wire Line
	5400 3300 4350 3300
Wire Wire Line
	4350 3300 4350 2350
Wire Wire Line
	4350 2350 3850 2350
Wire Wire Line
	3850 2250 4450 2250
Wire Wire Line
	4450 2250 4450 3400
Wire Wire Line
	4450 3400 5400 3400
$Comp
L LM317AT U1
U 1 1 562FB427
P 5850 4900
F 0 "U1" H 5650 5100 40  0000 C CNN
F 1 "LM317AT" H 5850 5100 40  0000 L CNN
F 2 "TO-220" H 5850 5000 30  0000 C CIN
F 3 "" H 5850 4900 60  0000 C CNN
	1    5850 4900
	-1   0    0    1   
$EndComp
Wire Wire Line
	5000 3000 5000 4950
Wire Wire Line
	5000 3000 5400 3000
Wire Wire Line
	5000 4950 5350 4950
Wire Wire Line
	5350 4950 5450 4950
$Comp
L R R1
U 1 1 562FB501
P 5700 4550
F 0 "R1" V 5780 4550 50  0000 C CNN
F 1 "180Ω" V 5700 4550 50  0000 C CNN
F 2 "" V 5630 4550 30  0000 C CNN
F 3 "" H 5700 4550 30  0000 C CNN
	1    5700 4550
	0    1    -1   0   
$EndComp
Wire Wire Line
	5850 4650 5850 4550
Wire Wire Line
	5550 4550 5350 4550
Wire Wire Line
	5350 4550 5350 4950
Connection ~ 5350 4950
$Comp
L R R2
U 1 1 562FB5B4
P 6000 4550
F 0 "R2" V 6080 4550 50  0000 C CNN
F 1 "560Ω" V 6000 4550 50  0000 C CNN
F 2 "" V 5930 4550 30  0000 C CNN
F 3 "" H 6000 4550 30  0000 C CNN
	1    6000 4550
	0    1    -1   0   
$EndComp
Wire Wire Line
	6150 4550 6850 4550
Wire Wire Line
	6850 4550 7550 4550
$Comp
L Battery BT1
U 1 1 562FB986
P 6850 4800
F 0 "BT1" H 6950 4850 50  0000 L CNN
F 1 "7.4V Battery" H 6950 4750 50  0000 L CNN
F 2 "" V 6850 4840 60  0000 C CNN
F 3 "" V 6850 4840 60  0000 C CNN
	1    6850 4800
	1    0    0    1   
$EndComp
Wire Wire Line
	6850 4950 6250 4950
Wire Wire Line
	6850 4550 6850 4650
Wire Wire Line
	1050 5550 7550 5550
Wire Wire Line
	7550 5550 7550 4550
Connection ~ 6850 4550
Connection ~ 1050 2100
Wire Wire Line
	1650 2200 1200 2200
Wire Wire Line
	1200 2200 1200 5450
Wire Wire Line
	1200 5450 6850 5450
Wire Wire Line
	6850 5450 6850 4950
Connection ~ 6850 4950
Text Notes 4700 4950 0    60   ~ 0
+5.1V
Text Notes 4650 5250 0    60   ~ 0
Radio Voltage Regulation Circuit
$EndSCHEMATC
