# RL78_G16_IICA0_I2C_Master
 RL78_G16_IICA0_I2C_Master

udpate @ 2024/09/03

1. initial G16 EVB , to test below function 

- UART : P03 , P04

- BUTTON : P137 

- IICA0 : SCLA0 : P60 ,  SDSA0 : P61

- use P41 for GPIO to sync with slave if necessary, check define : ENALBE_SPECIFIC_FUNCTION

- press button , will write I2C data , check IICA0_send_seq_cmd

	- write reg : 0x83 , below is cmd format

	- cmd 0 : 50h,83h,00h,crc
	
	- cmd 1~5 : 50h,83h,01h,data x 7,crc
	
	- cmd 6~10: 50h,83h,02h,data x 14,crc

- enable I2C read function if necessary , check define : ENABLE_I2C_RD

	- read reg : 0x00 , below is cmd format
	
	- slave will return , 00h,00h,5Ah,A5h,counter,00h,crc

2. below is LA capture 

WR command 0 

![image](https://github.com/released/RL78_G16_IICA0_I2C_Master/blob/main/LA_cmd_WR0.jpg)


WR command with data x 7 

![image](https://github.com/released/RL78_G16_IICA0_I2C_Master/blob/main/LA_cmd_WR1_1.jpg)

![image](https://github.com/released/RL78_G16_IICA0_I2C_Master/blob/main/LA_cmd_WR1_2.jpg)


WR command with data x 14 

![image](https://github.com/released/RL78_G16_IICA0_I2C_Master/blob/main/LA_cmd_WR2_1.jpg)

![image](https://github.com/released/RL78_G16_IICA0_I2C_Master/blob/main/LA_cmd_WR2_2.jpg)


RD command 
 
![image](https://github.com/released/RL78_G16_IICA0_I2C_Master/blob/main/LA_cmd_RD_1.jpg)

![image](https://github.com/released/RL78_G16_IICA0_I2C_Master/blob/main/LA_cmd_RD_2.jpg)

RD command , with enable SYNC pin
 
![image](https://github.com/released/RL78_G16_IICA0_I2C_Master/blob/main/LA_cmd_RD_with_SYNC.jpg)

3. below is LA timing measurement


WR command 0 (400K)

![image](https://github.com/released/RL78_G16_IICA0_I2C_Master/blob/main/LA_timing_0.jpg)

WR command 0 (100K)

![image](https://github.com/released/RL78_G16_IICA0_I2C_Master/blob/main/LA_timing_0_low_speed.jpg)


WR command with data x 7  (400K)

![image](https://github.com/released/RL78_G16_IICA0_I2C_Master/blob/main/LA_timing_1.jpg)


WR command with data x 7  (100K)

![image](https://github.com/released/RL78_G16_IICA0_I2C_Master/blob/main/LA_timing_1_low_speed.jpg)


WR command with data x 14  (400K)

![image](https://github.com/released/RL78_G16_IICA0_I2C_Master/blob/main/LA_timing_2.jpg)


WR command with data x 14  (100K)

![image](https://github.com/released/RL78_G16_IICA0_I2C_Master/blob/main/LA_timing_2_low_speed.jpg)

