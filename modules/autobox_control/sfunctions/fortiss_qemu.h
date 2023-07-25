/******************************************************************************
 * Apollo Autobox Control
 * Copyright (C) 2020 fortiss GmbH
 * Authors: Tobias Kessler, Jianjie Lin
 * 
 * This library is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU Lesser General Public License as published by the 
 * Free Software Foundation; either version 2.1 of the License, or (at your 
 * option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License 
 * for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License 
 * along with this library; if not, write to the Free Software Foundation, 
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *****************************************************************************/

#ifndef _FORTISS_QEMU_H
#define _FORTISS_QEMU_H

#include <stdio.h>

/*
 * Message outputs
 */
#define FORTISS_INFO_PRINTF(...)                   \
    do {                                                \
        printf(__VA_ARGS__);                            \
    } while (0)

#define FORTISS_WARNING_PRINTF(S, ...)                \
    do {                                                \
         static char *msg[MAX_MSG_LEN];                      \
        snprintf(msg, MAX_MSG_LEN, __VA_ARGS__);           \
       printf(msg);                               \
    } while (0)

#define FORTISS_ERROR_PRINTF(S, ...)                  \
    do {                                                    \
        static char *msg[MAX_MSG_LEN];                      \
        snprintf(msg, MAX_MSG_LEN, __VA_ARGS__);            \
    } while (0)

/*
 * End simulation
 */
#define FORTISS_EXIT(err)                                                      \
    do {                                                                    \
        printf("Application stopped.\n");                      \
        return;                                                             \
    } while (0);


#endif
