/*_____ I N C L U D E S ____________________________________________________*/
#include <stdio.h>

/*_____ D E C L A R A T I O N S ____________________________________________*/

/*_____ D E F I N I T I O N S ______________________________________________*/
#define IICA0_TIMEOUT_LIMIT                             (0xFFFF)


/*_____ M A C R O S ________________________________________________________*/

/*_____ F U N C T I O N S __________________________________________________*/

void drv_IICA0_callback_error(unsigned char err);
void drv_set_IICA0_receive_flag(bool flag);
bool drv_get_IICA0_receive_flag(void);
void drv_set_IICA0_send_flag(bool flag);
bool drv_get_IICA0_send_flag(void);

