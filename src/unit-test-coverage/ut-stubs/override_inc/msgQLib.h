/**
 * \file
 * \ingroup ut-stubs
 *
 * OSAL coverage stub replacement for msgQLib.h
 */

#ifndef OVERRIDE_MSGQLIB_H
#define OVERRIDE_MSGQLIB_H

#include "OCS_msgQLib.h"
#include <vxWorks.h>
#include <objLib.h>

/* ----------------------------------------- */
/* mappings for declarations in msgQLib.h */
/* ----------------------------------------- */
#define MSG_Q_FIFO             OCS_MSG_Q_FIFO
#define MSG_Q_PRIORITY         OCS_MSG_Q_PRIORITY
#define WIND_MSG_Q_OPTION_MASK OCS_WIND_MSG_Q_OPTION_MASK
#define MSG_PRI_NORMAL         OCS_MSG_PRI_NORMAL
#define MSG_PRI_URGENT         OCS_MSG_PRI_URGENT

#define MSG_Q_ID OCS_MSG_Q_ID

#define msgQCreate  OCS_msgQCreate
#define msgQDelete  OCS_msgQDelete
#define msgQReceive OCS_msgQReceive
#define msgQSend    OCS_msgQSend

#endif /* OVERRIDE_MSGQLIB_H */
