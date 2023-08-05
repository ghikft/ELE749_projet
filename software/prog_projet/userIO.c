#include "userIO.h"

void get_switche(alt_u32 sw_base, char* switchStatus)
{
	/**************************************************************************
	 * Read the state of the switch 
	 **************************************************************************
	 * Parameters
	 * sw_base : base adress of the switches
	 * switchStatus : pointer to the variable that contains the switch status
	 *
	 * Return
	 * none
	 *
	 * Side effects
	 * switchStatus : The pointed variable is modified
	 *
	 * Notes
	 * Apply a mask since there is only one switch at the lsb, everiting else 
	 * will be 0
	 *************************************************************************/

	* switchStatus = IORD(sw_base, 0) & 0x00000001;
}

void get_button(alt_u32 sw_base, char* buttonStatus)
{
	/**************************************************************************
	 * Read the state of the button
	 **************************************************************************
	 * Parameters
	 * sw_base : base adress of the switches
	 * buttonStatus : pointer to the variable that contains the button status
	 *
	 * Return
	 * none
	 *
	 * Side effects
	 * buttonStatus : The pointed variable is modified
	 *
	 * Notes
	 * Apply a mask since there is only one button at the lsb, everiting else
	 * will be 0
	 *************************************************************************/

	* buttonStatus = IORD(sw_base, 0) & 0x00000001;
}

void get_remote(alt_u32 sw_base, char* RemoteStatus)
{
	/**************************************************************************
	 * Read the state of the remote buttons
	 **************************************************************************
	 * Parameters
	 * sw_base : base adress of the switches
	 * RemoteStatus : pointer to the variable that contains the button status
	 *
	 * Return
	 * none
	 *
	 * Side effects
	 * RemoteStatus : The pointed variable is modified
	 *
	 * Notes
	 * Apply a mask since there is only 6 button on the remote, everiting else
	 * will be 0
	 *************************************************************************/

	* RemoteStatus = IORD(sw_base, 0) & 0x0000003F;
}