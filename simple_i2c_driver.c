/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>
#include "r_cg_userdefine.h"
#include "r_smc_entry.h"

#include "misc_config.h"
#include "custom_func.h"
#include "simple_i2c_driver.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

struct flag_16bit flag_IICA0_CTL;
#define FLAG_IICA0_TRIG_IICA0_SEND                 		    (flag_IICA0_CTL.bit0)
#define FLAG_IICA0_TRIG_IICA0_RCV                       	(flag_IICA0_CTL.bit1)
#define FLAG_IICA0_REVERSE2                 				(flag_IICA0_CTL.bit2)
#define FLAG_IICA0_REVERSE3                      		    (flag_IICA0_CTL.bit3)
#define FLAG_IICA0_REVERSE4                       	    	(flag_IICA0_CTL.bit4)
#define FLAG_IICA0_REVERSE5                               	(flag_IICA0_CTL.bit5)
#define FLAG_IICA0_REVERSE6                               	(flag_IICA0_CTL.bit6)
#define FLAG_IICA0_REVERSE7                               	(flag_IICA0_CTL.bit7)

/*_____ D E F I N I T I O N S ______________________________________________*/

/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

// for driver level
MD_STATUS drv_IICA0_read(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num)
{
	MD_STATUS ret = MD_OK;

	ret = R_Config_IICA0_Master_Receive(adr, rx_buf, rx_num, 100);

	if (ret != MD_OK)
    {
        printf_tiny("[drv_IICA0_read error]0x%02X\r\n" , ret);
		return ret;
	} 
    return ret;
}


MD_STATUS drv_IICA0_write(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num)
{
	MD_STATUS ret = MD_OK;

    ret = R_Config_IICA0_Master_Send(adr, tx_buf , tx_num , 100);

	if (ret != MD_OK)
    {
        printf_tiny("[drv_IICA0_write error]0x%02X\r\n" , ret);
		return ret;
	} 
    return ret;
}

bool drv_Is_IICA0_bus_busy(void)
{
    return IICBSY0;
}

void drv_IICA0_callback_error(unsigned char err)
{
    R_Config_IICA0_StopCondition();
    printf_tiny("I2C MD_STATUS:0x%02X\r\n",err);
}

void drv_set_IICA0_receive_flag(bool flag)
{
    FLAG_IICA0_TRIG_IICA0_RCV = flag;
}
bool drv_get_IICA0_receive_flag(void)
{
    return FLAG_IICA0_TRIG_IICA0_RCV;
}

void drv_set_IICA0_send_flag(bool flag)
{
    FLAG_IICA0_TRIG_IICA0_SEND = flag;
}
bool drv_get_IICA0_send_flag(void)
{
    return FLAG_IICA0_TRIG_IICA0_SEND;
}

// for application

MD_STATUS IICA0_read(unsigned char device_addr,unsigned char reg_addr,unsigned char* rx_xfer_data,unsigned short rx_num)
{
	MD_STATUS ret = MD_OK;
    unsigned char tmp = 0;	
	unsigned int u32IICA0TimeOutCnt = 0;

    tmp = reg_addr;

    // while (drv_get_IICA0_send_flag() || drv_Is_IICA0_bus_busy()){ ; } 	//Make sure bus is ready for xfer
	
	drv_set_IICA0_send_flag(1);
	ret = drv_IICA0_write(device_addr, &tmp, 1);
	if (ret != MD_OK)
    {
        printf_tiny("[I2C read error1]0x%02X\r\n" , ret);
		return ret;
	} 
	
	u32IICA0TimeOutCnt = IICA0_TIMEOUT_LIMIT;
	while (drv_get_IICA0_send_flag() || drv_Is_IICA0_bus_busy())
    {
        u32IICA0TimeOutCnt--;
        if(u32IICA0TimeOutCnt == 0)
        {
            printf_tiny("%s bus busy*(xfer is complete),dev addr:0x%02X,reg addr:0x%02X\r\n",__func__,device_addr,reg_addr);
            return MD_BUSY2;
        }
    } 	//Wait until the xfer is complete

    // OUTPUT low to inform slave , master will start to read
    #if defined (ENALBE_SPECIFIC_FUNCTION)
    TOUCH_SYNC = 0;
    #endif

	drv_set_IICA0_receive_flag(1);
	ret = drv_IICA0_read(device_addr, rx_xfer_data, rx_num);
	
	u32IICA0TimeOutCnt = IICA0_TIMEOUT_LIMIT;
	while (drv_get_IICA0_receive_flag() || drv_Is_IICA0_bus_busy())
    {
        u32IICA0TimeOutCnt--;
        if(u32IICA0TimeOutCnt == 0)
        {
            printf_tiny("%s bus busy**(xfer is complete),dev addr:0x%02X,reg addr:0x%02X\r\n",__func__,device_addr,reg_addr);
            return MD_BUSY2;
        }
    } 	//Wait until the xfer is complete
	if (ret != MD_OK)
    {
        printf_tiny("[I2C read error2]0x%02X\r\n" , ret);
		return ret;
	} 
    
    // OUTPUT high after read
    #if defined (ENALBE_SPECIFIC_FUNCTION)
    TOUCH_SYNC = 1;
    #endif

    // delay_ms(5);

    return ret;
}

MD_STATUS IICA0_write(unsigned char device_addr,unsigned char reg_addr,unsigned char* tx_xfer_data,unsigned short tx_num)
{
	MD_STATUS ret = MD_OK;
    unsigned char i = 0;
    unsigned char buffer[128] = {0};	
	unsigned int u32IICA0TimeOutCnt = 0;

    if (tx_num > 128)
    {
        printf_tiny("[I2C_write]tx_num over range(%2d)\r\n",tx_num);

        return MD_ARGERROR;
    }

    buffer[0] = reg_addr;
    for ( i = 0 ; i < tx_num ; i++ )
    {
        buffer[i+1] = tx_xfer_data[i];
    }

    // while (drv_get_IICA0_send_flag() || drv_Is_IICA0_bus_busy()){ ; } 	//Make sure bus is ready for xfer
	
	drv_set_IICA0_send_flag(1);
	ret = drv_IICA0_write(device_addr, buffer , tx_num + 1);   // reg byte + data byte , at least 2 bytes
	if (ret != MD_OK)
    {
        printf_tiny("[I2C write error1]0x%02X\r\n" , ret);
		return ret;
	} 
	
	u32IICA0TimeOutCnt = IICA0_TIMEOUT_LIMIT;
	while (drv_get_IICA0_send_flag() || drv_Is_IICA0_bus_busy())
    {
        u32IICA0TimeOutCnt--;
        if(u32IICA0TimeOutCnt == 0)
        {
            printf_tiny("%s bus busy(xfer is complete),dev addr:0x%02X,reg addr:0x%02X\r\n",__func__,device_addr,reg_addr);
            return MD_BUSY2;
        }
    } 	//Wait until the xfer is complete
   
    // delay_ms(5);

    return ret;
}

