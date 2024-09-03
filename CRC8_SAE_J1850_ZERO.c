/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>
#include <stdbool.h>
#include "r_cg_userdefine.h"

#include "misc_config.h"
#include "custom_func.h"

#include "CRC8_SAE_J1850_ZERO.h"

/*_____ D E C L A R A T I O N S ____________________________________________*/

// struct flag_8bit flag_CRC8_CTL;
// #define FLAG_CRC8_REVERSE0                 	    		    (flag_CRC8_CTL.bit0)
// #define FLAG_CRC8_REVERSE1                      			(flag_CRC8_CTL.bit1)
// #define FLAG_CRC8_REVERSE2                 	                (flag_CRC8_CTL.bit2)
// #define FLAG_CRC8_REVERSE3                           		(flag_CRC8_CTL.bit3)
// #define FLAG_CRC8_REVERSE4                         		    (flag_CRC8_CTL.bit4)
// #define FLAG_CRC8_REVERSE5                                  (flag_CRC8_CTL.bit5)
// #define FLAG_CRC8_REVERSE6                                  (flag_CRC8_CTL.bit6)
// #define FLAG_CRC8_REVERSE7                                  (flag_CRC8_CTL.bit7)


/*_____ D E F I N I T I O N S ______________________________________________*/


#define REFLECTED_INPUT_BITS_NUM                            (8U)
#define REFLECTED_OUTPUT_BITS_NUM                           (8U)

#define INITIAL_CRC8_VALUE                                  (0x0U)
#define FINAL_XOR_VALUE                                     (0x0U)
#define REFLECTED_OUTPUT                                    (false)
#define REFLECTED_INPUT                                     (false)


/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

/*
    https://github.com/IMProject/IMUtility/tree/2b16a07c91539e5b1795b6c93cdd4faf69e5fb1d/Src/crc/crc8_variants
*/

/* Table for CRC-8 SAE J1850 ZERO  (Polynomial 0x1D)*/
static const unsigned char crc_table[256] = 
{
    0x00U, 0x1DU, 0x3AU, 0x27U, 0x74U, 0x69U, 0x4EU, 0x53U, 0xE8U, 0xF5U, 0xD2U, 0xCFU, 0x9CU, 0x81U, 0xA6U, 0xBBU,
    0xCDU, 0xD0U, 0xF7U, 0xEAU, 0xB9U, 0xA4U, 0x83U, 0x9EU, 0x25U, 0x38U, 0x1FU, 0x02U, 0x51U, 0x4CU, 0x6BU, 0x76U,
    0x87U, 0x9AU, 0xBDU, 0xA0U, 0xF3U, 0xEEU, 0xC9U, 0xD4U, 0x6FU, 0x72U, 0x55U, 0x48U, 0x1BU, 0x06U, 0x21U, 0x3CU,
    0x4AU, 0x57U, 0x70U, 0x6DU, 0x3EU, 0x23U, 0x04U, 0x19U, 0xA2U, 0xBFU, 0x98U, 0x85U, 0xD6U, 0xCBU, 0xECU, 0xF1U,
    0x13U, 0x0EU, 0x29U, 0x34U, 0x67U, 0x7AU, 0x5DU, 0x40U, 0xFBU, 0xE6U, 0xC1U, 0xDCU, 0x8FU, 0x92U, 0xB5U, 0xA8U,
    0xDEU, 0xC3U, 0xE4U, 0xF9U, 0xAAU, 0xB7U, 0x90U, 0x8DU, 0x36U, 0x2BU, 0x0CU, 0x11U, 0x42U, 0x5FU, 0x78U, 0x65U,
    0x94U, 0x89U, 0xAEU, 0xB3U, 0xE0U, 0xFDU, 0xDAU, 0xC7U, 0x7CU, 0x61U, 0x46U, 0x5BU, 0x08U, 0x15U, 0x32U, 0x2FU,
    0x59U, 0x44U, 0x63U, 0x7EU, 0x2DU, 0x30U, 0x17U, 0x0AU, 0xB1U, 0xACU, 0x8BU, 0x96U, 0xC5U, 0xD8U, 0xFFU, 0xE2U,
    0x26U, 0x3BU, 0x1CU, 0x01U, 0x52U, 0x4FU, 0x68U, 0x75U, 0xCEU, 0xD3U, 0xF4U, 0xE9U, 0xBAU, 0xA7U, 0x80U, 0x9DU,
    0xEBU, 0xF6U, 0xD1U, 0xCCU, 0x9FU, 0x82U, 0xA5U, 0xB8U, 0x03U, 0x1EU, 0x39U, 0x24U, 0x77U, 0x6AU, 0x4DU, 0x50U,
    0xA1U, 0xBCU, 0x9BU, 0x86U, 0xD5U, 0xC8U, 0xEFU, 0xF2U, 0x49U, 0x54U, 0x73U, 0x6EU, 0x3DU, 0x20U, 0x07U, 0x1AU,
    0x6CU, 0x71U, 0x56U, 0x4BU, 0x18U, 0x05U, 0x22U, 0x3FU, 0x84U, 0x99U, 0xBEU, 0xA3U, 0xF0U, 0xEDU, 0xCAU, 0xD7U,
    0x35U, 0x28U, 0x0FU, 0x12U, 0x41U, 0x5CU, 0x7BU, 0x66U, 0xDDU, 0xC0U, 0xE7U, 0xFAU, 0xA9U, 0xB4U, 0x93U, 0x8EU,
    0xF8U, 0xE5U, 0xC2U, 0xDFU, 0x8CU, 0x91U, 0xB6U, 0xABU, 0x10U, 0x0DU, 0x2AU, 0x37U, 0x64U, 0x79U, 0x5EU, 0x43U,
    0xB2U, 0xAFU, 0x88U, 0x95U, 0xC6U, 0xDBU, 0xFCU, 0xE1U, 0x5AU, 0x47U, 0x60U, 0x7DU, 0x2EU, 0x33U, 0x14U, 0x09U,
    0x7FU, 0x62U, 0x45U, 0x58U, 0x0BU, 0x16U, 0x31U, 0x2CU, 0x97U, 0x8AU, 0xADU, 0xB0U, 0xE3U, 0xFEU, 0xD9U, 0xC4U
};


unsigned long Utils_BitReflect(unsigned long data, unsigned char n_bits) 
{

    unsigned long reflection = 0u;
    unsigned long temp_data = data;

    /*
    * Reflect the data around the center bit.
    */
    for (unsigned char bit = 0u; bit < n_bits; ++bit) 
    {
        /*
        * If the LSB bit is set, set the reflection of it.
        */
        if (1u == (temp_data & 1u) ) 
        {
            /* -E> compliant MC3R1.R12.2 1 The shift count is granted to be between 0 and 31 due to bit masking. */
            reflection |= (unsigned long)((unsigned long)1U << (0x1FU & ((n_bits - 1U) - bit)));
        }

        temp_data = (temp_data >> 1u);
    }

    return reflection;

}


unsigned char Crc8Base(
    const unsigned char crc_table[256],
    const unsigned char* crc_data_ptr,
    unsigned long crc_length,
    unsigned char crc_initial_value,
    unsigned char final_xor_value,
    bool reflected_output,
    bool reflected_input,
    bool final_xor) 
{

    unsigned char crc = crc_initial_value;
    const unsigned char* temp_data_ptr = crc_data_ptr;

    for (unsigned long counter = 0U; counter < crc_length; ++counter) 
    {
        unsigned char temp;

        if (reflected_input) 
        {
            temp = (unsigned char)Utils_BitReflect(*temp_data_ptr, REFLECTED_INPUT_BITS_NUM);
        } 
        else 
        {
            temp = *temp_data_ptr;
        }

        crc = crc ^ temp;
        crc = crc_table[crc];
        ++temp_data_ptr;
    }

    if (reflected_output) 
    {
        crc = (unsigned char)Utils_BitReflect((unsigned long)crc, REFLECTED_OUTPUT_BITS_NUM);
    }

    if (final_xor) 
    {
        crc ^= final_xor_value;
    }
    return crc;
}



unsigned char Crc8_saeJ1850Zero(const unsigned char* crc_data_ptr,unsigned long crc_length) 
{
    return Crc8Base(
               crc_table,
               crc_data_ptr,
               crc_length,
               INITIAL_CRC8_VALUE,
               FINAL_XOR_VALUE,
               REFLECTED_OUTPUT,
               REFLECTED_INPUT,
               false
           );
}


