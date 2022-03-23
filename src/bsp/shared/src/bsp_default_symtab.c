/*
 * File:  bsp_symtab.c
 *
 * Purpose:
 *   Application static symbol table default implementation.
 *
 * NOTE: This is isolated in a separate compilation unit, so that a user
 *   application may directly provide an OS_STATIC_SYMBOL_TABLE definition
 *   which will override this default.
 */

#include "osapi-module.h"
#include "bsp-impl.h"

OS_static_symbol_record_t OS_STATIC_SYMBOL_TABLE[] = {
    {"OS_Application_Startup", OS_Application_Startup}, {"OS_Application_Run", OS_Application_Run}, {NULL, NULL}};
