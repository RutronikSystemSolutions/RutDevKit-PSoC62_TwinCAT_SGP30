/*******************************************************************************
********************************************************************************
**                                                                            **
** ABCC Starter Kit version 3.05.02 (2018-08-30)                              **
**                                                                            **
** Delivered with:                                                            **
**    ABP            7.59.01 (2018-05-17)                                     **
**    ABCC Driver    5.05.02 (2018-08-30)                                     **
**                                                                            */
/*******************************************************************************
********************************************************************************
** COPYRIGHT NOTIFICATION (c) 2018 HMS Industrial Networks AB                 **
**                                                                            **
** This program is the property of HMS Industrial Networks AB.                **
** It may not be reproduced, distributed, or used without permission          **
** of an authorized company official.                                         **
********************************************************************************
********************************************************************************
** Assembly Mapping Object 0xEB - Public interfaces
********************************************************************************
********************************************************************************
** Services:
**   ASM_GetData()       - Get data for an assembly instance.
**   ASM_Init()          - Initializes the assembly mapping object.
**   ASM_ProcessCmdMsg() - Processes a message addressed to this object.
********************************************************************************
********************************************************************************
*/

#ifndef ASM_OBJ_H_
#define ASM_OBJ_H_

#include "abcc_td.h"

/*******************************************************************************
** Constants
********************************************************************************
*/

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Structure representing an assembly mapping instance.
**------------------------------------------------------------------------------
*/
typedef struct ASM_Instance
{
   const UINT32 lDescriptor;
   const AD_MapType* psMap;
#if ASM_IA_NAME_ENABLE
   const char* pacName;
#endif
}
ASM_InstanceType;

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Get assembly data for an instance.
**------------------------------------------------------------------------------
** Arguments:
**    iInstance     - Assembly mapping instance.
**    pxBuffer      - Buffer to put the assembly data in.
**    piOctetOffset - Offset in octets in pxBuffer where the data will be
**                    written to. This offset will be incremented according to
**                    the size written.
**
** Returns:
**    TRUE - Data was successfully written to destionation buffer.
**    FALSE - Invalid assembly mapping instance.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL ASM_GetData( UINT16 iInstance,
                          void* pxBuffer,
                          UINT16* piOctetOffset );

/*------------------------------------------------------------------------------
** Initializes the Assembly Mapping object.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ASM_Init( void );

/*------------------------------------------------------------------------------
** Process an object request against the Assembly Mapping Object.
**------------------------------------------------------------------------------
** Arguments:
**    psMsgBuffer - Pointer to ABCC command message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void ASM_ProcessCmdMsg( ABP_MsgType* psNewMessage );

#endif  /* inclusion lock */
