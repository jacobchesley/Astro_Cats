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
LIBS:Recovery
EELAYER 25 0
EELAYER END
$Descr User 10500 7500
encoding utf-8
Sheet 1 1
Title "Recovery Electronics"
Date "2016-01-13"
Rev "1.0"
Comp "UC Astro Cats"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Stratologger STRAT1
U 1 1 5627C3CC
P 2150 3300
F 0 "STRAT1" H 2150 3000 60  0000 C CNN
F 1 "Stratologger" H 2150 3300 60  0000 C CNN
F 2 "" H 2150 3300 60  0000 C CNN
F 3 "" H 2150 3300 60  0000 C CNN
	1    2150 3300
	1    0    0    -1  
$EndComp
$Comp
L SPST SW1
U 1 1 5627CA60
P 4000 2650
F 0 "SW1" H 4000 2750 50  0000 C CNN
F 1 "SPST" H 4000 2550 50  0000 C CNN
F 2 "" H 4000 2650 60  0000 C CNN
F 3 "" H 4000 2650 60  0000 C CNN
	1    4000 2650
	1    0    0    -1  
$EndComp
$Comp
L Ejection_Charge Main
U 1 1 56281C95
P 3000 1300
F 0 "Main" H 3000 1150 60  0000 C CNN
F 1 "Ejection_Charge" H 3050 1450 60  0000 C CNN
F 2 "" H 3000 1300 60  0000 C CNN
F 3 "" H 3000 1300 60  0000 C CNN
	1    3000 1300
	0    -1   -1   0   
$EndComp
$Comp
L Stratologger STRAT2
U 1 1 56282CE2
P 6850 3300
F 0 "STRAT2" H 6850 2950 60  0000 C CNN
F 1 "Stratologger" H 6850 3300 60  0000 C CNN
F 2 "" H 6850 3300 60  0000 C CNN
F 3 "" H 6850 3300 60  0000 C CNN
	1    6850 3300
	1    0    0    -1  
$EndComp
$Comp
L SPST SW2
U 1 1 56282CE8
P 8700 2650
F 0 "SW2" H 8700 2750 50  0000 C CNN
F 1 "SPST" H 8700 2550 50  0000 C CNN
F 2 "" H 8700 2650 60  0000 C CNN
F 3 "" H 8700 2650 60  0000 C CNN
	1    8700 2650
	1    0    0    -1  
$EndComp
$Comp
L Ejection_Charge Main
U 1 1 56282CFB
P 7700 1300
F 0 "Main" H 7700 1150 60  0000 C CNN
F 1 "Ejection_Charge" H 7700 1450 60  0000 C CNN
F 2 "" H 7700 1300 60  0000 C CNN
F 3 "" H 7700 1300 60  0000 C CNN
	1    7700 1300
	0    -1   -1   0   
$EndComp
$Comp
L Stratologger STRAT3
U 1 1 56284A78
P 2150 6300
F 0 "STRAT3" H 2150 5950 60  0000 C CNN
F 1 "Stratologger" H 2150 6300 60  0000 C CNN
F 2 "" H 2150 6300 60  0000 C CNN
F 3 "" H 2150 6300 60  0000 C CNN
	1    2150 6300
	1    0    0    -1  
$EndComp
$Comp
L SPST SW3
U 1 1 56284A7E
P 3850 5650
F 0 "SW3" H 3850 5750 50  0000 C CNN
F 1 "SPST" H 3850 5550 50  0000 C CNN
F 2 "" H 3850 5650 60  0000 C CNN
F 3 "" H 3850 5650 60  0000 C CNN
	1    3850 5650
	1    0    0    -1  
$EndComp
$Comp
L Ejection_Charge Drogue
U 1 1 56284A91
P 2300 4600
F 0 "Drogue" H 2300 4450 60  0000 C CNN
F 1 "Ejection_Charge" H 2300 4750 60  0000 C CNN
F 2 "" H 2300 4600 60  0000 C CNN
F 3 "" H 2300 4600 60  0000 C CNN
	1    2300 4600
	0    -1   -1   0   
$EndComp
$Comp
L Battery BT2
U 1 1 562FD1E0
P 9500 3300
F 0 "BT2" H 9600 3350 50  0000 L CNN
F 1 "9V Battery" H 9600 3250 50  0000 L CNN
F 2 "" V 9500 3340 60  0000 C CNN
F 3 "" V 9500 3340 60  0000 C CNN
	1    9500 3300
	1    0    0    -1  
$EndComp
$Comp
L Battery BT3
U 1 1 562FD484
P 4800 6300
F 0 "BT3" H 4900 6350 50  0000 L CNN
F 1 "9V Battery" H 4900 6250 50  0000 L CNN
F 2 "" V 4800 6340 60  0000 C CNN
F 3 "" V 4800 6340 60  0000 C CNN
	1    4800 6300
	1    0    0    -1  
$EndComp
$Comp
L Battery BT1
U 1 1 562FD59E
P 4800 3300
F 0 "BT1" H 4900 3350 50  0000 L CNN
F 1 "9V Battery" H 4900 3250 50  0000 L CNN
F 2 "" V 4800 3340 60  0000 C CNN
F 3 "" V 4800 3340 60  0000 C CNN
	1    4800 3300
	1    0    0    -1  
$EndComp
$Comp
L Ejection_Charge Fairing
U 1 1 5634F799
P 3000 4600
F 0 "Fairing" H 3000 4450 60  0000 C CNN
F 1 "Ejection_Charge" H 3000 4750 60  0000 C CNN
F 2 "" H 3000 4600 60  0000 C CNN
F 3 "" H 3000 4600 60  0000 C CNN
	1    3000 4600
	0    -1   -1   0   
$EndComp
$Comp
L Ejection_Charge Drogue
U 1 1 56350472
P 7000 1300
F 0 "Drogue" H 7000 1150 60  0000 C CNN
F 1 "Ejection_Charge" H 7000 1450 60  0000 C CNN
F 2 "" H 7000 1300 60  0000 C CNN
F 3 "" H 7000 1300 60  0000 C CNN
	1    7000 1300
	0    -1   -1   0   
$EndComp
Text Notes 7450 800  0    60   ~ 0
Redundant
Text Notes 2050 4100 0    60   ~ 0
Redundant
Wire Wire Line
	3500 3150 4800 3150
Wire Wire Line
	3500 3450 4800 3450
Wire Wire Line
	4500 2300 4500 2650
Wire Wire Line
	3300 2300 4500 2300
Wire Wire Line
	3300 2850 3300 2300
Wire Wire Line
	3400 2650 3400 2850
Wire Wire Line
	3500 2650 3400 2650
Wire Wire Line
	8200 3150 9500 3150
Wire Wire Line
	8200 3450 9500 3450
Wire Wire Line
	9200 2300 9200 2650
Wire Wire Line
	8000 2300 9200 2300
Wire Wire Line
	8000 2850 8000 2300
Wire Wire Line
	8100 2650 8100 2850
Wire Wire Line
	8200 2650 8100 2650
Wire Wire Line
	3500 6150 4800 6150
Wire Wire Line
	3500 6450 4800 6450
Wire Wire Line
	3050 2650 3050 2000
Wire Wire Line
	2950 2650 2950 2000
Wire Wire Line
	7750 2650 7750 2000
Wire Wire Line
	7650 2650 7650 2000
Wire Wire Line
	7350 2650 7350 2150
Wire Wire Line
	7350 2150 6950 2150
Wire Wire Line
	6950 2150 6950 2000
Wire Wire Line
	7450 2650 7450 2050
Wire Wire Line
	7450 2050 7050 2050
Wire Wire Line
	7050 2050 7050 2000
Wire Wire Line
	3050 5300 3050 5650
Wire Wire Line
	2950 5300 2950 5650
Wire Wire Line
	2350 5300 2750 5300
Wire Wire Line
	2750 5300 2750 5650
Wire Wire Line
	2650 5650 2650 5400
Wire Wire Line
	2650 5400 2250 5400
Wire Wire Line
	2250 5400 2250 5300
Wire Wire Line
	3250 5650 3250 5300
Wire Wire Line
	3250 5300 4350 5300
Wire Wire Line
	4350 5300 4350 5650
$EndSCHEMATC
