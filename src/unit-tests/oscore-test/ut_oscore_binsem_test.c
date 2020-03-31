/*================================================================================*
** File:  ut_oscore_binsem_test.c
** Owner: Alan Cudmore
** Date:  April 2013
**================================================================================*/

/*--------------------------------------------------------------------------------*
** Includes
**--------------------------------------------------------------------------------*/

#include "ut_oscore_binsem_test.h"

/*--------------------------------------------------------------------------------*
** Macros
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Data types
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** External global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Global variables
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function prototypes
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Local function definitions
**--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------*
** Syntax: OS_BinSemCreate
** Purpose: Creates a binary semaphore
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_TAKEN if the name passed in has already been used
**          OS_ERR_NO_FREE_IDS if there are no more free binary semaphore ids
**          OS_SEM_FAILURE if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_bin_sem_create_test()
{
    int i;
    char  sem_name[UT_OS_NAME_BUFF_SIZE];
    char  long_sem_name[UT_OS_NAME_BUFF_SIZE];
    uint32  sem_ids[OS_MAX_BIN_SEMAPHORES+1];

    /*-----------------------------------------------------*/
    if (!UT_OS_IMPL(OS_BinSemCreate(&sem_ids[0], "Good", 1, 0))) return;
    OS_BinSemDelete(sem_ids[0]);

    /*-----------------------------------------------------*/
    UT_OS_RETVAL(OS_BinSemCreate(NULL, "BinSem1", 1, 0), OS_INVALID_POINTER, "#1 Null-pointer-arg-1");

    /*-----------------------------------------------------*/
    UT_OS_RETVAL(OS_BinSemCreate(&sem_ids[0], NULL, 1, 0), OS_INVALID_POINTER, "#2 Null-pointer-arg-2");

    /*-----------------------------------------------------*/
    memset(long_sem_name, 'X', sizeof(long_sem_name));
    long_sem_name[sizeof(long_sem_name)-1] = '\0';
    UT_OS_RETVAL(OS_BinSemCreate(&sem_ids[0], long_sem_name, 1, 0), OS_ERR_NAME_TOO_LONG, "#3 Name-too-long");

    /*-----------------------------------------------------*/
    /* Setup */
    for ( i = 0; i< OS_MAX_BIN_SEMAPHORES; i++ )
    {
       memset(sem_name, '\0', sizeof(sem_name));
       UT_os_sprintf(sem_name, "BINSEM%d", i);

       if(!UT_OS_CHECK(OS_BinSemCreate(&sem_ids[i], sem_name, 1, 0), "BinSemCreate"))
       {
          break;
       }
    }

    if ( i == OS_MAX_BIN_SEMAPHORES )
    {
       UT_OS_RETVAL(OS_BinSemCreate(&sem_ids[OS_MAX_BIN_SEMAPHORES], "OneTooMany", 1, 0), OS_ERR_NO_FREE_IDS, "#4 No-free-IDs");
    }

    /* Reset test environment */
    OS_DeleteAllObjects();

    /*-----------------------------------------------------*/
    /* Setup */
    if(UT_OS_CHECK(OS_BinSemCreate(&sem_ids[0], "DUPLICATE", 1, 0), "BinSemCreate"))
    {
       UT_OS_RETVAL(OS_BinSemCreate(&sem_ids[0], "DUPLICATE", 1, 0), OS_ERR_NAME_TAKEN, "#5 Duplicate-name");

       /* Reset test environment */
       UT_OS_CHECK(OS_BinSemDelete(sem_ids[0]), "BinSemDelete");
    }

    /*-----------------------------------------------------*/
    UT_OS_TEST_RESULT( "#6 OS-call-failure", UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    UT_OS_SUCCESS(OS_BinSemCreate(&sem_ids[0], "Good", 1, 0), "#7 Nominal");

    /* Reset test environment */
    UT_OS_CHECK(OS_BinSemDelete(sem_ids[0]), "BinSemDelete");
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_BinSemDelete
** Purpose: Deletes a binary semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in does not exist
**          OS_SEM_FAILURE if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_bin_sem_delete_test()
{
    int32 res = 0;
    const char* testDesc;
    uint32  bin_sem_id;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_BinSemDelete(0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_bin_sem_delete_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_BinSemDelete(99999);
    if ( res == OS_ERR_INVALID_ID )
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_BinSemCreate(&bin_sem_id, "DeleteTest", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Bin Sem Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
       res = OS_BinSemDelete(bin_sem_id);
       if ( res == OS_SUCCESS )
          UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
       else
          UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);
    }

UT_os_bin_sem_delete_test_exit_tag:
    return;
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_BinSemFlush
** Purpose: Releases all the tasks waiting on the given semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore id
**          OS_SEM_FAILURE if the semaphore was not previously init'd or is not in the
**                         array of semaphores defined by the system
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_bin_sem_flush_test()
{
    int32 res = 0;
    const char* testDesc;
    uint32  bin_sem_id;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_BinSemFlush(0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_bin_sem_flush_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_BinSemFlush(99999);
    if ( res == OS_ERR_INVALID_ID )
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_BinSemCreate(&bin_sem_id, "FlushTest", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Bin Sem Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
       res = OS_BinSemFlush(bin_sem_id);
       if ( res == OS_SUCCESS )
          UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
       else
          UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

       res = OS_BinSemDelete(bin_sem_id);
    }

UT_os_bin_sem_flush_test_exit_tag:
    return;
    
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_BinSemGive
** Purpose: Gives back a binary semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore id
**          OS_SEM_FAILURE if the semaphore was not previously init'd or is not in the
**                         array of semaphores defined by the system
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_bin_sem_give_test()
{
    int32 res = 0;
    const char* testDesc;
    uint32  bin_sem_id;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_BinSemGive(0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_bin_sem_give_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_BinSemGive(99999);
    if ( res == OS_ERR_INVALID_ID )
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_BinSemCreate(&bin_sem_id, "GiveTest", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Bin Sem Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_BinSemGive(bin_sem_id);
        if ( res == OS_SUCCESS )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_BinSemDelete(bin_sem_id);
    }

UT_os_bin_sem_give_test_exit_tag:
    return;

}

/*--------------------------------------------------------------------------------*
** Syntax: OS_BinSemTake
** Purpose: Reserves a binary semaphore
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore id
**          OS_SEM_FAILURE if the semaphore was not previously init'd or is not in the
**                         array of semaphores defined by the system
**          OS_SEM_FAILURE if the OS call failed
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_bin_sem_take_test()
{
    int32 res = 0;
    const char* testDesc;
    uint32  bin_sem_id;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_BinSemTake(0);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_bin_sem_take_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_BinSemTake(99999);
    if ( res == OS_ERR_INVALID_ID )
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_BinSemCreate(&bin_sem_id, "TakeTest", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Bin Sem Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_BinSemTake(bin_sem_id);
        if ( res == OS_SUCCESS )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_BinSemDelete(bin_sem_id);
    }

UT_os_bin_sem_take_test_exit_tag:
    return;
    
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_BinSemTimedWait
** Purpose: Reserves a binary semaphore with a timeout
** Parameters: To-be-filled-in
** Returns: OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore id
**          OS_SEM_FAILURE if the OS call failed
**          OS_SEM_TIMEOUT if the semaphore was not relinquished in time
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_bin_sem_timed_wait_test()
{
    int32 res = 0;
    const char* testDesc;
    uint32  bin_sem_id;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_BinSemTimedWait(0,1000);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_bin_sem_timed_wait_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_BinSemTimedWait(99999, 1000);
    if ( res == OS_ERR_INVALID_ID )
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 OS-call-failure";

    UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_INFO);

    /*-----------------------------------------------------*/
    testDesc = "#3 Sem-take-timed-out";

    /* Setup */
    res = OS_BinSemCreate(&bin_sem_id, "TimedWait", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Sem-take-timed-out - Bin Sem Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
       res = OS_BinSemTake(bin_sem_id);
       if ( res != OS_SUCCESS )
       {
           testDesc = "#3 Sem-take-timed-out - Bin Sem Take failed";
           UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
       }
       else
       {
          res = OS_BinSemTimedWait(bin_sem_id,1000);
          if ( res == OS_SEM_TIMEOUT )
              UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
          else
              UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

       }
       res = OS_BinSemDelete(bin_sem_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#4 Nominal";

    /* Setup */
    res = OS_BinSemCreate(&bin_sem_id, "TimedWait", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#4 Nominal - Bin Sem Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_BinSemTimedWait(bin_sem_id,1000);
        if ( res == OS_SUCCESS )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_BinSemDelete(bin_sem_id);
    }

UT_os_bin_sem_timed_wait_test_exit_tag:
    return;
    
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_BinSemGetIdByName
** Purpose: Returns the id of a given binary semaphore name
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if any of the pointers passed in is null
**          OS_ERR_NAME_TOO_LONG if the name passed in is too long
**          OS_ERR_NAME_NOT_FOUND if the name passed in was not found in the binary
**                                semaphore table
**          OS_SUCCESS if succeeded
**--------------------------------------------------------------------------------*/
void UT_os_bin_sem_get_id_by_name_test()
{
    int32 res = 0;
    const char* testDesc;
    uint32  bin_sem_id;
    char long_sem_name[UT_OS_NAME_BUFF_SIZE];

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_BinSemGetIdByName(0, "InvalidName");
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_bin_sem_get_id_by_name_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-pointer-arg-1";

    res = OS_BinSemGetIdByName(NULL, "InvalidName");
    if ( res == OS_INVALID_POINTER )
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

     /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg-2";

    res = OS_BinSemGetIdByName(&bin_sem_id, NULL);
    if ( res == OS_INVALID_POINTER )
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#3 Name-too-long";

    memset(long_sem_name, 'Y', sizeof(long_sem_name));
    long_sem_name[sizeof(long_sem_name)-1] = '\0';
    res = OS_BinSemGetIdByName(&bin_sem_id, long_sem_name);
    if ( res == OS_ERR_NAME_TOO_LONG )
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#4 Name-not-found";

    res = OS_BinSemGetIdByName(&bin_sem_id, "NameNotFound");
    if ( res == OS_ERR_NAME_NOT_FOUND )
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#5 Nominal";

    /* Setup */
    res = OS_BinSemCreate(&bin_sem_id, "GetIDByName", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#5 Nominal - Bin Sem Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
       res = OS_BinSemGetIdByName(&bin_sem_id, "GetIDByName");
       if ( res == OS_SUCCESS )
          UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
       else
          UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

       res = OS_BinSemDelete(bin_sem_id);
    }

UT_os_bin_sem_get_id_by_name_test_exit_tag:
    return;
    
}

/*--------------------------------------------------------------------------------*
** Syntax: OS_BinSemGetInfo
** Purpose: Returns semaphore information about the given binary semaphore id
** Parameters: To-be-filled-in
** Returns: OS_INVALID_POINTER if the pointer passed in is null
**          OS_ERR_INVALID_ID if the id passed in is not a valid binary semaphore id
**          OS_SUCCESS if succeeded
** Test #1: TBD
**--------------------------------------------------------------------------------*/
void UT_os_bin_sem_get_info_test()
{
    int32              res = 0;
    const char*        testDesc;
    uint32             bin_sem_id;
    OS_bin_sem_prop_t  bin_sem_prop;

    /*-----------------------------------------------------*/
    testDesc = "API not implemented";

    res = OS_BinSemGetInfo(0, &bin_sem_prop);
    if (res == OS_ERR_NOT_IMPLEMENTED)
    {
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_NA);
        goto UT_os_bin_sem_get_info_test_exit_tag;
    }

    /*-----------------------------------------------------*/
    testDesc = "#1 Invalid-ID-arg";

    res = OS_BinSemGetInfo(99999, &bin_sem_prop);
    if ( res == OS_ERR_INVALID_ID )
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
    else
       UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

    /*-----------------------------------------------------*/
    testDesc = "#2 Invalid-pointer-arg";

    /* Setup */
    res = OS_BinSemCreate(&bin_sem_id, "GetInfo", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#2 Invalid-pointer-arg - Bin Sem Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_BinSemGetInfo(bin_sem_id, NULL);
        if ( res == OS_INVALID_POINTER )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_BinSemDelete(bin_sem_id);
    }

    /*-----------------------------------------------------*/
    testDesc = "#3 Nominal";

    /* Setup */
    res = OS_BinSemCreate(&bin_sem_id, "GetInfo", 1, 0);
    if ( res != OS_SUCCESS )
    {
        testDesc = "#3 Nominal - Bin Sem Create failed";
        UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_TSF);
    }
    else
    {
        res = OS_BinSemGetInfo(bin_sem_id, &bin_sem_prop);
        if ( res == OS_SUCCESS )
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_PASS);
        else
            UT_OS_TEST_RESULT( testDesc, UTASSERT_CASETYPE_FAILURE);

        res = OS_BinSemDelete(bin_sem_id);
    }

UT_os_bin_sem_get_info_test_exit_tag:
    return;
    
}

/*================================================================================*
** End of File: ut_oscore_binsem_test.c
**================================================================================*/
