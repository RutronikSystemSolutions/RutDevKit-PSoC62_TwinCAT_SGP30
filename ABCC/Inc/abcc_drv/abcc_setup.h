/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Driver version 5.05.02 (2018-08-30)                                   **
**                                                                            **
** Delivered with:                                                            **
**    ABP            7.59.01 (2018-05-17)                                     **
**                                                                            */
/*******************************************************************************
********************************************************************************
** COPYRIGHT NOTIFICATION (c) 2013 HMS Industrial Networks AB                 **
**                                                                            **
** This program is the property of HMS Industrial Networks AB.                **
** It may not be reproduced, distributed, or used without permission          **
** of an authorized company official.                                         **
********************************************************************************
********************************************************************************
** file_description
********************************************************************************
********************************************************************************
** Services:
** ABCC_SetupInit()           : Init setup state amchine
** ABCC_SetupCommands()       : Send next command
** ABCC_SetupResponses()      : Handle setup comand response
********************************************************************************
********************************************************************************
*/

#ifndef ABCC_SETUP_H_
#define ABCC_SETUP_H_

#include "abcc_drv_cfg.h"
#include "abcc_td.h"
#include "abp.h"

/*******************************************************************************
** Constants
********************************************************************************
*/

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Inititsiate internal varaibles used by the setup state machine
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SetupInit( void );

/*------------------------------------------------------------------------------
** Start command sequence to be run during ABCC setup state.
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_StartSetup( void );



#endif  /* inclusion lock */