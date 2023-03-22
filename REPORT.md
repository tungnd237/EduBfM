# EduBfM Report

Name: Nguyen Duong Tung

Student id: 49004034

# Problem Analysis

The main objective of this project is to implement a part of the ODYSSEUS/COSMOS storage system, specifically the coarse-granule locking version. The focus of this project is on the buffer manager, which is responsible for maintaining the in-memory copy of disk pages or trains. The buffer manager is an essential component of a DBMS, as it is responsible for efficiently managing the data flow between the main memory and disk storage. The implementation of this project will provide an understanding of the different modules in a DBMS and their interactions. The project will involve implementing the data structures and operations of the buffer manager, such as EduBfM_GetTrain(), EduBfM_FreeTrain(), EduBfM_SetDirty(), EduBfM_FlushAll(), and EduBfM_DiscardAll(). In addition, some smaller functions like edubfm_ReadTrain(), edubfm_AllocTrain(), edubfm_Insert(), edubfm_Delete(), edubfm_Deleteall(), edubfm_LookUp(), and edubfm_FlushTrain() will also be implemented.

# Design For Problem Solving

## High Level

Understand the data structures required for the buffer manager, including buffer pool, buffer table, and hash table.

Firstly, implement the supporting functions for buffer manager operations: ReadTrain, AllocTrain, Insert, Delete, DeleteAll, LookUp, and FlushTrain.

After that, implement the primary buffer manager operations: GetTrain, FreeTrain, SetDirty, FlushAll, and DiscardAll.

### Read Train

Store the page/train in a buffer element reading it from the disk, and return the pointer to the corresponding buffer element.

### Allocate Train

Allocate a buffer element in bufferPool to store a page/train,
and return the array index of the buffer element.

### Insert 

Insert the array index of the buffer element into hashTable.

### Delete

Delete the array index of the buffer element from hashTable.

### Delete All

Delete every entry (array index of the buffer element) from
each hashTable.

### Look up

Search for the array index of the buffer element having the
page/train whose hash key (BfMHashKey) is the same with
that given as a parameter from hashTable, and return it.

### Flush Train

Write out a modified page/train into the disk.

### GetTrain

Fix the page/train in bufferPool, and return the pointer to
the buffer element containing the page/train.

### FreeTrain

Unfix the page/train from bufferPool.

### SetDirty

Set the DIRTY bit to 1 to indicate that the page/train residing
in the buffer element has been modified.

### FlushAll

Write out the modified pages/trains residing in each
bufferPool to the disk.

### DiscardAll.

Delete pages/trains residing in each bufferPool without
writing them out to disk

## Low Level

### Read Train

Load the specific page or train from the disk into a designated buffer element, and then provide the pointer that directs to the associated buffer element.

### Allocate Train

Define a function edubfm_AllocTrain that accepts the type of buffer (PAGE or TRAIN) as input.
Check if unsupported functionality is being used. If so, return an error for not supported.
Initialize variables: secondChanceCount, victim.
Begin a loop for the second chance buffer replacement algorithm.
Check if the current buffer element is unfixed.
If the buffer element is unfixed, verify if the REFER bit is set.
If the REFER bit is set, reset it. Otherwise, break the loop as the buffer element can be allocated.
Increment secondChanceCount and update the victim index.
If no unfixed buffer element is found, return an error.
If the victim's pageNo is NIL, reset all bits, update the next victim index, and return the victim.
If the buffer element is dirty, call the function to flush the train to disk. If there's an error, return it.
Reset all bits in the buffer element.
Update the next victim index.
Delete the array index of the buffer element from the hashTable. If there's an error, return it.
Finally, return the victim.

### Insert 

Define a function edubfm_Insert that accepts a hash key, an index used in the buffer pool, and the buffer type as inputs.
Check the validity of the key.
If the index is less than 0 or greater than the number of buffers for the given type, return an error for bad buffer index.
Calculate the hash value using the hash function.
If there is a collision in the hash table, handle it using the chaining method. Set the next hash entry of the current index to the existing hash table entry.
If there is no collision, set the next hash entry of the current index to NIL.
Insert the buffer element index into the hash table at the calculated hash value.
Finally, return success (eNOERROR).

### Delete

Define a function edubfm_Delete that accepts a hash key and buffer type as inputs.
Check the validity of the key.
Calculate the hash value using the hash function.
Initialize the variables i and prev with the hash table entry at the calculated hash value and NIL, respectively.
Start a loop to search for the entry in the hash table.
If the entry is found, check if it is the first element in the list (prev is NIL).
If the entry is the first one in the list, update the hash table entry at the hash value to the next hash entry of the current index i.
If the entry is in the middle or at the end of the list, set the next hash entry of the previous index to the next hash entry of the current index i.
Return success (eNOERROR).
If the entry is not found after the loop, return an error for entry not found (eNOTFOUND_BFM).

### Delete All

Define a function edubfm_DeleteAll that takes no input parameters.
Start a loop to iterate over all buffer types.
For each buffer type, get the size of the hash table.
Initialize another loop to iterate over each hash table entry for the current buffer type.
Set each hash table entry to NIL (-1).
After iterating through all buffer types and their respective hash table entries, return success (eNOERROR).

### Look up

Define a function edubfm_LookUp that accepts a hash key and buffer type as inputs.
Check the validity of the key.
Calculate the hash value using the hash function.
Initialize the variable i with the hash table entry at the calculated hash value.
Start a loop to search for the entry in the hash table.
If the entry is found, return the index i.
If the entry is not found after the loop, return the constant for not found in the hash table (NOTFOUND_IN_HTABLE).

### Flush Train

Define a function EduBfM_FlushAll that takes no input parameters.
Initialize a loop to iterate over all buffer types.
For each buffer type, start another loop to iterate over all buffer elements.
Check if the current buffer element has the dirty bit set.
If the dirty bit is set, write out the page or train to the disk by calling the edubfm_FlushTrain function with the current key and buffer type as inputs.
If there's an error during the flush operation, return it.
After iterating through all buffer types and their respective buffer elements, return success (eNOERROR).

### GetTrain

Define a function EduBfM_GetTrain that takes a train ID, a pointer to the returned buffer, and the buffer type as inputs.
Check the validity of the input parameters, including the buffer pointer and buffer type.
Search for the buffer element containing the train using the edubfm_LookUp function.
If the train is not found in the buffer pool, allocate a buffer element using the edubfm_AllocTrain function.
If the allocation is successful, read the train from the disk using the edubfm_ReadTrain function.
Update the bufTable with the train ID and buffer index.
Set the buffer element as fixed, and set the REFER bit.
Insert the allocated buffer element into the hashTable using the edubfm_Insert function.
If the train is found in the buffer pool, update the bufTable by incrementing the fixed count and setting the REFER bit.
Return the pointer to the buffer element.

### FreeTrain

Define a function EduBfM_FreeTrain that takes a train ID and the buffer type as inputs.
Check the validity of the input buffer type.
Find the index of the buffer element containing the train using the edubfm_LookUp function.
If the train is not found in the buffer pool, return an error indicating that the train is not found.
If the train is found, decrease the fixed count of the buffer element by 1.
If the fixed count becomes less than 0, print a warning message and set the fixed count to 0.
Return a success status, indicating no errors.

### SetDirty

Define a function EduBfM_SetDirty that takes a train ID and the buffer type as inputs.
Check the validity of the input buffer type.
Find the index of the buffer element containing the train using the edubfm_LookUp function.
If the train is not found in the buffer pool, return an error indicating that the train is not found.
If the train is found, set the DIRTY bit of the buffer element to 1.
Return a success status, indicating no errors.

### FlushAll

Define a function EduBfM_FlushAll that takes no inputs.
Iterate over all buffer types (type 0 and type 1).
For each buffer type, iterate over all buffer elements.
Check if the dirty bit is set for the current buffer element.
If the dirty bit is set, call the edubfm_FlushTrain function to write out the page/train to the disk using the buffer element's key and buffer type.
If an error occurs during the flushing process, return the error code.
After iterating through all buffer elements and flushing the dirty ones, return a success status, indicating no errors.

### DiscardAll.

Define a function EduBfM_DiscardAll that takes no inputs.
Iterate over all buffer types (type 0 and type 1).
For each buffer type, iterate over all buffer elements.
Initialize the key of the current buffer element by setting its page number to NIL.
Reset all bits of the current buffer element to 0.
After iterating through all buffer elements, delete all entries in the hash table using the edubfm_DeleteAll function.
If an error occurs during the deletion process, return the error code.
Otherwise, return a success status, indicating no errors.

# Mapping Between Implementation And the Design

### Read Train

Four edubfm_ReadTrain(
    TrainID *trainId,		/* IN which train? */
    char    *aTrain,		/* OUT a pointer to buffer */
    Four    type )		/* IN buffer type */
{
    Four e;			/* for error */


	/* Error check whether using not supported functionality by EduBfM */
	if (RM_IS_ROLLBACK_REQUIRED()) ERR(eNOTSUPPORTED_EDUBFM);
    
    /* read the train from disk into the buffer */
    e = RDsM_ReadTrain(trainId, aTrain, BI_BUFSIZE(type));
    if (e < eNOERROR) return e;

    return( eNOERROR );

}  /* edubfm_ReadTrain */

### Allocate Train

Four edubfm_AllocTrain(
    Four 	type)			/* IN type of buffer (PAGE or TRAIN) */
{
    Four 	e;			/* for error */
    Four 	victim;			/* return value */
    Four 	i;
    Four 	secondChanceCount;	/* to count second chances given */

    /* Error check whether using not supported functionality by EduBfM */
    if(sm_cfgParams.useBulkFlush) ERR(eNOTSUPPORTED_EDUBFM);

    secondChanceCount = 0;
    victim = BI_NEXTVICTIM(type);

    /* Second chance buffer replacement algorithm */
    while (secondChanceCount < 2 * BI_NBUFS(type)) {
        if (BI_FIXED(type, victim) == 0) { // Unfixed buffer element found
            if (BI_BITS(type, victim) & REFER) { // REFER bit is set
                BI_BITS(type, victim) &= ~REFER; // Reset REFER bit to 0
            } else { // REFER bit is not set, buffer element can be allocated
                break;
            }
        }
        secondChanceCount++;
        victim = (victim +1) % BI_NBUFS(type);
    }

    if (secondChanceCount >= 2 * BI_NBUFS(type) ) {
        ERR(eNOUNFIXEDBUF_BFM); // No unfixed buffer element found
    }

    if (BI_KEY(type, victim).pageNo == NIL) {
        BI_BITS(type, victim) = 0;
        BI_NEXTVICTIM(type) = (victim + 1) % BI_NBUFS(type);
        return victim;
    }

    // If the buffer element is dirty, flush its contents to disk
    if (BI_BITS(type, victim) & DIRTY) {
        e = edubfm_FlushTrain(&BI_KEY(type, victim), type);
        if (e < eNOERROR) ERR(e);
    }

    // Reset all bits in the buffer element
    BI_BITS(type, victim) = 0;

    // Update the next victim index
    BI_NEXTVICTIM(type) = (victim + 1) % BI_NBUFS(type) ;

    // Delete the array index of the buffer element from the hashTable
    e = edubfm_Delete(&BI_KEY(type, victim), type);
    if (e < eNOERROR) ERR(e);

    return(victim);

}  /* edubfm_AllocTrain */

### Insert 

Four edubfm_Insert(
    BfMHashKey       *key,           /* IN a hash key in Buffer Manager */
    Two              index,          /* IN an index used in the buffer pool */
    Four             type)           /* IN buffer type */
{
    Four            i;            
    Two             hashValue;

    CHECKKEY(key);    /*@ check validity of key */

    if( (index < 0) || (index > BI_NBUFS(type)) )
        ERR( eBADBUFINDEX_BFM );

    // Calculate the hash value using the hash function
    hashValue = BFM_HASH(key, type);

    // If there is a collision, handle it using the chaining method
    if (BI_HASHTABLEENTRY(type,hashValue) != NIL) {
        BI_NEXTHASHENTRY(type, index) = BI_HASHTABLEENTRY(type,hashValue);
    } else {
        BI_NEXTHASHENTRY(type, index) = NIL;
    }

    // Insert the buffer element index into the hashTable
    BI_HASHTABLEENTRY(type,hashValue) = index;

    return( eNOERROR );

}  /* edubfm_Insert */

### Delete

Four edubfm_Delete(
    BfMHashKey          *key,                   /* IN a hash key in buffer manager */
    Four                type )                  /* IN buffer type */
{
    Two                 i, prev;                
    Two                 hashValue;

    CHECKKEY(key);    /*@ check validity of key */

    // Calculate the hash value
    hashValue = BFM_HASH(key,type);

    // Initialize i and prev
    i = BI_HASHTABLEENTRY(type, hashValue);
    prev = NIL;

    // Search for the entry in the hashTable
    while (i != NIL) {
        if (EQUALKEY(&BI_KEY(type, i), key)) {
            // Found the entry to be deleted
            if (prev == NIL) {
                // The entry is the first one in the list
                BI_HASHTABLEENTRY(type, hashValue) = BI_NEXTHASHENTRY(type, i);
            } else {
                // The entry is in the middle or at the end of the list
                BI_NEXTHASHENTRY(type, prev) = BI_NEXTHASHENTRY(type, i);
            }
            return eNOERROR;
        }
        prev = i;
        i = BI_NEXTHASHENTRY(type, i);
    }

    // Entry not found
    ERR( eNOTFOUND_BFM );

}  /* edubfm_Delete */

### Delete All

Four edubfm_DeleteAll(void)
{
    Two 	i;
    Four        tableSize;

    /* Iterate over all buffer types */
    for (Four type = 0; type < NUM_BUF_TYPES; type++) {
        /* Get the size of the hashTable for the current buffer type */
        tableSize = HASHTABLESIZE(type);

        /* Set each hashTable entry to NIL(-1) */
        for (i = 0; i < tableSize; i++) {
            BI_HASHTABLEENTRY(type,i) = NIL;
        }
    }

    return(eNOERROR);

} /* edubfm_DeleteAll() */

### Look up

Four edubfm_LookUp(
    BfMHashKey          *key,                   /* IN a hash key in Buffer Manager */
    Four                type)                   /* IN buffer type */
{
    Two                 i, j;                   /* indices */
    Two                 hashValue;

    CHECKKEY(key);    /*@ check validity of key */

    hashValue = BFM_HASH(key,type);

    i = BI_HASHTABLEENTRY(type,hashValue);

    while (i != NIL) {
        if (EQUALKEY(&BI_KEY(type, i), key)) {
        return i;
        }
        i = BI_NEXTHASHENTRY(type, i);
    }

    return(NOTFOUND_IN_HTABLE);
    
}  /* edubfm_LookUp */

### Flush Train

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
        e = RDsM_WriteTrain(BI_BUFFER(type, index), trainId, BI_BUFSIZE(type));
        if (e < 0) {
            return e;
        }
        // Reset the DIRTY bit
        BI_BITS(type, index) &= ~DIRTY;
    }

    return eNOERROR;
} /* edubfm_FlushTrain */


### GetTrain

Four EduBfM_GetTrain(
    TrainID             *trainId,               /* IN train to be used */
    char                **retBuf,               /* OUT pointer to the returned buffer */
    Four                type )                  /* IN buffer type */
{
    Four                e;                      /* for error */
    Four                index;                  /* index of the buffer pool */
    BfMHashKey key;
    /* Check the validity of given parameters */
    if(retBuf == NULL) ERR(eBADBUFFER_BFM);

    /* Is the buffer type valid? */
    if(IS_BAD_BUFFERTYPE(type)) ERR(eBADBUFFERTYPE_BFM);

    if (trainId == NULL) ERR(eBADBUFFER_BFM);

    /* Search for the buffer element containing the train */
    index = edubfm_LookUp(trainId, type);

    if (index == NOTFOUND_IN_HTABLE) {
        /* Train not found in bufferPool, allocate a buffer element */
        index = edubfm_AllocTrain(type);
        if (index < eNOERROR) ERR(index);

        /* Read the train from disk */
        e = edubfm_ReadTrain(trainId, BI_BUFFER(type, index), type);
        if (e < eNOERROR) ERR(e);

        /* Update the bufTable */
        key.volNo = trainId->volNo;
        key.pageNo = trainId->pageNo;
        BI_KEY(type, index) = key;
        BI_FIXED(type, index) = 1;
        BI_BITS(type, index) |= REFER;

        /* Insert the allocated buffer element into the hashTable */
        e = edubfm_Insert(trainId, index, type);
        if (e < eNOERROR) ERR(e);
    }
    else {
        /* Train found in bufferPool, update the bufTable */
        BI_FIXED(type, index)++;
        BI_BITS(type, index) |= REFER;
    }

    /* Return the pointer to the buffer element */
    *retBuf = BI_BUFFER(type, index);

    return(eNOERROR);   /* No error */

}  /* EduBfM_GetTrain() */

### FreeTrain

Four EduBfM_FreeTrain( 
    TrainID             *trainId,       /* IN train to be freed */
    Four                type)           /* IN buffer type */
{
    Four                index;          /* index on buffer holding the train */
    Four 		e;		/* error code */

    /*@ check if the parameter is valid. */
    if (IS_BAD_BUFFERTYPE(type)) ERR(eBADBUFFERTYPE_BFM);

    /* Find the index of the buffer element containing the train using its hash value */
    e = edubfm_LookUp(trainId, type);
    if (e < eNOERROR) ERR(e);

    if (e == NOTFOUND_IN_HTABLE) {
        return eNOTFOUND_BFM;
    }
    index = e;

    /* Decrease the fixed variable of the buffer element by 1 */
    BI_FIXED(type, index) -= 1;

    /* If the value of fixed becomes less than 0 */
    if (BI_FIXED(type, index) < 0) {
        printf("Warning: Fixed counter is less than 0!!!\n");
        BI_FIXED(type, index) = 0;
    }

    return( eNOERROR );
    
} /* EduBfM_FreeTrain() */

### SetDirty

Four EduBfM_SetDirty(
    TrainID             *trainId,               /* IN which train has been modified in the buffer?  */
    Four                type )                  /* IN buffer type */
{
    Four                index;                  /* an index of the buffer table & pool */
    Four                e;                      /* error code */

    /*@ Is the parameter valid? */
    if (IS_BAD_BUFFERTYPE(type)) ERR(eBADBUFFERTYPE_BFM);

    /* Find the index of the buffer element containing the train using its hash value */
    e = edubfm_LookUp(trainId, type);
    if (e < eNOERROR) ERR(e);

    if (e == NOTFOUND_IN_HTABLE) {
        return eNOTFOUND_BFM;
    }

    index = e;

    /* Set the DIRTY bit of the buffer element to 1 */
    BI_BITS(type, index) |= DIRTY;

    return( eNOERROR );

}  /* EduBfM_SetDirty */

### FlushAll

Four EduBfM_FlushAll(void)
{
    Four        e;                      /* error */
    Two         i;                      /* index */
    Four        type;                   /* buffer type */

    /* Iterate over all buffer types */
    for (type = 0; type < 2; type++) {
        /* Iterate over all buffer elements */
        for (i = 0; i < BI_NBUFS(type); i++) {
            /* Check if the buffer element has the dirty bit set */
            if (BI_BITS(type, i) & DIRTY) {
                /* Write out the page/train to the disk */
                e = edubfm_FlushTrain(&BI_KEY(type, i), type);
                if (e < eNOERROR) ERR(e);
            }
        }
    }

    return( eNOERROR );
    
}  /* EduBfM_FlushAll() */

### DiscardAll.

Four EduBfM_DiscardAll(void)
{
    Four 	e;			/* error */
    Two 	i;			/* index */
    Four 	type;			/* buffer type */

    /* Iterate over all buffer types */
    for (type = 0; type < 2; type++) {
        /* Iterate over all buffer elements */
        for (i = 0; i < BI_NBUFS(type); i++) {
            /* Initialize the key */
            BI_KEY(type, i).pageNo = NIL;

            /* Reset all bits */
            BI_BITS(type, i) = 0;
        }
    }
    /* Delete all entries in the hashTable */
    e = edubfm_DeleteAll();
    if (e < eNOERROR) ERR(e);

    return(eNOERROR);

}  /* EduBfM_DiscardAll() */