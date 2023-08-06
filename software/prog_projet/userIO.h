/* Description
* Library Read external user input
* Autor: Nicolas & Robin
*********************************************************************/
#ifndef USER_IO_h_
#define USER_IO_h_

#include "io.h"
#include "alt_types.h"
#include "sys/alt_irq.h"
#include "system.h"
# include "sys/alt_stdio.h"


void get_switche(alt_u32 sw_base, char* switchStatus);

void get_button(alt_u32 sw_base, char* buttonStatus);

void get_remote(alt_u32 sw_base, char* RemoteStatus);

#endif//end USER_IO_h_