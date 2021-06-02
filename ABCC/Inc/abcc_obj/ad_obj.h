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
** Application Data Object 0xFE (ADI) - Public interfaces
********************************************************************************
********************************************************************************
** Services :
** AD_GetMapSizeBits()          - Get size of all ADI's in a map.
** AD_GetNumAdisInMap()         - Get number of ADI's in a map.
** AD_Init()                    - Initiate AD object.
** AD_WriteBufferFromPdMap()    - Get ADI values from a map and write to a
**                                buffer.
** AD_WritePdMapFromBuffer()    - Get data from a buffer and write to ADI values
                                  according to map.
** AD_ProcObjectRequest()       - Object request handling.
** AD_RemapDone()               - Remap finished.
** AD_UpdatePdReadData()        - Update of read process data.
** AD_UpdatePdWriteData()       - Update of write process data.
** AD_GetAdiInstEntry()       - Get pointer to ADI entry
** AD_GetAdiValue()           - Get ADI data
********************************************************************************
********************************************************************************
*/
#ifndef AD_OBJ_H
#define AD_OBJ_H

#include "abcc_td.h"
#include "abp.h"
#include "abcc_ad_if.h"
#include "appl_abcc_handler.h"

/*******************************************************************************
** Typedefs
********************************************************************************
*/

/*******************************************************************************
** Public Services
********************************************************************************
*/
/*------------------------------------------------------------------------------
**  Initiates the AD object.
**------------------------------------------------------------------------------
** Arguments:
**    psAdiEntry   - Pointer to used ADI entry table.
**    iNumAdi      - Number of ADI:s in ADI entry table.
**    psDefaultMap - Pointer to default map. Set to NULL if no default map
**                   shall be used.
**
** Returns:
**    APPL_ErrCodeType
**------------------------------------------------------------------------------
*/
EXTFUNC APPL_ErrCodeType AD_Init( const AD_AdiEntryType* psAdiEntry,
                                  UINT16 iNumAdi,
                                  const AD_MapType* psDefaultMap );

/*------------------------------------------------------------------------------
**  Retrieve default mapping information from AD object. The AD objects also
**  prepares for network endian conversion if needed. The function may only be
**  called when requested by driver through the callback ABCC_CbfAdiMappingReq()
**------------------------------------------------------------------------------
** Arguments:
**     ppsAdiEntry    - Pointer to retrieve ADI entry table.
**                      NULL is returned if no default map is available.
**     ppsDefaultMap  - Pointer to retrieve default map information.
**                      NULL is returned if no default map is available.
**
** Returns:
**    Number of ADI:s in psAdiEntry table.
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 AD_AdiMappingReq( const AD_AdiEntryType** ppsAdiEntry,
                                 const AD_MapType** ppsDefaultMap );

/*------------------------------------------------------------------------------
** Indicate to AD object that the remap is finished
**------------------------------------------------------------------------------
** Arguments:
**    None
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
#if( ABCC_CFG_REMAP_SUPPORT_ENABLED )
EXTFUNC void AD_RemapDone( void );
#endif

/*------------------------------------------------------------------------------
** Process an object request against the Application Data Object.
**------------------------------------------------------------------------------
** Arguments:
**    psMsgBuffer      - Pointer to ABCC command message.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void AD_ProcObjectRequest( ABP_MsgType* psMsgBuffer );

/*------------------------------------------------------------------------------
** Update AD object with new read process data received from the ABCC.
**------------------------------------------------------------------------------
** Arguments:
**    pxPdDataBuf      - Pointer read process data buffer.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void AD_UpdatePdReadData( void* pxPdDataBuf );

/*------------------------------------------------------------------------------
** Fetch write process data from AD object.
**------------------------------------------------------------------------------
** Arguments:
**    pxPdDataBuf       - Pointer write process data buffer.
**
** Returns:
**    TRUE  -  Write process data buffer is updated.
**    FALSE -  No update was made.
**
**------------------------------------------------------------------------------
*/
EXTFUNC BOOL AD_UpdatePdWriteData( void* pxPdDataBuf );

/*------------------------------------------------------------------------------
** Get pointer to ADI entry table from ADI instance number
**------------------------------------------------------------------------------
** Arguments:
**    iInstance       - ADI instance.
**
** Returns:
**    Pointer to ADI entry table.
**------------------------------------------------------------------------------
*/
EXTFUNC const AD_AdiEntryType* AD_GetAdiInstEntry( UINT16 iInstance );

/*------------------------------------------------------------------------------
** Get the value of the specified ADI entry in network endian.
**------------------------------------------------------------------------------
** Arguments:
**    psAdiEntry        - Pointer to ADI entry.
**    pxDest            - Destination base pointer.
**    bNumElements      - Number of elements to read.
**    bStartIndex       - Index to first element to read.
**    piDestBitOffset   - Pointer to destination bit offset.
**                        This offset will be incremented according to the size
**                        of the read ADI. Can be set to an initial offset
**                        before calling the function.
**    fExplicit         - Indicates whether the get request originated from an
**                        explicit get request or a write process data request.
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void AD_GetAdiValue( const AD_AdiEntryType* psAdiEntry,
                             void* pxDest,
                             UINT8 bNumElements,
                             UINT8 bStartIndex,
                             UINT16* piDestBitOffset,
                             BOOL fExplicit );

/*------------------------------------------------------------------------------
** Get size of all ADI's in a map in octets.
**------------------------------------------------------------------------------
** Arguments:
**    pasMap - Pointer to array of ADI map elements. AD_MAP_END_ENTRY shall be
**             used to indicate end of array.
**
** Returns:
**    ADI map size in bits.
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 AD_GetMapSizeOctets( const AD_MapType* pasMap );

/*------------------------------------------------------------------------------
** Get number of ADI's in a map.
**------------------------------------------------------------------------------
** Arguments:
**    pasMap - Pointer to array of ADI map elements. AD_MAP_END_ENTRY shall be
**             used to indicate end of array.
**
** Returns:
**    Number of ADI's in the map.
**------------------------------------------------------------------------------
*/
EXTFUNC UINT16 AD_GetNumAdisInMap( const AD_MapType* pasMap );

/*------------------------------------------------------------------------------
** Write to a buffer using data from ADI's specified in an array of map
** elements.
**------------------------------------------------------------------------------
** Arguments:
**    pxDstPdDataBuf - Destination data buffer.
**    piOctetOffset  - Pointer to octet offset relative pxPdDataBuf.
**                     This offset will be incremented according to the size
**                     written to the buffer.
**    pasMap         - Pointer to array of ADI map elements.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void AD_WriteBufferFromPdMap( void* pxDstPdDataBuf,
                                      UINT16* piOctetOffset,
                                      const AD_MapType* pasMap );

/*------------------------------------------------------------------------------
** Write the ADI's specified in an array of map elements using data from
** the provided buffer.
**------------------------------------------------------------------------------
** Arguments:
**    pasMap         - Pointer to array of ADI map elements.
**    pxSrcPdDataBuf - Buffer containing the data that shall be used to write to
**                     the ADI's in the map.
**    piOctetOffset  - Pointer to octet offset relative pxPdDataBuf.
**                     This offset will be incremented according to the size
**                     read from the buffer.
**
** Returns:
**    None
**------------------------------------------------------------------------------
*/
EXTFUNC void AD_WritePdMapFromBuffer( const AD_MapType* pasMap,
                                      void* pxSrcPdDataBuf,
                                      UINT16* piOctetOffset );

#endif  /* inclusion lock */
