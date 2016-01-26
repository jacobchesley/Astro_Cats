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
EELAYER 25 0
EELAYER END
$Descr User 8500 6500
encoding utf-8
Sheet 1 1
Title "Ground Electronics"
Date "2016-01-14"
Rev "2"
Comp "UC Astro Cats"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ARDUINO_MEGA U?
U 1 1 56322F87
P 2550 2250
F 0 "U?" H 1650 3500 60  0000 C CNN
F 1 "ARDUINO_MEGA" V 2550 2200 60  0000 C CNN
F 2 "" H 2550 2250 60  0000 C CNN
F 3 "" H 2550 2250 60  0000 C CNN
	1    2550 2250
	1    0    0    -1  
$EndComp
$Comp
L XTEND_900 XT?
U 1 1 56322FAC
P 6150 1800
F 0 "XT?" H 6150 1800 60  0000 C CNN
F 1 "XTEND_900" H 6200 1900 60  0000 C CNN
F 2 "" H 6150 1800 60  0000 C CNN
F 3 "" H 6150 1800 60  0000 C CNN
	1    6150 1800
	1    0    0    -1  
$EndComp
$Comp
L LM317AT U?
U 1 1 5632313B
P 5550 3650
F 0 "U?" H 5350 3850 40  0000 C CNN
F 1 "LM317AT" H 5550 3850 40  0000 L CNN
F 2 "TO-220" H 5550 3750 30  0000 C CIN
F 3 "" H 5550 3650 60  0000 C CNN
	1    5550 3650
	-1   0    0    1   
$EndComp
$Comp
L R R1
U 1 1 5632317E
P 5400 3300
F 0 "R1" V 5480 3300 50  0000 C CNN
F 1 "180Ω" V 5400 3300 50  0000 C CNN
F 2 "" V 5330 3300 30  0000 C CNN
F 3 "" H 5400 3300 30  0000 C CNN
	1    5400 3300
	0    1    -1   0   
$EndComp
$Comp
L R R2
U 1 1 56323226
P 5700 3300
F 0 "R2" V 5780 3300 50  0000 C CNN
F 1 "560Ω" V 5700 3300 50  0000 C CNN
F 2 "" V 5630 3300 30  0000 C CNN
F 3 "" H 5700 3300 30  0000 C CNN
	1    5700 3300
	0    -1   -1   0   
$EndComp
$Comp
L Earth #PWR?
U 1 1 56323375
P 6000 3300
F 0 "#PWR?" H 6000 3050 50  0001 C CNN
F 1 "Earth" H 6000 3150 50  0001 C CNN
F 2 "" H 6000 3300 60  0000 C CNN
F 3 "" H 6000 3300 60  0000 C CNN
	1    6000 3300
	1    0    0    -1  
$EndComp
$Comp
L +9V #PWR?
U 1 1 5634F39A
P 6000 3700
F 0 "#PWR?" H 6000 3550 50  0001 C CNN
F 1 "+9V" H 6000 3840 50  0000 C CNN
F 2 "" H 6000 3700 60  0000 C CNN
F 3 "" H 6000 3700 60  0000 C CNN
	1    6000 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4750 1700 3650 1700
Wire Wire Line
	4750 1800 4300 1800
Wire Wire Line
	4300 1800 4300 1600
Wire Wire Line
	4300 1600 3650 1600
Wire Wire Line
	3650 1300 4750 1300
Wire Wire Line
	5550 3400 5550 3300
Connection ~ 5550 3300
Wire Wire Line
	5250 3300 5050 3300
Wire Wire Line
	5050 3300 5050 3700
Wire Wire Line
	4450 3700 5150 3700
Wire Wire Line
	5850 3300 6150 3300
Wire Wire Line
	5950 3700 6000 3700
Wire Wire Line
	4450 3700 4450 1400
Wire Wire Line
	4450 1400 4750 1400
Connection ~ 5050 3700
Wire Wire Line
	1450 1750 1050 1750
Wire Wire Line
	1050 1750 1050 4350
Wire Wire Line
	1050 4350 6000 4350
Wire Wire Line
	6000 4350 6000 3700
Connection ~ 6000 3700
Wire Wire Line
	6150 3300 6150 4500
Wire Wire Line
	6150 4500 900  4500
Wire Wire Line
	900  4500 900  1650
Wire Wire Line
	900  1650 1450 1650
Connection ~ 6000 3300
Text Notes 4250 3800 0    60   ~ 0
+5.1V
Text Notes 4450 4000 0    60   ~ 0
Radio Voltage Regulation Circuit
Wire Wire Line
	4750 1900 3850 1900
Wire Wire Line
	3850 1900 3850 2200
Wire Wire Line
	3850 2200 3650 2200
$EndSCHEMATC
