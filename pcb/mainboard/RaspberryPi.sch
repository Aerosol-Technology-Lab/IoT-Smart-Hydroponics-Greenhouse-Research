EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 2 3
Title ""
Date "2021-10-17"
Rev "1.0"
Comp "Aerosol Technology Lab | Texas A&M University @ College Station, TX"
Comment1 "Biological and Agricultural Engineering"
Comment2 "Author: Charlemagne Wong | CSCE 21'"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_02x20_Odd_Even J?
U 1 1 61714C61
P 4100 2900
AR Path="/61714C61" Ref="J?"  Part="1" 
AR Path="/6170A1B9/61714C61" Ref="J18"  Part="1" 
F 0 "J18" H 4150 4017 50  0000 C CNN
F 1 "Raspberry Pi" H 4150 3926 50  0000 C CNN
F 2 "Connector_IDC:IDC-Header_2x20_P2.54mm_Vertical" H 4100 2900 50  0001 C CNN
F 3 "~" H 4100 2900 50  0001 C CNN
	1    4100 2900
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 61714C67
P 4550 1850
AR Path="/61714C67" Ref="#PWR?"  Part="1" 
AR Path="/6170A1B9/61714C67" Ref="#PWR0155"  Part="1" 
F 0 "#PWR0155" H 4550 1700 50  0001 C CNN
F 1 "+5V" H 4565 2023 50  0000 C CNN
F 2 "" H 4550 1850 50  0001 C CNN
F 3 "" H 4550 1850 50  0001 C CNN
	1    4550 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 2000 4550 2000
Wire Wire Line
	4550 2000 4550 1850
Wire Wire Line
	4400 2100 4550 2100
Wire Wire Line
	4550 2100 4550 2000
Connection ~ 4550 2000
Wire Wire Line
	3750 1850 3750 2000
Wire Wire Line
	3750 2000 3900 2000
Text GLabel 3850 2100 0    50   Input ~ 0
PI_SDA1
Text GLabel 3850 2200 0    50   Input ~ 0
PI_SCL1
Wire Wire Line
	3850 2200 3900 2200
Wire Wire Line
	3900 2100 3850 2100
Text GLabel 3850 2300 0    50   Input ~ 0
MC_GPCLK0
Text GLabel 3850 2500 0    50   Input ~ 0
PI_GPIO0
Text GLabel 3850 2600 0    50   Input ~ 0
PI_GPIO2
Text GLabel 3850 2700 0    50   Input ~ 0
PI_GPIO3
Text GLabel 3850 2900 0    50   Input ~ 0
PI_MOSI
Text GLabel 3850 3000 0    50   Input ~ 0
PI_MISO
Text GLabel 3850 3100 0    50   Input ~ 0
PI_SCLK
Text GLabel 3850 3300 0    50   Input ~ 0
PI_SDA0
Text GLabel 4450 3300 2    50   Input ~ 0
PI_SCL0
Wire Wire Line
	4450 3300 4400 3300
Text GLabel 4450 3500 2    50   Input ~ 0
PI_GPIO26
Text GLabel 4450 3700 2    50   Input ~ 0
PI_GPIO27
Text GLabel 4450 3800 2    50   Input ~ 0
PI_GPIO28-PCM_DIN
Text GLabel 4450 3900 2    50   Input ~ 0
PI_GPIO29-PCM_DOUT
Text GLabel 3850 3800 0    50   Input ~ 0
GPIO_25
Text GLabel 3850 3700 0    50   Input ~ 0
PI_GPIO24-PCM_FS-PWM1
Text GLabel 3850 3600 0    50   Input ~ 0
PI_GPIO23-PWM1
Text GLabel 3850 3500 0    50   Input ~ 0
PI_GPIO22-GPCLK2
Text GLabel 3850 3400 0    50   Input ~ 0
PI_GPIO21-GPCLK1
Text GLabel 4450 2300 2    50   Input ~ 0
PI_TX
Text GLabel 4450 2400 2    50   Input ~ 0
PI_RX
Text GLabel 4450 2500 2    50   Input ~ 0
PI_GPIO1-PCM_CLK-PWM0
Text GLabel 4450 2700 2    50   Input ~ 0
PI_GPIO4
Text GLabel 4450 2800 2    50   Input ~ 0
PI_GPIO5
Text GLabel 4450 3000 2    50   Input ~ 0
PI_GPIO6
Text GLabel 4450 3100 2    50   Input ~ 0
PI_GPIO10-CE0(SPI)
Text GLabel 4450 3200 2    50   Input ~ 0
PI_GPIO11-CE1(SPI)
Wire Wire Line
	3900 3800 3850 3800
Wire Wire Line
	3900 3700 3850 3700
Wire Wire Line
	3850 3600 3900 3600
Wire Wire Line
	3900 3500 3850 3500
Wire Wire Line
	3850 3400 3900 3400
Wire Wire Line
	3900 3300 3850 3300
Wire Wire Line
	3850 3100 3900 3100
Wire Wire Line
	3900 3000 3850 3000
Wire Wire Line
	3850 2900 3900 2900
Wire Wire Line
	3900 2700 3850 2700
Wire Wire Line
	3850 2600 3900 2600
Wire Wire Line
	3900 2500 3850 2500
Wire Wire Line
	3850 2300 3900 2300
Wire Wire Line
	4400 2300 4450 2300
Wire Wire Line
	4450 2400 4400 2400
Wire Wire Line
	4450 2700 4400 2700
Wire Wire Line
	4400 2500 4450 2500
Wire Wire Line
	4400 3000 4450 3000
Wire Wire Line
	4450 3100 4400 3100
Wire Wire Line
	4400 3200 4450 3200
Wire Wire Line
	4450 3500 4400 3500
Wire Wire Line
	4400 3700 4450 3700
Wire Wire Line
	4450 3800 4400 3800
Wire Wire Line
	4400 3900 4450 3900
Wire Wire Line
	4400 2800 4450 2800
$Comp
L power:GND #PWR?
U 1 1 61714CAC
P 3400 3200
AR Path="/61714CAC" Ref="#PWR?"  Part="1" 
AR Path="/6170A1B9/61714CAC" Ref="#PWR0156"  Part="1" 
F 0 "#PWR0156" H 3400 2950 50  0001 C CNN
F 1 "GND" V 3405 3072 50  0000 R CNN
F 2 "" H 3400 3200 50  0001 C CNN
F 3 "" H 3400 3200 50  0001 C CNN
	1    3400 3200
	0    1    1    0   
$EndComp
Wire Wire Line
	3400 3200 3900 3200
$Comp
L power:GND #PWR?
U 1 1 61714CB3
P 3400 2400
AR Path="/61714CB3" Ref="#PWR?"  Part="1" 
AR Path="/6170A1B9/61714CB3" Ref="#PWR0157"  Part="1" 
F 0 "#PWR0157" H 3400 2150 50  0001 C CNN
F 1 "GND" V 3405 2272 50  0000 R CNN
F 2 "" H 3400 2400 50  0001 C CNN
F 3 "" H 3400 2400 50  0001 C CNN
	1    3400 2400
	0    1    1    0   
$EndComp
Wire Wire Line
	3400 2400 3900 2400
NoConn ~ 3900 2800
$Comp
L power:GND #PWR?
U 1 1 61714CBB
P 3400 3900
AR Path="/61714CBB" Ref="#PWR?"  Part="1" 
AR Path="/6170A1B9/61714CBB" Ref="#PWR0158"  Part="1" 
F 0 "#PWR0158" H 3400 3650 50  0001 C CNN
F 1 "GND" V 3405 3772 50  0000 R CNN
F 2 "" H 3400 3900 50  0001 C CNN
F 3 "" H 3400 3900 50  0001 C CNN
	1    3400 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	3400 3900 3900 3900
$Comp
L power:GND #PWR?
U 1 1 61714CC2
P 4950 2200
AR Path="/61714CC2" Ref="#PWR?"  Part="1" 
AR Path="/6170A1B9/61714CC2" Ref="#PWR0159"  Part="1" 
F 0 "#PWR0159" H 4950 1950 50  0001 C CNN
F 1 "GND" V 4955 2072 50  0000 R CNN
F 2 "" H 4950 2200 50  0001 C CNN
F 3 "" H 4950 2200 50  0001 C CNN
	1    4950 2200
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4950 2200 4400 2200
$Comp
L power:GND #PWR?
U 1 1 61714CC9
P 4950 2600
AR Path="/61714CC9" Ref="#PWR?"  Part="1" 
AR Path="/6170A1B9/61714CC9" Ref="#PWR0160"  Part="1" 
F 0 "#PWR0160" H 4950 2350 50  0001 C CNN
F 1 "GND" V 4955 2472 50  0000 R CNN
F 2 "" H 4950 2600 50  0001 C CNN
F 3 "" H 4950 2600 50  0001 C CNN
	1    4950 2600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4400 2600 4950 2600
$Comp
L power:GND #PWR?
U 1 1 61714CD0
P 4950 2900
AR Path="/61714CD0" Ref="#PWR?"  Part="1" 
AR Path="/6170A1B9/61714CD0" Ref="#PWR0161"  Part="1" 
F 0 "#PWR0161" H 4950 2650 50  0001 C CNN
F 1 "GND" V 4955 2772 50  0000 R CNN
F 2 "" H 4950 2900 50  0001 C CNN
F 3 "" H 4950 2900 50  0001 C CNN
	1    4950 2900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4400 2900 4950 2900
$Comp
L power:GND #PWR?
U 1 1 61714CD7
P 4950 3400
AR Path="/61714CD7" Ref="#PWR?"  Part="1" 
AR Path="/6170A1B9/61714CD7" Ref="#PWR0162"  Part="1" 
F 0 "#PWR0162" H 4950 3150 50  0001 C CNN
F 1 "GND" V 4955 3272 50  0000 R CNN
F 2 "" H 4950 3400 50  0001 C CNN
F 3 "" H 4950 3400 50  0001 C CNN
	1    4950 3400
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4950 3400 4400 3400
$Comp
L power:GND #PWR?
U 1 1 61714CDE
P 4950 3600
AR Path="/61714CDE" Ref="#PWR?"  Part="1" 
AR Path="/6170A1B9/61714CDE" Ref="#PWR0163"  Part="1" 
F 0 "#PWR0163" H 4950 3350 50  0001 C CNN
F 1 "GND" V 4955 3472 50  0000 R CNN
F 2 "" H 4950 3600 50  0001 C CNN
F 3 "" H 4950 3600 50  0001 C CNN
	1    4950 3600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	4950 3600 4400 3600
$Comp
L mainboard:+3.3V_PI #PWR?
U 1 1 61714CE5
P 3750 1850
AR Path="/61714CE5" Ref="#PWR?"  Part="1" 
AR Path="/6170A1B9/61714CE5" Ref="#PWR0164"  Part="1" 
F 0 "#PWR0164" H 3750 1640 50  0001 C CNN
F 1 "+3.3V_PI" H 3735 2025 50  0000 C CNN
F 2 "" H 3750 1850 50  0001 C CNN
F 3 "" H 3750 1850 50  0001 C CNN
	1    3750 1850
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x06 J?
U 1 1 61714CEB
P 3950 5550
AR Path="/61714CEB" Ref="J?"  Part="1" 
AR Path="/6170A1B9/61714CEB" Ref="J17"  Part="1" 
F 0 "J17" H 4030 5542 50  0000 L CNN
F 1 "RTC Module" H 4030 5451 50  0000 L CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x06_P2.54mm_Vertical" H 3950 5550 50  0001 C CNN
F 3 "~" H 3950 5550 50  0001 C CNN
	1    3950 5550
	1    0    0    -1  
$EndComp
$Comp
L power:+5V #PWR?
U 1 1 61714CF1
P 6100 2050
AR Path="/61714CF1" Ref="#PWR?"  Part="1" 
AR Path="/6170A1B9/61714CF1" Ref="#PWR0165"  Part="1" 
F 0 "#PWR0165" H 6100 1900 50  0001 C CNN
F 1 "+5V" H 6115 2223 50  0000 C CNN
F 2 "" H 6100 2050 50  0001 C CNN
F 3 "" H 6100 2050 50  0001 C CNN
	1    6100 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 61714CF7
P 6100 2250
AR Path="/61714CF7" Ref="C?"  Part="1" 
AR Path="/6170A1B9/61714CF7" Ref="C15"  Part="1" 
F 0 "C15" H 6215 2296 50  0000 L CNN
F 1 "10uF" H 6215 2205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6138 2100 50  0001 C CNN
F 3 "~" H 6100 2250 50  0001 C CNN
	1    6100 2250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C?
U 1 1 61714CFD
P 6500 2250
AR Path="/61714CFD" Ref="C?"  Part="1" 
AR Path="/6170A1B9/61714CFD" Ref="C16"  Part="1" 
F 0 "C16" H 6615 2296 50  0000 L CNN
F 1 "0.1uF" H 6615 2205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 6538 2100 50  0001 C CNN
F 3 "~" H 6500 2250 50  0001 C CNN
	1    6500 2250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 61714D03
P 6100 2450
AR Path="/61714D03" Ref="#PWR?"  Part="1" 
AR Path="/6170A1B9/61714D03" Ref="#PWR0166"  Part="1" 
F 0 "#PWR0166" H 6100 2200 50  0001 C CNN
F 1 "GND" H 6105 2277 50  0000 C CNN
F 2 "" H 6100 2450 50  0001 C CNN
F 3 "" H 6100 2450 50  0001 C CNN
	1    6100 2450
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 2450 6100 2400
Wire Wire Line
	6100 2100 6100 2050
Wire Wire Line
	6100 2100 6500 2100
Connection ~ 6100 2100
Wire Wire Line
	6100 2400 6500 2400
Connection ~ 6100 2400
NoConn ~ 3750 5350
NoConn ~ 3750 5450
Text GLabel 3700 5650 0    50   Input ~ 0
PI_SDA1
Text GLabel 3700 5550 0    50   Input ~ 0
PI_SCL1
Wire Wire Line
	3700 5550 3750 5550
Wire Wire Line
	3750 5650 3700 5650
$Comp
L power:GND #PWR?
U 1 1 61714D15
P 3700 5900
AR Path="/61714D15" Ref="#PWR?"  Part="1" 
AR Path="/6170A1B9/61714D15" Ref="#PWR0167"  Part="1" 
F 0 "#PWR0167" H 3700 5650 50  0001 C CNN
F 1 "GND" H 3705 5727 50  0000 C CNN
F 2 "" H 3700 5900 50  0001 C CNN
F 3 "" H 3700 5900 50  0001 C CNN
	1    3700 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3700 5900 3700 5850
Wire Wire Line
	3700 5850 3750 5850
$Comp
L mainboard:+3.3V_PI #PWR?
U 1 1 61714D1D
P 3150 5750
AR Path="/61714D1D" Ref="#PWR?"  Part="1" 
AR Path="/6170A1B9/61714D1D" Ref="#PWR0168"  Part="1" 
F 0 "#PWR0168" H 3150 5540 50  0001 C CNN
F 1 "+3.3V_PI" H 3135 5925 50  0000 C CNN
F 2 "" H 3150 5750 50  0001 C CNN
F 3 "" H 3150 5750 50  0001 C CNN
	1    3150 5750
	1    0    0    -1  
$EndComp
$Comp
L Jumper:SolderJumper_2_Open JP?
U 1 1 61714D23
P 3500 5750
AR Path="/61714D23" Ref="JP?"  Part="1" 
AR Path="/6170A1B9/61714D23" Ref="JP6"  Part="1" 
F 0 "JP6" H 2850 5650 50  0000 C CNN
F 1 "PI-RTC_PWR_EN" H 3250 5650 50  0000 C CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Open_TrianglePad1.0x1.5mm" H 3500 5750 50  0001 C CNN
F 3 "~" H 3500 5750 50  0001 C CNN
	1    3500 5750
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 5750 3350 5750
Wire Wire Line
	3650 5750 3750 5750
Text GLabel 2350 2050 2    50   Input ~ 0
PI_SDA1
Text GLabel 2350 2200 2    50   Input ~ 0
PI_SCL1
$Comp
L Device:R_Small_US R17
U 1 1 6172223C
P 2150 2050
F 0 "R17" V 1950 2050 50  0000 C CNN
F 1 "4.7k" V 2050 2050 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 2150 2050 50  0001 C CNN
F 3 "~" H 2150 2050 50  0001 C CNN
	1    2150 2050
	0    1    1    0   
$EndComp
$Comp
L Device:R_Small_US R18
U 1 1 61722485
P 2150 2200
F 0 "R18" V 2250 2200 50  0000 C CNN
F 1 "4.7k" V 2350 2200 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric" H 2150 2200 50  0001 C CNN
F 3 "~" H 2150 2200 50  0001 C CNN
	1    2150 2200
	0    1    1    0   
$EndComp
Wire Wire Line
	2250 2050 2350 2050
Wire Wire Line
	2350 2200 2250 2200
$Comp
L mainboard:+3.3V_PI #PWR?
U 1 1 61725A96
P 1850 1850
AR Path="/61725A96" Ref="#PWR?"  Part="1" 
AR Path="/6170A1B9/61725A96" Ref="#PWR0169"  Part="1" 
F 0 "#PWR0169" H 1850 1640 50  0001 C CNN
F 1 "+3.3V_PI" H 1835 2025 50  0000 C CNN
F 2 "" H 1850 1850 50  0001 C CNN
F 3 "" H 1850 1850 50  0001 C CNN
	1    1850 1850
	1    0    0    -1  
$EndComp
Wire Wire Line
	1850 1850 1850 2050
Wire Wire Line
	1850 2200 2050 2200
Wire Wire Line
	2050 2050 1850 2050
Connection ~ 1850 2050
Wire Wire Line
	1850 2050 1850 2200
$EndSCHEMATC