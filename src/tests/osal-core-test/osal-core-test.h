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

/**
 * \file
 *
 * Purpose:
 * This is a simple header file used to remove a lot of the task definitions
 * from the example1.c file so as to make that code easier to read
 *
 */

#ifndef OSAL_CORE_TEST_H
#define OSAL_CORE_TEST_H

/* Task 0 */
#define TASK_0_STACK_SIZE 1024
#define TASK_0_PRIORITY   230

/* Task 1 */
#define TASK_1_STACK_SIZE 1024
#define TASK_1_PRIORITY   231

/* Task 2 */
#define TASK_2_STACK_SIZE 1024
#define TASK_2_PRIORITY   232

/* Task 3 */
#define TASK_3_STACK_SIZE 1024
#define TASK_3_PRIORITY   233

/* Global Data */
#define MSGQ_DEPTH 50
#define MSGQ_SIZE  4

#endif /* OSAL_CORE_TEST_H */
