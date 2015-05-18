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
LIBS:special
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
LIBS:clock_schematic_library
LIBS:clock-cache
EELAYER 27 0
EELAYER END
$Descr USLedger 17000 11000
encoding utf-8
Sheet 1 1
Title ""
Date "26 apr 2015"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L GND #PWR01
U 1 1 5336447B
P 10600 5100
F 0 "#PWR01" H 10600 5100 30  0001 C CNN
F 1 "GND" H 10600 5030 30  0001 C CNN
F 2 "" H 10600 5100 60  0000 C CNN
F 3 "" H 10600 5100 60  0000 C CNN
	1    10600 5100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 53364495
P 9400 5400
F 0 "#PWR02" H 9400 5400 30  0001 C CNN
F 1 "GND" H 9400 5330 30  0001 C CNN
F 2 "" H 9400 5400 60  0000 C CNN
F 3 "" H 9400 5400 60  0000 C CNN
	1    9400 5400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 5336449B
P 8400 5000
F 0 "#PWR03" H 8400 5000 30  0001 C CNN
F 1 "GND" H 8400 4930 30  0001 C CNN
F 2 "" H 8400 5000 60  0000 C CNN
F 3 "" H 8400 5000 60  0000 C CNN
	1    8400 5000
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR04
U 1 1 533644C9
P 9350 3400
F 0 "#PWR04" H 9350 3500 30  0001 C CNN
F 1 "VCC" H 9350 3500 30  0000 C CNN
F 2 "" H 9350 3400 60  0000 C CNN
F 3 "" H 9350 3400 60  0000 C CNN
	1    9350 3400
	1    0    0    -1  
$EndComp
Text Label 11550 4200 2    60   ~ 0
INT 2
Text Label 7550 4200 0    60   ~ 0
INT 1
Text Notes 7650 2900 0    79   ~ 0
QUADRATURE ENCODER\nSWITCH DEBOUNCE BLOCK
Text Notes 13100 3000 0    79   ~ 0
MODE SELECT KNOB
Text Notes 13100 3100 0    39   ~ 0
(POTS ARE WAY CHEAPER THAN MECHANICAL SP/MT SWITCHES)
$Comp
L R R7
U 1 1 5336474C
P 6550 6900
F 0 "R7" V 6630 6900 40  0000 C CNN
F 1 "33K" V 6557 6901 40  0000 C CNN
F 2 "~" V 6480 6900 30  0000 C CNN
F 3 "~" H 6550 6900 30  0000 C CNN
	1    6550 6900
	0    -1   -1   0   
$EndComp
$Comp
L R R8
U 1 1 53364759
P 6550 7750
F 0 "R8" V 6630 7750 40  0000 C CNN
F 1 "33K" V 6557 7751 40  0000 C CNN
F 2 "~" V 6480 7750 30  0000 C CNN
F 3 "~" H 6550 7750 30  0000 C CNN
	1    6550 7750
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR05
U 1 1 53364820
P 8950 8350
F 0 "#PWR05" H 8950 8350 30  0001 C CNN
F 1 "GND" H 8950 8280 30  0001 C CNN
F 2 "" H 8950 8350 60  0000 C CNN
F 3 "" H 8950 8350 60  0000 C CNN
	1    8950 8350
	1    0    0    -1  
$EndComp
$Comp
L CRYSTAL X1
U 1 1 5336496D
P 6750 8550
F 0 "X1" H 6750 8700 60  0000 C CNN
F 1 "CRYSTAL 32.768kHz" H 6750 8400 60  0000 C CNN
F 2 "~" H 6750 8550 60  0000 C CNN
F 3 "~" H 6750 8550 60  0000 C CNN
	1    6750 8550
	-1   0    0    1   
$EndComp
$Comp
L C C4
U 1 1 53364A88
P 7300 8900
F 0 "C4" H 7300 9000 40  0000 L CNN
F 1 "12.5pF" H 7306 8815 40  0000 L CNN
F 2 "~" H 7338 8750 30  0000 C CNN
F 3 "~" H 7300 8900 60  0000 C CNN
	1    7300 8900
	1    0    0    -1  
$EndComp
$Comp
L C C2
U 1 1 53364A97
P 6300 8950
F 0 "C2" H 6300 9050 40  0000 L CNN
F 1 "12.5pF" H 6306 8865 40  0000 L CNN
F 2 "~" H 6338 8800 30  0000 C CNN
F 3 "~" H 6300 8950 60  0000 C CNN
	1    6300 8950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 53364C53
P 7300 9250
F 0 "#PWR06" H 7300 9250 30  0001 C CNN
F 1 "GND" H 7300 9180 30  0001 C CNN
F 2 "" H 7300 9250 60  0000 C CNN
F 3 "" H 7300 9250 60  0000 C CNN
	1    7300 9250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR07
U 1 1 53364C59
P 6300 9250
F 0 "#PWR07" H 6300 9250 30  0001 C CNN
F 1 "GND" H 6300 9180 30  0001 C CNN
F 2 "" H 6300 9250 60  0000 C CNN
F 3 "" H 6300 9250 60  0000 C CNN
	1    6300 9250
	1    0    0    -1  
$EndComp
Text Label 5400 6850 0    39   ~ 0
INT 1
Text Label 5400 6750 0    39   ~ 0
INT 2
Text Label 5400 6650 0    39   ~ 0
MODE SELECT
Text Notes 15350 5450 0    79   ~ 0
ALARM SET SWITCH
Text Label 5400 6950 0    39   ~ 0
ALARM LIGHT
Text Label 5400 7050 0    39   ~ 0
ALARM BUZZER
Text Label 5400 7150 0    39   ~ 0
AM PM LIGHT
Text Label 10850 7250 0    39   ~ 0
ALARM LIGHT
Text Label 10850 8150 0    39   ~ 0
ALARM BUZZER
Text Label 10850 7550 0    39   ~ 0
AM PM LIGHT
$Comp
L R R14
U 1 1 5336522A
P 12000 7250
F 0 "R14" V 12080 7250 40  0000 C CNN
F 1 "220" V 12007 7251 40  0000 C CNN
F 2 "~" V 11930 7250 30  0000 C CNN
F 3 "~" H 12000 7250 30  0000 C CNN
	1    12000 7250
	0    -1   -1   0   
$EndComp
$Comp
L R R15
U 1 1 53365237
P 12000 7550
F 0 "R15" V 12080 7550 40  0000 C CNN
F 1 "220" V 12007 7551 40  0000 C CNN
F 2 "~" V 11930 7550 30  0000 C CNN
F 3 "~" H 12000 7550 30  0000 C CNN
	1    12000 7550
	0    -1   -1   0   
$EndComp
$Comp
L LED D2
U 1 1 5336523F
P 12450 7250
F 0 "D2" H 12450 7350 50  0000 C CNN
F 1 "LED" H 12450 7150 50  0000 C CNN
F 2 "~" H 12450 7250 60  0000 C CNN
F 3 "~" H 12450 7250 60  0000 C CNN
	1    12450 7250
	1    0    0    -1  
$EndComp
$Comp
L LED D3
U 1 1 5336524C
P 12450 7550
F 0 "D3" H 12450 7650 50  0000 C CNN
F 1 "LED" H 12450 7450 50  0000 C CNN
F 2 "~" H 12450 7550 60  0000 C CNN
F 3 "~" H 12450 7550 60  0000 C CNN
	1    12450 7550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 53365252
P 12800 7350
F 0 "#PWR08" H 12800 7350 30  0001 C CNN
F 1 "GND" H 12800 7280 30  0001 C CNN
F 2 "" H 12800 7350 60  0000 C CNN
F 3 "" H 12800 7350 60  0000 C CNN
	1    12800 7350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 53365258
P 12800 7650
F 0 "#PWR09" H 12800 7650 30  0001 C CNN
F 1 "GND" H 12800 7580 30  0001 C CNN
F 2 "" H 12800 7650 60  0000 C CNN
F 3 "" H 12800 7650 60  0000 C CNN
	1    12800 7650
	1    0    0    -1  
$EndComp
$Comp
L SPEAKER SP1
U 1 1 533652ED
P 12750 8250
F 0 "SP1" H 12650 8500 70  0000 C CNN
F 1 "SPEAKER" H 12650 8000 70  0000 C CNN
F 2 "~" H 12750 8250 60  0000 C CNN
F 3 "~" H 12750 8250 60  0000 C CNN
	1    12750 8250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR010
U 1 1 53365387
P 12200 8650
F 0 "#PWR010" H 12200 8650 30  0001 C CNN
F 1 "GND" H 12200 8580 30  0001 C CNN
F 2 "" H 12200 8650 60  0000 C CNN
F 3 "" H 12200 8650 60  0000 C CNN
	1    12200 8650
	1    0    0    -1  
$EndComp
Text Notes 10850 7050 0    79   ~ 0
LIGHTS / BUZZER (EXTRA)
$Comp
L VCC #PWR011
U 1 1 53365427
P 2150 6100
F 0 "#PWR011" H 2150 6200 30  0001 C CNN
F 1 "VCC" H 2150 6200 30  0000 C CNN
F 2 "" H 2150 6100 60  0000 C CNN
F 3 "" H 2150 6100 60  0000 C CNN
	1    2150 6100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 5336542D
P 2150 7950
F 0 "#PWR012" H 2150 7950 30  0001 C CNN
F 1 "GND" H 2150 7880 30  0001 C CNN
F 2 "" H 2150 7950 60  0000 C CNN
F 3 "" H 2150 7950 60  0000 C CNN
	1    2150 7950
	1    0    0    -1  
$EndComp
$Comp
L AVR-ISP-6 CON1
U 1 1 53378124
P 5200 9650
F 0 "CON1" H 5120 9890 50  0000 C CNN
F 1 "AVR-ISP-6" H 4960 9420 50  0000 L BNN
F 2 "AVR-ISP-6" V 4680 9690 50  0001 C CNN
F 3 "" H 5200 9650 60  0000 C CNN
	1    5200 9650
	1    0    0    -1  
$EndComp
Text Notes 4450 10050 0    79   ~ 0
ISP
$Comp
L R R9
U 1 1 53378721
P 10850 4200
F 0 "R9" V 10930 4200 40  0000 C CNN
F 1 "10K" V 10857 4201 40  0000 C CNN
F 2 "~" V 10780 4200 30  0000 C CNN
F 3 "~" H 10850 4200 30  0000 C CNN
	1    10850 4200
	0    1    1    0   
$EndComp
$Comp
L R R12
U 1 1 53378727
P 8150 4200
F 0 "R12" V 8230 4200 40  0000 C CNN
F 1 "10K" V 8157 4201 40  0000 C CNN
F 2 "~" V 8080 4200 30  0000 C CNN
F 3 "~" H 8150 4200 30  0000 C CNN
	1    8150 4200
	0    1    1    0   
$EndComp
$Comp
L C C6
U 1 1 536A4944
P 8400 4500
F 0 "C6" H 8400 4600 40  0000 L CNN
F 1 ".01uF" H 8406 4415 40  0000 L CNN
F 2 "~" H 8438 4350 30  0000 C CNN
F 3 "~" H 8400 4500 60  0000 C CNN
	1    8400 4500
	1    0    0    -1  
$EndComp
$Comp
L C C5
U 1 1 536A5550
P 10600 4600
F 0 "C5" H 10600 4700 40  0000 L CNN
F 1 ".01uF" H 10606 4515 40  0000 L CNN
F 2 "~" H 10638 4450 30  0000 C CNN
F 3 "~" H 10600 4600 60  0000 C CNN
	1    10600 4600
	1    0    0    -1  
$EndComp
Text Label 5400 6550 0    39   ~ 0
MODE SELECT POWER
$Comp
L GND #PWR013
U 1 1 5373C40E
P 4600 4600
F 0 "#PWR013" H 4600 4600 30  0001 C CNN
F 1 "GND" H 4600 4530 30  0001 C CNN
F 2 "" H 4600 4600 60  0000 C CNN
F 3 "" H 4600 4600 60  0000 C CNN
	1    4600 4600
	1    0    0    -1  
$EndComp
$Comp
L R R5
U 1 1 5373C536
P 5800 4050
F 0 "R5" V 5880 4050 40  0000 C CNN
F 1 "130K" V 5807 4051 40  0000 C CNN
F 2 "~" V 5730 4050 30  0000 C CNN
F 3 "~" H 5800 4050 30  0000 C CNN
	1    5800 4050
	1    0    0    -1  
$EndComp
$Comp
L R R6
U 1 1 5373C59E
P 5800 4850
F 0 "R6" V 5880 4850 40  0000 C CNN
F 1 "200K" V 5807 4851 40  0000 C CNN
F 2 "~" V 5730 4850 30  0000 C CNN
F 3 "~" H 5800 4850 30  0000 C CNN
	1    5800 4850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 5373C5A4
P 6400 4650
F 0 "#PWR014" H 6400 4650 30  0001 C CNN
F 1 "GND" H 6400 4580 30  0001 C CNN
F 2 "" H 6400 4650 60  0000 C CNN
F 3 "" H 6400 4650 60  0000 C CNN
	1    6400 4650
	1    0    0    -1  
$EndComp
$Comp
L MCP1624 U1
U 1 1 5373C97A
P 4600 3700
F 0 "U1" H 4750 4100 60  0000 C CNN
F 1 "MCP1624" H 4850 3350 60  0000 C CNN
F 2 "~" H 4550 4200 60  0000 C CNN
F 3 "~" H 4550 4200 60  0000 C CNN
	1    4600 3700
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 5373C996
P 6400 4200
F 0 "C3" H 6400 4300 40  0000 L CNN
F 1 "10uF" H 6406 4115 40  0000 L CNN
F 2 "~" H 6438 4050 30  0000 C CNN
F 3 "~" H 6400 4200 60  0000 C CNN
	1    6400 4200
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR015
U 1 1 5373CA73
P 7000 3600
F 0 "#PWR015" H 7000 3700 30  0001 C CNN
F 1 "VCC" H 7000 3700 30  0000 C CNN
F 2 "" H 7000 3600 60  0000 C CNN
F 3 "" H 7000 3600 60  0000 C CNN
	1    7000 3600
	0    1    1    0   
$EndComp
$Comp
L GND #PWR016
U 1 1 5373CA80
P 5800 5350
F 0 "#PWR016" H 5800 5350 30  0001 C CNN
F 1 "GND" H 5800 5280 30  0001 C CNN
F 2 "" H 5800 5350 60  0000 C CNN
F 3 "" H 5800 5350 60  0000 C CNN
	1    5800 5350
	1    0    0    -1  
$EndComp
$Comp
L INDUCTOR L1
U 1 1 5373CB63
P 4100 2900
F 0 "L1" V 4050 2900 40  0000 C CNN
F 1 "4.7uH" V 4200 2900 40  0000 C CNN
F 2 "~" H 4100 2900 60  0000 C CNN
F 3 "~" H 4100 2900 60  0000 C CNN
	1    4100 2900
	0    -1   -1   0   
$EndComp
$Comp
L C C1
U 1 1 5373D080
P 3450 4050
F 0 "C1" H 3450 4150 40  0000 L CNN
F 1 "10uF" H 3456 3965 40  0000 L CNN
F 2 "~" H 3488 3900 30  0000 C CNN
F 3 "~" H 3450 4050 60  0000 C CNN
	1    3450 4050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR017
U 1 1 5373D15F
P 3450 4500
F 0 "#PWR017" H 3450 4500 30  0001 C CNN
F 1 "GND" H 3450 4430 30  0001 C CNN
F 2 "" H 3450 4500 60  0000 C CNN
F 3 "" H 3450 4500 60  0000 C CNN
	1    3450 4500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR018
U 1 1 5373D423
P 2500 5250
F 0 "#PWR018" H 2500 5250 30  0001 C CNN
F 1 "GND" H 2500 5180 30  0001 C CNN
F 2 "" H 2500 5250 60  0000 C CNN
F 3 "" H 2500 5250 60  0000 C CNN
	1    2500 5250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR019
U 1 1 5373D429
P 1800 5250
F 0 "#PWR019" H 1800 5250 30  0001 C CNN
F 1 "GND" H 1800 5180 30  0001 C CNN
F 2 "" H 1800 5250 60  0000 C CNN
F 3 "" H 1800 5250 60  0000 C CNN
	1    1800 5250
	1    0    0    -1  
$EndComp
$Comp
L QUADRATURE_ENCODR S1
U 1 1 5373DB9B
P 9400 4500
F 0 "S1" H 9850 5000 60  0000 C CNN
F 1 "QUADRATURE_ENCODR" H 9950 4150 60  0000 C CNN
F 2 "" H 8850 5150 60  0000 C CNN
F 3 "" H 8850 5150 60  0000 C CNN
	1    9400 4500
	1    0    0    -1  
$EndComp
$Comp
L AMMETER DS1
U 1 1 53743847
P 7700 6850
F 0 "DS1" H 8000 7100 60  0000 C CNN
F 1 "AMMETER" H 7900 6550 60  0000 C CNN
F 2 "" H 7350 6800 60  0000 C CNN
F 3 "" H 7350 6800 60  0000 C CNN
	1    7700 6850
	1    0    0    -1  
$EndComp
$Comp
L AMMETER DS2
U 1 1 53743854
P 7700 7700
F 0 "DS2" H 8000 7950 60  0000 C CNN
F 1 "AMMETER" H 7900 7400 60  0000 C CNN
F 2 "" H 7350 7650 60  0000 C CNN
F 3 "" H 7350 7650 60  0000 C CNN
	1    7700 7700
	1    0    0    -1  
$EndComp
$Comp
L POT R2
U 1 1 53750AFE
P 12700 4800
F 0 "R2" H 12700 4700 50  0000 C CNN
F 1 "100K" H 12700 4800 50  0000 C CNN
F 2 "~" H 12700 4800 60  0000 C CNN
F 3 "~" H 12700 4800 60  0000 C CNN
	1    12700 4800
	0    1    1    0   
$EndComp
$Comp
L R R1
U 1 1 53750B10
P 12700 3950
F 0 "R1" V 12780 3950 40  0000 C CNN
F 1 "0K" V 12707 3951 40  0000 C CNN
F 2 "~" V 12630 3950 30  0000 C CNN
F 3 "~" H 12700 3950 30  0000 C CNN
	1    12700 3950
	1    0    0    -1  
$EndComp
Text Label 12700 3050 2    60   ~ 0
MODE SELECT POWER
$Comp
L FET_N Q1
U 1 1 53750C74
P 13900 4000
F 0 "Q1" H 13803 4250 70  0000 C CNN
F 1 "FET_N" H 13753 3753 60  0000 C CNN
F 2 "~" H 13900 4000 60  0000 C CNN
F 3 "~" H 13900 4000 60  0000 C CNN
	1    13900 4000
	-1   0    0    1   
$EndComp
$Comp
L FET_P Q2
U 1 1 53750C83
P 13900 5550
F 0 "Q2" H 13803 5800 70  0000 C CNN
F 1 "FET_P" H 13753 5303 60  0000 C CNN
F 2 "~" H 13900 5550 60  0000 C CNN
F 3 "~" H 13900 5550 60  0000 C CNN
	1    13900 5550
	-1   0    0    1   
$EndComp
$Comp
L SPST SW1
U 1 1 53750D8A
P 15150 5400
F 0 "SW1" H 15150 5500 70  0000 C CNN
F 1 "SPST" H 15150 5300 70  0000 C CNN
F 2 "~" H 15150 5400 60  0000 C CNN
F 3 "~" H 15150 5400 60  0000 C CNN
	1    15150 5400
	0    1    1    0   
$EndComp
Wire Wire Line
	8400 3600 10600 3600
Wire Wire Line
	8400 4100 8400 4300
Wire Wire Line
	10600 4100 10600 4400
Connection ~ 8400 4150
Connection ~ 10600 4200
Wire Wire Line
	9350 3400 9350 3600
Connection ~ 9350 3600
Wire Wire Line
	9400 5050 9400 5400
Wire Wire Line
	4500 7250 6100 7250
Wire Wire Line
	6800 6900 7050 6900
Wire Wire Line
	8350 6900 8950 6900
Wire Wire Line
	6800 7750 7050 7750
Wire Wire Line
	4500 7450 5250 7450
Wire Wire Line
	5250 7450 5250 8200
Wire Wire Line
	5250 8200 7300 8200
Wire Wire Line
	4500 7550 5150 7550
Wire Wire Line
	5150 7550 5150 8550
Wire Wire Line
	5150 8550 6450 8550
Wire Wire Line
	7300 8200 7300 8700
Wire Wire Line
	6300 8550 6300 8750
Connection ~ 6300 8550
Wire Wire Line
	6300 9250 6300 9150
Wire Wire Line
	7300 9250 7300 9100
Wire Wire Line
	4500 6850 5400 6850
Wire Wire Line
	4500 6750 5400 6750
Wire Wire Line
	4500 6650 5400 6650
Wire Wire Line
	4500 6950 5400 6950
Wire Wire Line
	10850 7250 11750 7250
Wire Wire Line
	10850 7550 11750 7550
Wire Wire Line
	7050 8550 7300 8550
Connection ~ 7300 8550
Connection ~ 8400 4200
Connection ~ 10600 4150
Wire Wire Line
	12650 7250 12800 7250
Wire Wire Line
	12800 7250 12800 7350
Wire Wire Line
	12650 7550 12800 7550
Wire Wire Line
	12800 7550 12800 7650
Wire Wire Line
	12450 8350 12200 8350
Wire Wire Line
	12200 8350 12200 8650
Wire Wire Line
	2150 6550 2400 6550
Wire Wire Line
	2150 6100 2150 6550
Wire Wire Line
	2150 7950 2150 7750
Wire Wire Line
	2150 7750 2400 7750
Wire Wire Line
	4600 9750 5050 9750
Wire Wire Line
	4900 9200 6000 9200
Wire Wire Line
	6000 9200 6000 9650
Wire Wire Line
	6000 9650 5350 9650
Wire Wire Line
	4800 7050 4800 9550
Wire Wire Line
	4800 9550 5050 9550
Wire Wire Line
	4700 6950 4700 9650
Wire Wire Line
	4700 9650 5050 9650
Connection ~ 4700 6950
Wire Wire Line
	4600 7750 4500 7750
Wire Wire Line
	8400 4700 8400 5000
Wire Wire Line
	10600 4800 10600 5100
Wire Wire Line
	4500 7650 6100 7650
Wire Wire Line
	4600 7750 4600 9750
Wire Wire Line
	4900 9200 4900 7150
Wire Wire Line
	8950 6900 8950 8350
Wire Wire Line
	4500 6550 5400 6550
Connection ~ 4800 7050
Connection ~ 4900 7150
Wire Wire Line
	4500 7050 5400 7050
Wire Wire Line
	4500 7150 5400 7150
Wire Wire Line
	4600 4300 4600 4600
Wire Wire Line
	5150 3600 7000 3600
Wire Wire Line
	5800 4300 5800 4600
Wire Wire Line
	6400 4400 6400 4650
Wire Wire Line
	5150 3800 5350 3800
Wire Wire Line
	5350 3800 5350 4400
Wire Wire Line
	5350 4400 5800 4400
Connection ~ 5800 4400
Wire Wire Line
	6400 4000 6400 3600
Connection ~ 6400 3600
Wire Wire Line
	5800 5100 5800 5350
Wire Wire Line
	1800 3600 4050 3600
Wire Wire Line
	3900 3600 3900 3800
Wire Wire Line
	3900 3800 4050 3800
Wire Wire Line
	3650 3600 3650 2900
Wire Wire Line
	3650 2900 3800 2900
Connection ~ 3900 3600
Wire Wire Line
	4400 2900 4600 2900
Wire Wire Line
	4600 2900 4600 3050
Connection ~ 3650 3600
Wire Wire Line
	3450 3600 3450 3850
Connection ~ 3450 3600
Wire Wire Line
	3450 4250 3450 4500
Wire Wire Line
	1800 2500 1800 3800
Wire Wire Line
	2500 3800 2500 3600
Connection ~ 2500 3600
Wire Wire Line
	1800 5050 1800 5250
Wire Wire Line
	2500 5050 2500 5250
Wire Wire Line
	10250 4150 10600 4150
Wire Wire Line
	8400 4150 8550 4150
Wire Wire Line
	8350 7750 8950 7750
Connection ~ 8950 7750
Wire Wire Line
	6300 6900 6100 6900
Wire Wire Line
	6100 6900 6100 7250
Wire Wire Line
	6100 7650 6100 7750
Wire Wire Line
	6100 7750 6300 7750
Wire Wire Line
	12700 4200 12700 4550
Wire Wire Line
	12700 5050 12700 5300
Wire Wire Line
	12850 4800 13150 4800
Wire Wire Line
	12700 3050 12700 3700
Wire Wire Line
	15150 4100 15150 4900
Wire Wire Line
	15150 4550 14550 4550
Wire Wire Line
	14550 4000 14550 5550
Wire Wire Line
	14550 4000 14200 4000
Connection ~ 15150 4550
Wire Wire Line
	14550 5550 14200 5550
Connection ~ 14550 4550
Wire Wire Line
	15150 3300 15150 3600
Wire Wire Line
	12700 3300 15150 3300
Connection ~ 12700 3300
$Comp
L GND #PWR020
U 1 1 537512F1
P 12700 6100
F 0 "#PWR020" H 12700 6100 30  0001 C CNN
F 1 "GND" H 12700 6030 30  0001 C CNN
F 2 "" H 12700 6100 60  0000 C CNN
F 3 "" H 12700 6100 60  0000 C CNN
	1    12700 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	12700 5800 12700 6100
Text Label 13150 4800 0    59   ~ 0
MODE SELECT
Wire Wire Line
	12700 5150 13750 5150
Wire Wire Line
	13750 5150 13750 5200
Connection ~ 12700 5150
Wire Wire Line
	13750 4350 13750 4450
Wire Wire Line
	13750 4450 12700 4450
Connection ~ 12700 4450
Wire Wire Line
	13750 5900 13750 6100
Wire Wire Line
	15150 5900 15150 6100
$Comp
L GND #PWR021
U 1 1 537516B3
P 13750 6100
F 0 "#PWR021" H 13750 6100 30  0001 C CNN
F 1 "GND" H 13750 6030 30  0001 C CNN
F 2 "" H 13750 6100 60  0000 C CNN
F 3 "" H 13750 6100 60  0000 C CNN
	1    13750 6100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR022
U 1 1 537516B9
P 15150 6100
F 0 "#PWR022" H 15150 6100 30  0001 C CNN
F 1 "GND" H 15150 6030 30  0001 C CNN
F 2 "" H 15150 6100 60  0000 C CNN
F 3 "" H 15150 6100 60  0000 C CNN
	1    15150 6100
	1    0    0    -1  
$EndComp
Text Notes 2250 3000 0    79   ~ 0
POWER BLOCK
$Comp
L R R3
U 1 1 53764252
P 12700 5550
F 0 "R3" V 12780 5550 40  0000 C CNN
F 1 "10K" V 12707 5551 40  0000 C CNN
F 2 "~" V 12630 5550 30  0000 C CNN
F 3 "~" H 12700 5550 30  0000 C CNN
	1    12700 5550
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 53764258
P 15150 3850
F 0 "R4" V 15230 3850 40  0000 C CNN
F 1 "100K" V 15157 3851 40  0000 C CNN
F 2 "~" V 15080 3850 30  0000 C CNN
F 3 "~" H 15150 3850 30  0000 C CNN
	1    15150 3850
	1    0    0    -1  
$EndComp
$Comp
L R R11
U 1 1 5376426B
P 8400 3850
F 0 "R11" V 8480 3850 40  0000 C CNN
F 1 "130K" V 8407 3851 40  0000 C CNN
F 2 "~" V 8330 3850 30  0000 C CNN
F 3 "~" H 8400 3850 30  0000 C CNN
	1    8400 3850
	1    0    0    -1  
$EndComp
$Comp
L R R10
U 1 1 53764271
P 10600 3850
F 0 "R10" V 10680 3850 40  0000 C CNN
F 1 "130K" V 10607 3851 40  0000 C CNN
F 2 "~" V 10530 3850 30  0000 C CNN
F 3 "~" H 10600 3850 30  0000 C CNN
	1    10600 3850
	1    0    0    -1  
$EndComp
$Comp
L R R13
U 1 1 537644A8
P 11600 8150
F 0 "R13" V 11680 8150 40  0000 C CNN
F 1 "220" V 11607 8151 40  0000 C CNN
F 2 "~" V 11530 8150 30  0000 C CNN
F 3 "~" H 11600 8150 30  0000 C CNN
	1    11600 8150
	0    -1   -1   0   
$EndComp
Wire Wire Line
	11850 8150 12450 8150
Wire Wire Line
	11350 8150 10850 8150
$Comp
L DIODE D1
U 1 1 537645F2
P 11950 8550
F 0 "D1" H 11950 8650 40  0000 C CNN
F 1 "DIODE" H 11950 8450 40  0000 C CNN
F 2 "~" H 11950 8550 60  0000 C CNN
F 3 "~" H 11950 8550 60  0000 C CNN
	1    11950 8550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	11950 8750 11950 8850
$Comp
L GND #PWR023
U 1 1 53764BAE
P 11950 8850
F 0 "#PWR023" H 11950 8850 30  0001 C CNN
F 1 "GND" H 11950 8780 30  0001 C CNN
F 2 "" H 11950 8850 60  0000 C CNN
F 3 "" H 11950 8850 60  0000 C CNN
	1    11950 8850
	1    0    0    -1  
$EndComp
Wire Wire Line
	5350 9550 5700 9550
Wire Wire Line
	5700 9550 5700 9450
Wire Wire Line
	5350 9750 5700 9750
Wire Wire Line
	5700 9750 5700 9850
$Comp
L GND #PWR024
U 1 1 53777472
P 5700 9850
F 0 "#PWR024" H 5700 9850 30  0001 C CNN
F 1 "GND" H 5700 9780 30  0001 C CNN
F 2 "" H 5700 9850 60  0000 C CNN
F 3 "" H 5700 9850 60  0000 C CNN
	1    5700 9850
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR025
U 1 1 5377747A
P 5700 9450
F 0 "#PWR025" H 5700 9550 30  0001 C CNN
F 1 "VCC" H 5700 9550 30  0000 C CNN
F 2 "" H 5700 9450 60  0000 C CNN
F 3 "" H 5700 9450 60  0000 C CNN
	1    5700 9450
	1    0    0    -1  
$EndComp
Wire Wire Line
	11950 8350 11950 8150
Connection ~ 11950 8150
Wire Wire Line
	13750 3300 13750 3650
Connection ~ 13750 3300
Wire Wire Line
	5800 3600 5800 3800
Connection ~ 5800 3600
$Comp
L ATTINY84A-SS IC1
U 1 1 5378D9C9
P 3450 7150
F 0 "IC1" H 2700 7900 60  0000 C CNN
F 1 "ATTINY84A-SS" H 3950 6400 60  0000 C CNN
F 2 "SO14" H 2750 6400 60  0001 C CNN
F 3 "" H 3450 7150 60  0000 C CNN
	1    3450 7150
	1    0    0    -1  
$EndComp
$Comp
L BATTERY_CLIP BT3
U 1 1 53B41CA4
P 2500 4100
F 0 "BT3" H 2600 4200 60  0000 L CNN
F 1 "BATTERY_CLIP" H 2600 4300 60  0000 L CNN
F 2 "" H 2600 4250 60  0000 C CNN
F 3 "" H 2600 4250 60  0000 C CNN
	1    2500 4100
	1    0    0    -1  
$EndComp
$Comp
L BATTERY_CLIP BT1
U 1 1 53B41CBB
P 1800 4100
F 0 "BT1" H 1150 4200 60  0000 L CNN
F 1 "BATTERY_CLIP" H 1150 4300 60  0000 L CNN
F 2 "" H 1900 4250 60  0000 C CNN
F 3 "" H 1900 4250 60  0000 C CNN
	1    1800 4100
	1    0    0    -1  
$EndComp
$Comp
L BATTERY_CLIP BT2
U 1 1 53B41CFA
P 1800 4750
F 0 "BT2" H 2300 5100 60  0000 L CNN
F 1 "BATTERY_CLIP" H 1850 5000 60  0000 L CNN
F 2 "" H 1900 4900 60  0000 C CNN
F 3 "" H 1900 4900 60  0000 C CNN
	1    1800 4750
	-1   0    0    1   
$EndComp
$Comp
L BATTERY_CLIP BT4
U 1 1 53B41D0A
P 2500 4750
F 0 "BT4" H 2300 5050 60  0000 L CNN
F 1 "BATTERY_CLIP" H 1850 4950 60  0000 L CNN
F 2 "" H 2600 4900 60  0000 C CNN
F 3 "" H 2600 4900 60  0000 C CNN
	1    2500 4750
	-1   0    0    1   
$EndComp
Wire Wire Line
	11100 4200 11550 4200
Wire Wire Line
	7550 4200 7900 4200
$Comp
L USB-MICRO-B CON2
U 1 1 54A87A25
P 1850 1450
F 0 "CON2" H 1550 1800 50  0000 C CNN
F 1 "USB-MICRO-B" H 1700 1100 50  0000 C CNN
F 2 "" H 1850 1350 50  0000 C CNN
F 3 "" H 1850 1350 50  0000 C CNN
	1    1850 1450
	1    0    0    -1  
$EndComp
$Comp
L MCP1702T-1502E/TO U2
U 1 1 54A87F96
P 3200 1300
F 0 "U2" H 3000 1500 40  0000 C CNN
F 1 "MCP1702T-1502E/TO" H 3200 1500 40  0000 L CNN
F 2 "TO-92" H 3200 1400 30  0000 C CIN
F 3 "~" H 3200 1300 60  0000 C CNN
	1    3200 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 1250 2800 1250
Wire Wire Line
	3200 2050 3200 1550
Wire Wire Line
	3600 1250 3950 1250
Wire Wire Line
	3950 1250 3950 2500
Wire Wire Line
	3950 2500 1800 2500
Connection ~ 1800 3600
Wire Wire Line
	2650 800  2650 1400
Wire Wire Line
	1100 800  2650 800 
Wire Wire Line
	1100 1250 1100 800 
Connection ~ 1100 2050
$Comp
L GND #PWR026
U 1 1 54A88164
P 1100 2300
F 0 "#PWR026" H 1100 2300 30  0001 C CNN
F 1 "GND" H 1100 2230 30  0001 C CNN
F 2 "" H 1100 2300 60  0000 C CNN
F 3 "" H 1100 2300 60  0000 C CNN
	1    1100 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1100 2050 3200 2050
Wire Wire Line
	1100 1650 1100 2300
Wire Wire Line
	1350 1650 1100 1650
Wire Wire Line
	1350 1250 1100 1250
$Comp
L C C7
U 1 1 54A89CEC
P 2650 1600
F 0 "C7" H 2650 1700 40  0000 L CNN
F 1 "1uF" H 2656 1515 40  0000 L CNN
F 2 "~" H 2688 1450 30  0000 C CNN
F 3 "~" H 2650 1600 60  0000 C CNN
	1    2650 1600
	1    0    0    -1  
$EndComp
Connection ~ 2650 1250
Wire Wire Line
	2650 1800 2650 2050
Connection ~ 2650 2050
Text Label 7550 4200 0    60   ~ 0
INT 1
Text Notes 6800 900  2    60   ~ 0
TEST PORT BLOCK
Wire Wire Line
	1800 3200 1600 3200
Connection ~ 1800 3200
Text Label 1600 3200 2    60   ~ 0
BATTERY POWER
Wire Wire Line
	5800 1100 6300 1100
Text Label 5800 1100 2    60   ~ 0
BATTERY POWER
Wire Wire Line
	6300 1200 5250 1200
Wire Wire Line
	5250 1200 5250 1300
$Comp
L GND #PWR027
U 1 1 552410F8
P 5250 1300
F 0 "#PWR027" H 5250 1300 30  0001 C CNN
F 1 "GND" H 5250 1230 30  0001 C CNN
F 2 "" H 5250 1300 60  0000 C CNN
F 3 "" H 5250 1300 60  0000 C CNN
	1    5250 1300
	1    0    0    -1  
$EndComp
$Comp
L CONN_6 P1
U 1 1 55241100
P 6650 1350
F 0 "P1" V 6600 1350 60  0000 C CNN
F 1 "CONN_6" V 6700 1350 60  0000 C CNN
F 2 "" H 6650 1350 60  0000 C CNN
F 3 "" H 6650 1350 60  0000 C CNN
	1    6650 1350
	1    0    0    -1  
$EndComp
Text Label 6900 7100 0    60   ~ 0
HOURS
Text Label 6900 7550 0    60   ~ 0
MINUTES
Text Label 5950 1400 2    60   ~ 0
HOURS
Text Label 5950 1300 2    60   ~ 0
MINUTES
Wire Wire Line
	6300 1300 5950 1300
Wire Wire Line
	6300 1400 5950 1400
$Comp
L VCC #PWR028
U 1 1 55241264
P 5900 1600
F 0 "#PWR028" H 5900 1700 30  0001 C CNN
F 1 "VCC" H 5900 1700 30  0000 C CNN
F 2 "" H 5900 1600 60  0000 C CNN
F 3 "" H 5900 1600 60  0000 C CNN
	1    5900 1600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5900 1600 6300 1600
Wire Wire Line
	6300 1500 5950 1500
Text Label 5950 1500 2    59   ~ 0
MODE SELECT
Wire Wire Line
	6900 7100 6900 6900
Connection ~ 6900 6900
Wire Wire Line
	6900 7550 6900 7750
Connection ~ 6900 7750
Wire Wire Line
	8000 1100 8400 1100
Wire Wire Line
	8000 1300 8000 1600
$Comp
L CONN_2 P2
U 1 1 552DA9D4
P 8750 1200
F 0 "P2" V 8700 1200 40  0000 C CNN
F 1 "CONN_2" V 8800 1200 40  0000 C CNN
F 2 "" H 8750 1200 60  0000 C CNN
F 3 "" H 8750 1200 60  0000 C CNN
	1    8750 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	8000 1300 8400 1300
$Comp
L VCC #PWR029
U 1 1 552DAA8D
P 8000 1100
F 0 "#PWR029" H 8000 1200 30  0001 C CNN
F 1 "VCC" H 8000 1200 30  0000 C CNN
F 2 "" H 8000 1100 60  0000 C CNN
F 3 "" H 8000 1100 60  0000 C CNN
	1    8000 1100
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR030
U 1 1 552DAA93
P 8000 1600
F 0 "#PWR030" H 8000 1600 30  0001 C CNN
F 1 "GND" H 8000 1530 30  0001 C CNN
F 2 "" H 8000 1600 60  0000 C CNN
F 3 "" H 8000 1600 60  0000 C CNN
	1    8000 1600
	1    0    0    -1  
$EndComp
Text Notes 8800 950  2    60   ~ 0
POWER FOR LIGHTING
Wire Wire Line
	9200 1100 9600 1100
Wire Wire Line
	9200 1300 9200 1600
$Comp
L CONN_2 P3
U 1 1 552DAE13
P 9950 1200
F 0 "P3" V 9900 1200 40  0000 C CNN
F 1 "CONN_2" V 10000 1200 40  0000 C CNN
F 2 "" H 9950 1200 60  0000 C CNN
F 3 "" H 9950 1200 60  0000 C CNN
	1    9950 1200
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 1300 9600 1300
$Comp
L VCC #PWR031
U 1 1 552DAE1A
P 9200 1100
F 0 "#PWR031" H 9200 1200 30  0001 C CNN
F 1 "VCC" H 9200 1200 30  0000 C CNN
F 2 "" H 9200 1100 60  0000 C CNN
F 3 "" H 9200 1100 60  0000 C CNN
	1    9200 1100
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR032
U 1 1 552DAE20
P 9200 1600
F 0 "#PWR032" H 9200 1600 30  0001 C CNN
F 1 "GND" H 9200 1530 30  0001 C CNN
F 2 "" H 9200 1600 60  0000 C CNN
F 3 "" H 9200 1600 60  0000 C CNN
	1    9200 1600
	1    0    0    -1  
$EndComp
$EndSCHEMATC
