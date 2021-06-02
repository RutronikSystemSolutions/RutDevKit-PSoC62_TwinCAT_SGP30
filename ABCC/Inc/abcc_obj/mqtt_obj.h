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
** This is the public header file for the MQTT object.
********************************************************************************
********************************************************************************
** Services:
**    MQTT_ProcessCmdMsg()              - Processes commands sent to the MQTT
**                                        object.
**    MQTT_GetPublishConfigurationCfn() - Callback function to get the publish
**                                        configuration of a dataset.
********************************************************************************
********************************************************************************
*/
#ifndef MQTT_OBJ_H
#define MQTT_OBJ_H

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

#if MQTT_CMD_PUBLISH_CFG_ENABLE
/*------------------------------------------------------------------------------
** Retrieves the Publish configuration of a requested dataset.
**------------------------------------------------------------------------------
** Arguments:
**    eDatasetType       - Dataset type.
**    iDatasetIdentifier - Dataset identifier
**    pfRetainBit        - Pointer to retain bit buffer. Assign to configured
**                         Retain bit state of the specified dataset.
**    ppcTopicString     - Pointer to pointer to reference to a topic string.
**                         Assign to topic string configured for the specified
**                         dataset.
**
** Returns:
**    ABP_ERR_NO_ERROR if the dataset was identified and a publish configuration
**    exists (pfRetainBit and ppcTopicString assigned).
**    For unknown or not supported dataset types, respond with
**    ABP_ERR_INV_CMD_EXT_0. If the dataset identifier is unkown or not
**    supported respond with ABP_ERR_OUT_OF_RANGE.
**------------------------------------------------------------------------------
*/
ABP_MsgErrorCodeType MQTT_GetPublishConfigurationCfn( ABP_AppDatasetType eDatasetType,
                                                      UINT16 iDatasetIdentifier,
                                                      BOOL* pfRetainBit,
                                                      const char** ppcTopicString );
#endif /* #if MQTT_CMD_PUBLISH_CFG_ENABLE */

/*------------------------------------------------------------------------------
** The function that processes the commands sent to the MQTT object.
**------------------------------------------------------------------------------
** Arguments:
**    psNewMessage      - Pointer to a ABP_MsgType message.
**
** Returns:
**    None.
**------------------------------------------------------------------------------
*/
EXTFUNC void MQTT_ProcessCmdMsg( ABP_MsgType* psNewMessage );

#endif  /* inclusion lock */
