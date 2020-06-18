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
** This program is an example of how to parse a symbol table dump file.
** On linux: build with $ gcc -o dumpsymtab dumpsymtab.c
**           run with   $ ./dumpsymtab | sort > syms.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


typedef struct  
{
   char name[64]; /* Needs to match OS_MAX_SYM_NAME in osconfig.h */
   unsigned int value;
} symRecord;

int main (void)
{
   int       symfile;
   int       status;
   int       keepgoing = 1;
   symRecord symrec;

   /*
   ** Open file
   */
   symfile = open("SymbolTable512k.dat", O_RDONLY);
   if ( symfile > 0 )
   {
       printf("Symbol file opened\n");
       printf("Size of SymRecord = %d\n",sizeof(symRecord));
       while ( keepgoing == 1 )
       {
          /*
          ** read symbol entry from file
          */
          status = read(symfile, (void *)&symrec, sizeof(symRecord));
          if ( status == sizeof(symRecord))
          {
             printf("%s \t: 0x%08X\n",symrec.name, symrec.value);
          }
          else
          {
             printf("Size read = %d\n",status);
             close(symfile);
             keepgoing = 0;
          }
       }
   }
   else
   {
      printf("Can't open file\n");
   }
 
   return 0;
}

