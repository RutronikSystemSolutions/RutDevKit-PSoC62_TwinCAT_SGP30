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
** COPYRIGHT NOTIFICATION (c) 2015 HMS Industrial Networks AB                 **
**                                                                            **
** This code is the property of HMS Industrial Networks AB.                   **
** The source code may not be reproduced, distributed, or used without        **
** permission. When used together with a product from HMS, permission is      **
** granted to modify, reproduce and distribute the code in binary form        **
** without any restrictions.                                                  **
**                                                                            **
** THE CODE IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND. HMS DOES NOT    **
** WARRANT THAT THE FUNCTIONS OF THE CODE WILL MEET YOUR REQUIREMENTS, OR     **
** THAT THE OPERATION OF THE CODE WILL BE UNINTERRUPTED OR ERROR-FREE, OR     **
** THAT DEFECTS IN IT CAN BE CORRECTED.                                       **
********************************************************************************
********************************************************************************
** This is the public header file for the APP object.
********************************************************************************
********************************************************************************
** Services:
**    APP_GetCandidateFwAvailable() - Checks if there is an firmware available
**                                    in the module's candidate area.
**    APP_ProcResetRequest()        - Performs a reset of the application
**    APP_ProcessCmdMsg()           - Processes commands sent to the APP object.
**    APP_HwConfAddress()           - Set attribute ABP_APP_IA_HW_CONF_ADDR.
********************************************************************************
********************************************************************************
*/
#ifndef APP_OBJ_H
#define APP_OBJ_H

#include "abp.h"
#include "abcc_obj_cfg.h"

/*******************************************************************************
** Constants
********************************************************************************
*/

#define APP_DATA_NOTIF_HEAD_SIZE 18
#define APP_DATA_NOTIF_TAIL_SIZE 8

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*------------------------------------------------------------------------------
** States used when building the data notification response message.
**------------------------------------------------------------------------------
*/
typedef enum
{
   APP_DATA_NOTIF_HEAD,
   APP_DATA_NOTIF_VALUE,
   APP_DATA_NOTIF_TAIL,
   APP_DATA_NOTIF_DONE
}
APP_DataNotifDescStateType;

/*------------------------------------------------------------------------------
** Type to describe and identify a data notification response message. The
** content is initiated and used by the internal functions.
**------------------------------------------------------------------------------
*/
typedef struct APP_DataNotifDesc
{
   ABP_AppDatasetType eDataset;
   APP_DataNotifDescStateType eState;
   UINT8* pbNotifEntryValue;
   UINT32 lNotifEntryValueSize;
   UINT8  abHead[ APP_DATA_NOTIF_HEAD_SIZE ];
   UINT32 lHeadSize;
   UINT8  abTail[ APP_DATA_NOTIF_TAIL_SIZE ];
   UINT32 lTailSize;
}
APP_DataNotifDescType;

/*******************************************************************************
** Public Globals
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/

/*------------------------------------------------------------------------------
** Call to check if there is firmware available in the candidate area. This
** function retrieves the value from a NVS.
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    BOOL                 - TRUE: Firmware available in candidate area.
**                           FALSE: Firmware NOT available in candidate area.
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL APP_GetCandidateFwAvailable( void );


/*------------------------------------------------------------------------------
** Set attribute ABP_APP_IA_HW_CONF_ADDR to indicate if the address is set by
** hardware switches or not.
**------------------------------------------------------------------------------
** Arguments:
**    fhwConfAddress:   - TRUE if address is set by HW switches.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void APP_HwConfAddress( BOOL fhwConfAddress );


/*------------------------------------------------------------------------------
** Called to perform a reset upon request.
**------------------------------------------------------------------------------
** Arguments:
**    bResetType        - Type of reset, see ABP_RESET_XXX defines.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void APP_ProcResetRequest( UINT8 bResetType );

/*------------------------------------------------------------------------------
** The function that processes the commands sent to the APP object.
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void APP_ProcessCmdMsg( ABP_MsgType* psNewMessage );

#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE

/*------------------------------------------------------------------------------
** Function that can be used to prepare a single ADI to be published
** on a supported network. The function requires the published AD instance and a
** pointer to a buffer that holds the Adi value.
** If only a part of the ADI is published the number of elements and start index
** must be also be provided. Number of elements set to 0 means that the whole
** ADI is published.
** The timestamp is omitted from the response if set to 0.
**------------------------------------------------------------------------------
** Arguments:
**    psDataNotifDesc               - Identifies data notification instance.
**                                    Initiated by prepare function.
**    pbAdiValue                    - Pointer to ADI value to publish.
**    iValueSize                    - The ADI value size in octets.
**    iNwChannel                    - Network channel, see ABP_APP_NOTIFENTRY_
**                                    defines in abp.h.
**    iInstance                     - ADI instance to publish.
**    bStartIndex                   - Start index of published data.
**                                    If bNumElements = 0 this value is
**                                    don't care.
**    bNumElements                  - Number of elements in ADI value,
**                                    0 = All elements.
**    lTimestampLow                 - Timestamp, nanoseconds since
**                                    00:00:00 (UTC), 1 January 1970
**                                    low 32 bits.
**    lTimestampHigh                - Timestamp, high 32 bits
**                                    If lTimestampLow and lTimestampHigh = 0
**                                    no timestamp will be published.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void APP_PrepareDataNotifSingleAdi( APP_DataNotifDescType* psDataNotifDesc,
                                            UINT8* pbAdiValue,
                                            UINT16 iValueSize,
                                            UINT16 iNwChannel,
                                            UINT16 iInstance,
                                            UINT8 bStartIndex,
                                            UINT8 bNumElements,
                                            UINT32 lTimestampLow,
                                            UINT32 lTimestampHigh );
#endif /* APP_CMD_GET_DATA_NOTIFICATION_ENABLE */

#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE && ASM_OBJ_ENABLE
/*------------------------------------------------------------------------------
** Function that can be used to prepare an assembly instance to be published
** on a supported network. The function requires the published Assembly mapping
** instance and a pointer to a buffer that holds the assembly value.
** The timestamp is omitted from the response if set to 0.
**------------------------------------------------------------------------------
** Arguments:
**    psDataNotifDesc               - Identifies data notification instance.
**                                    Initiated by prepare function.
**    pabAsmValueBuffer             - Pointer to assemby value to publish
**    iValueSize                    - The assembly value size in octets.
**    iNwChannel                    - Network channels the notification shall
**                                    target, see the ABP_APP_NOTIFENTRY_
**                                    defines in abp.h.
**    iInstance                     - Assembly Mapping instance to publish.
**    lTimestampLow                 - Timestamp, nanoseconds since
**                                    00:00:00 (UTC), 1 January 1970
**                                    low 32 bits.
**    lTimestampHigh                - Timestamp, high 32 bits
**                                    If lTimestampLow and lTimestampHigh = 0
**                                    no timestamp will be published.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void APP_PrepareDataNotifAsmInst( APP_DataNotifDescType* psDataNotifDesc,
                                          UINT8* pabAsmValueBuffer,
                                          UINT16 iValueSize,
                                          UINT16 iNwChannel,
                                          UINT16 iInstance,
                                          UINT32 lTimestampLow,
                                          UINT32 lTimestampHigh );
#endif /* APP_CMD_GET_DATA_NOTIFICATION_ENABLE */

#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE
/*------------------------------------------------------------------------------
** Function that can be used to prepare transparent data to be published
** on a supported network. The function requires a dataset identifier and a
** pointer to a buffer that holds the transparent payload.
**------------------------------------------------------------------------------
** Arguments:
**    psDataNotifDesc               - Identifies data notification instance.
**                                    Initiated by prepare function.
**    pxTransparentPayload          - Pointer to the data to be published.
**    iPayloadSize                  - The size of the payload in octets.
**    iNwChannel                    - Network channel, see ABP_APP_NOTIFENTRY_
**                                    defines in abp.h.
**    iIdentifierTransparent        - Dataset identifier defined by the user
**                                    The identifier of a transparent network
**                                    payload is not related to any Anybus
**                                    object. The host application must ensure
**                                    that the identifier is unique within the
**                                    set of defined transparent datasets.
**
** Returns:
**    Number of octets added to the buffer
**------------------------------------------------------------------------------
*/
EXTFUNC void APP_PrepareDataNotifTransparent( APP_DataNotifDescType* psDataNotifDesc,
                                              UINT8* pxTransparentPayload,
                                              UINT16 iPayloadSize,
                                              UINT16 iNwChannel,
                                              UINT16 iIdentifierTransparent );

#endif /* APP_CMD_GET_DATA_NOTIFICATION_ENABLE */

#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE
/*------------------------------------------------------------------------------
** Get value pointer from data notification descriptor instance.
**------------------------------------------------------------------------------
** Arguments:
**    psDataNotifDesc               - Identifies data notification instance.
**                                    Initiated by prepare function.
**
** Returns:
**    Pointer to value buffer
**------------------------------------------------------------------------------
*/
EXTFUNC UINT8* APP_GetDataNotifValuePtr( const APP_DataNotifDescType* psDataNotifDesc );

#endif /* APP_CMD_GET_DATA_NOTIFICATION_ENABLE */

#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE
/*------------------------------------------------------------------------------
** Can be used to publish a Dataset on a supported network once a Data
** Notification Request has been received (APP_OnDataNotificationReqCfn has been
** called). The function requires a data notification descriptor initiated by the
** corresponding prepare function.
**------------------------------------------------------------------------------
** Arguments:
**    psDataNotifDesc               - Identifies data notification instance.
**                                    Initiated by prepare function.
**    pnDone                        - Function to be called when the provided
**                                    data notification resources can be used again.
**                                    The psDataNotifDesc will be provided as the
**                                    pxObject (see ABCC_SegMsgHandlerDoneFuncType()
**                                    description in abcc.h) and the value pointer
**                                    can be retrieved by calling
**                                    APP_GetDataNotifValuePtr().
**
** Returns:
**    Number of data notification resources left, -1 = Out of resources.
**------------------------------------------------------------------------------
*/
EXTFUNC INT16  APP_SendDataNotification( APP_DataNotifDescType* psDataNotifDesc,
                                         ABCC_SegMsgHandlerDoneFuncType pnDone );
#endif /* APP_CMD_GET_DATA_NOTIFICATION_ENABLE */

#if APP_CMD_GET_DATA_NOTIFICATION_ENABLE
/*------------------------------------------------------------------------------
** Function called when Get Data Notification command is received.
** The default function name can be overridden by the user by defining
** ON_DATA_NOTIFICATION_REQ_CBF to the alternative function name.
**
**------------------------------------------------------------------------------
** Arguments:
**    iDataNotifCounter             - Counter of the number of available
**                                    Get Data Notification resources
**
** Returns:
**    Message error code
**------------------------------------------------------------------------------
*/
#ifndef APP_ON_DATA_NOTIFICATION_REQ_CBF
   #define APP_ON_DATA_NOTIFICATION_REQ_CBF APP_OnDataNotificationReqCfn
#endif
EXTFUNC ABP_MsgErrorCodeType APP_ON_DATA_NOTIFICATION_REQ_CBF( UINT16 iDataNotifCounter );

#endif /* APP_CMD_GET_DATA_NOTIFICATION_ENABLE */

#endif  /* inclusion lock */
