/*
 *  NASA Docket No. GSC-18,370-1, and identified as "Operating System Abstraction Layer"
 *
 *  Copyright (c) 2019 United States Government as represented by
 *  the Administrator of the National Aeronautics and Space Administration.
 *  All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

/*
 * File: utlist.h
 *
 * Purpose: This file contains functions to implement a generic linked list data structure.
 */

#ifndef _utlist_
#define _utlist_

/*
 * Includes
 */

#include "common_types.h"

/*
 * Macro Definitions
 */

/* Macros to redefine list functions to look like stack and queue functions */
#define UtStack_Push    UtList_Add
#define UtStack_Pop     UtList_RemoveLast
#define UtStack_IsEmpty UtList_IsEmpty
#define UtStack_Depth   UtList_Depth

#define UtQueue_Add     UtList_Add
#define UtQueue_Get     UtList_RemoveFirst
#define UtQueue_Look    UtList_First
#define UtQueue_Delete  UtList_DeleteFirst
#define UtQueue_IsEmpty UtList_IsEmpty
#define UtQueue_Depth   UtList_Depth

/*
 * Type Definitions
 */

typedef struct UtListNodeTag
{
    struct UtListNodeTag *Next;
    struct UtListNodeTag *Prev;
    void *                Data;
    uint32                DataSize;
    uint32                Tag;
} UtListNode_t;

typedef struct
{
    UtListNode_t *Tags;
    uint32        NumberOfTags;
} UtListHead_t;

/*
 * Exported Functions
 */

/* Dynamically allocates a new list head.  A list head could also just be declared, this function is useful
 * if you need to dynamically allocate memory for a new list head.  Note always free list heads allocated by
 * this function by calling UtList_Destroy. */
UtListHead_t *UtList_Create(uint32 NumTags);

/* Frees a list head created by UtList_Create. */
void UtList_Destroy(UtListHead_t *ListHead);

/* Deletes all nodes on the list. */
void UtList_Reset(UtListNode_t *TagHead);

/* Merge two lists heads together */
void UtList_Merge(UtListNode_t *TagHead1, UtListNode_t *TagHead2);

/* Dynamically adds a new node to the list.  Nodes are always added to the end of the list.  Memory is dynamically
 * allocated for the new node and to hold the data pointed to by Data.  A Tag field is also provided to be used to
 * store user defined information with the node. */
void UtList_Add(UtListHead_t *ListHead, void *Data, uint32 DataSize, uint32 Tag);

/* Deletes the specified node from the list, this will screw up if you do not pass in a valid DeleteNode.  I do not
 * verify that DeleteNode is a member of the list. */
void UtList_DeleteNode(UtListNode_t *DeleteNode);

/* Returns true if the list is empty.  This is the same as (UtListHead->NumberOfEntries == 0). */
bool UtList_IsEmpty(UtListNode_t *TagHead);

/* Returns the head node of a list for the given tag */
UtListNode_t *UtList_GetHead(UtListHead_t *ListHead, uint32 Tag);

/* Returns the next node in the list, given the current node */
UtListNode_t *UtList_GetNext(UtListNode_t *ListNode);

/* Returns the data object associated with the current node */
void *UtList_GetObject(UtListNode_t *ListNode);

/* Check if the current node marks the end of the list */
bool UtList_IsEnd(UtListNode_t *TagHead, UtListNode_t *ListNode);

#endif
