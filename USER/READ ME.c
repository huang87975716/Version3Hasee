/*when MotorRowStop() and MotorColStop() are used, Delay_us(500) is needed after.
*/
/*Motor--GPIO
	RankRight_1 -- PA4-
	RankRight_2 -- PE7-
	RankRight_3 -- PE8-
	RankRight_4 -- PE9-
	RankRight_5 -- PE10-
	RankRight_6 -- PB10-
	RankRight_7 -- PB11-
	RankRight_8 -- PB13-
	RankRight_9 -- PB15-
	RankRight_10 -- PB12
	RankRight_11 -- PB14
	
	LineRight_1 -- PC11
	LineRight_2 -- PD4
	LineRight_3 -- PE3
	LineRight_4 -- PE2
	LineRight_5 -- PE5
	LineRight_6 -- PE4
	LineRight_7 -- PC13
	LineRight_8 -- PE6
	LineRight_9 -- PC15
	LineRight_10 -- PC14
	LineRight_11 -- PA0

	RankContrary_1 -- PD8-
	RankContrary_2 -- PD10-
	RankContrary_3 -- PD12-
	RankContrary_4 -- PD14-
	RankContrary_5 -- PC6-
	RankContrary_6 -- PC8-
	RankContrary_7 -- PA8-
	RankContrary_8 -- PA10-
	RankContrary_9 -- PA12-
	RankContrary_10 -- P7 of I2C addressed at 0x42
	RankContrary_11 -- P6 of I2C addressed at 0x42
	
	LineContrary_1 -- PC12
	LineContrary_2 -- PC10
	LineContrary_3 -- PA15
	LineContrary_4 -- PA11
	LineContrary_5 -- PA9
	LineContrary_6 -- PC9
	LineContrary_7 -- PC7
	LineContrary_8 -- PD15
	LineContrary_9 -- PD13
	LineContrary_10 -- PD11
	LineContrary_11 -- PD9
*/

/*I2C GPIO	
	SDA -- PB6
	SCL -- PB7
*/

/*ELS GPIO
	ELS1 -- PD6
	ELS2 -- PD5
	ELS3 -- PB3
	ELS4 -- PD7
	ELS5 -- PB5
	ELS6 -- PB4
	ELS7 -- PB9
	ELS8 -- PB8
	ELS9 -- PE1
	ELS0 -- PE0
	ELS11 -- P5 of I2C addressed at 0x42
*/
/*protocol for master
AA(header) XX(command) XX(data0) XX(data1) XX(data2) XX(data3) XX(checksum)	
*/

/*Optical Coupler Interface and L298 drive interface
PE13	SW1		S_SEN1
PD2		SW4		S_SEN2
PE14	SW2		S_SEN3
PD3		SW3		S_SEN4

PE12	TC_S1
PE11	TC_S2
PA1		TC_EN
*/

/*
For PCF8574 addressed 0x42, p7~p4 is pulled up as hign volatage, becasue of the exitance of triode, the readout is 0;
the optical coupler receive occupy about 258-79 about 160mA
the main problem is the relay controlled by triode, which is now solved by the MOSFET
*/
