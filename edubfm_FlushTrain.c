/******************************************************************************/
/*                                                                            */
/*    ODYSSEUS/EduCOSMOS Educational-Purpose Object Storage System            */
/*                                                                            */
/*    Developed by Professor Kyu-Young Whang et al.                           */
/*                                                                            */
/*    Database and Multimedia Laboratory                                      */
/*                                                                            */
/*    Computer Science Department and                                         */
/*    Advanced Information Technology Research Center (AITrc)                 */
/*    Korea Advanced Institute of Science and Technology (KAIST)              */
/*                                                                            */
/*    e-mail: kywhang@cs.kaist.ac.kr                                          */
/*    phone: +82-42-350-7722                                                  */
/*    fax: +82-42-350-8380                                                    */
/*                                                                            */
/*    Copyright (c) 1995-2013 by Kyu-Young Whang                              */
/*                                                                            */
/*    All rights reserved. No part of this software may be reproduced,        */
/*    stored in a retrieval system, or transmitted, in any form or by any     */
/*    means, electronic, mechanical, photocopying, recording, or otherwise,   */
/*    without prior written permission of the copyright owner.                */
/*                                                                            */
/******************************************************************************/
/*
 * Module: edubfm_FlushTrain.c
 *
 * Description : 
 *  Write a train specified by 'trainId' into the disk.
 *
 * Exports:
 *  Four edubfm_FlushTrain(TrainID *, Four)
 */


#include "EduBfM_common.h"
#include "RDsM.h"
#include "RM.h"
#include "EduBfM_Internal.h"



/*@================================
 * edubfm_FlushTrain()
 *================================*/
/*
 * Function: Four edubfm_FlushTrain(TrainID*, Four)
 *
 * Description : 
 * (Following description is for original ODYSSEUS/COSMOS BfM.
 *  For ODYSSEUS/EduCOSMOS EduBfM, refer to the EduBfM project manual.)
 *
 *  Write a train specified by 'trainId' into the disk.
 *  Construct a hash key using the TrainID 'trainId'(actually same)
 *  in order to look up the buffer in the buffer pool. If it is successfully
 *  found, then force it out to the disk using RDsM, especially
 *  RDsM_WriteTrain().
 *
 * Returns:
 *  error code
 *    some errors caused by function calls
 */
Four edubfm_FlushTrain(TrainID *trainId, Four type) {
    Four e;      // for errors
    Four index;  // for an index

    /* Error check whether using not supported functionality by EduBfM */
    if (RM_IS_ROLLBACK_REQUIRED()) {
        return eNOTSUPPORTED_EDUBFM;
    }
    
    BfMHashKey hashKey;
    hashKey.volNo = trainId->volNo;
    hashKey.pageNo = trainId->pageNo;
    // Construct a hash key using the TrainID 'trainId' to look up the buffer in the buffer pool
    index = edubfm_LookUp(&hashKey, type);
    if (index < 0) {
        // The train does not exist in bufferPool
        return eNOTFOUND_BFM;
    }

    // Write out the page/train into the disk if DIRTY bit is set to 1
    if (BI_BITS(type, index) & DIRTY) {
        e = RDsM_WriteTrain(BI_BUFFER(type, index), trainId, 1);
        if (e < 0) {
            return e;
        }
        // Reset the DIRTY bit
        BI_BITS(type, index) &= ~DIRTY;
    }

    return eNOERROR;
} /* edubfm_FlushTrain */
