#ifndef __CLUB_INCLUDE__
#define __CLUB_INCLUDE__


#define DEFAULT_VALUE2 0

#define TASK_ITEM(EntryFunc, TaskId, uwArg, TaskStackSize, pcName, uwResved) {(TSK_ENTRY_FUNC)EntryFunc, TaskId, uwArg, TaskStackSize, pcName, uwResved},
//#define TASK_LIST() {#include "tasklist.h"}

#endif 

