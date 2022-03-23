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
