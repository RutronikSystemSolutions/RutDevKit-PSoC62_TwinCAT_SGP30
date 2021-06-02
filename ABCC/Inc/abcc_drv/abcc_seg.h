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
** Implements the ABCC segmentation protocol.
********************************************************************************
********************************************************************************
** Services:
** ABCC_SegmentationInit()    : Init segmenation handler
** ABCC_SegmentationHandler() : Implements the segmentation protocol
********************************************************************************
********************************************************************************
*/

#ifndef ABCC_SEG_H_
#define ABCC_SEG_H_

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
** Init internal variables
**------------------------------------------------------------------------------
** Arguments:
**       None.
**
** Returns:
**       None.
**------------------------------------------------------------------------------
*/
EXTFUNC void ABCC_SegmentationInit( void );

/*------------------------------------------------------------------------------
** Checks if a received command message is related to an ongoing segmentation
** session. If that is the case the response is handled by this function.
** Note that the segmentation session is started by the user by calling
** ABCC_StartServerRespSegmentationSession() declared in abcc.h.
**------------------------------------------------------------------------------
** Arguments:
**       psMsg - Pointer to abcc command message
**
** Returns:
**       TRUE  - Message related to a segmentation session and handled by this
**               function.
**       FALSE - Message is not related to a segmentation session.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ABCC_HandleSegmentAck( ABP_MsgType* psMsg );

#endif  /* inclusion lock */
