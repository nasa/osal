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
 * File: utlist.c
 *
 * Purpose: This file contains functions to implement a generic linked list data structure.
 */

/*
 * Includes
 */

#include "common_types.h"
#include "utlist.h"
#include <stdlib.h>
#include <string.h>

/*
 * Function Definitions
 */

UtListHead_t *UtList_Create(uint32 NumTags)
{
    struct ListAllocator
    {
        UtListHead_t Head;
        UtListNode_t Tags[];
    };
    struct ListAllocator *NewList;
    UtListNode_t *        TagHead;
    size_t                ActualSize;
    uint32                i;

    ActualSize = sizeof(struct ListAllocator) + (sizeof(UtListNode_t) * NumTags);
    NewList    = (struct ListAllocator *)malloc(ActualSize);

    memset(NewList, 0, ActualSize);

    NewList->Head.Tags         = NewList->Tags;
    NewList->Head.NumberOfTags = NumTags;

    for (i = 0; i < NumTags; ++i)
    {
        TagHead       = &NewList->Head.Tags[i];
        TagHead->Tag  = i;
        TagHead->Next = TagHead;
        TagHead->Prev = TagHead;
    }

    return (&NewList->Head);
}

void UtList_Destroy(UtListHead_t *ListHead)
{
    uint32 i;

    for (i = 0; i < ListHead->NumberOfTags; ++i)
    {
        UtList_Reset(&ListHead->Tags[i]);
    }
    free(ListHead);
}

void UtList_Reset(UtListNode_t *TagHead)
{
    while (!UtList_IsEmpty(TagHead))
    {
        UtList_DeleteNode(TagHead->Next);
    }
}

void UtList_Merge(UtListNode_t *TagHead1, UtListNode_t *TagHead2)
{
    UtListNode_t *Tail1 = TagHead1->Prev;
    UtListNode_t *Tail2 = TagHead2->Prev;

    Tail1->Next    = TagHead2;
    Tail2->Next    = TagHead1;
    TagHead1->Prev = Tail2;
    TagHead2->Prev = Tail1;
}

void UtList_Insert_After(UtListNode_t *ExistingNode, UtListNode_t *NewNode)
{
    NewNode->Next       = ExistingNode->Next;
    NewNode->Prev       = ExistingNode;
    NewNode->Prev->Next = NewNode;
    NewNode->Next->Prev = NewNode;
}

void UtList_Insert_Before(UtListNode_t *ExistingNode, UtListNode_t *NewNode)
{
    NewNode->Next       = ExistingNode;
    NewNode->Prev       = ExistingNode->Prev;
    NewNode->Prev->Next = NewNode;
    NewNode->Next->Prev = NewNode;
}

void UtList_Extract(UtListNode_t *ExistingNode)
{
    ExistingNode->Next->Prev = ExistingNode->Prev;
    ExistingNode->Prev->Next = ExistingNode->Next;
    ExistingNode->Next       = ExistingNode;
    ExistingNode->Prev       = ExistingNode;
}

UtListNode_t *UtList_NewNode(void *Data, uint32 DataSize)
{
    union NodeAllocator
    {
        UtListNode_t Node;
        double       AlignDbl;
        void *       AlignPtr;
        long         AlignLong;
    } * AllocNode;

    AllocNode = malloc(sizeof(union NodeAllocator) + DataSize);
    memset(AllocNode, 0, sizeof(union NodeAllocator));
    AllocNode->Node.Data     = &AllocNode[1];
    AllocNode->Node.DataSize = DataSize;
    memcpy(AllocNode->Node.Data, Data, DataSize);

    AllocNode->Node.Next = &AllocNode->Node;
    AllocNode->Node.Prev = &AllocNode->Node;

    return &AllocNode->Node;
}

void UtList_Add(UtListHead_t *ListHead, void *Data, uint32 DataSize, uint32 Tag)
{
    UtListNode_t *TagHead;
    UtListNode_t *NewNode;

    TagHead = UtList_GetHead(ListHead, Tag);
    if (TagHead != NULL)
    {
        NewNode      = UtList_NewNode(Data, DataSize);
        NewNode->Tag = Tag;
        UtList_Insert_Before(TagHead, NewNode);
    }
}

void UtList_DeleteNode(UtListNode_t *DeleteNode)
{
    UtList_Extract(DeleteNode);

    /* non-data/header nodes shouldn't be free()'ed */
    if (DeleteNode->Data != NULL)
    {
        free(DeleteNode);
    }
}

bool UtList_IsEmpty(UtListNode_t *TagHead)
{
    return (TagHead->Next == TagHead);
}

UtListNode_t *UtList_GetHead(UtListHead_t *ListHead, uint32 Tag)
{
    if (Tag >= ListHead->NumberOfTags)
    {
        return NULL;
    }
    return &ListHead->Tags[Tag];
}

UtListNode_t *UtList_GetNext(UtListNode_t *ListNode)
{
    return ListNode->Next;
}

void *UtList_GetObject(UtListNode_t *ListNode)
{
    return ListNode->Data;
}

bool UtList_IsEnd(UtListNode_t *TagHead, UtListNode_t *ListNode)
{
    return (TagHead == ListNode);
}
