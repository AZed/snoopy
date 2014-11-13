/*
 * SNOOPY LOGGER
 *
 * File: snoopy/filter/only_root.c
 *
 * Copyright (c) 2014 bostjan@a2o.si
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */



/*
 * SNOOPY FILTER: only_root
 *
 * Description:
 *     Only logs messages from root (uid=0 actually)
 *
 * Params:
 *     msg:   pointer to string that contains formatted log message (may be manipulated)
 *     arg:   arguments passed to this filter
 *
 * Return:
 *     SNOOPY_FILTER_PASS or SNOOPY_FILTER_DROP
 */
#include "snoopy.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>



int snoopy_filter_only_root (char *msg, char *arg)
{
    if (0 == getuid()) {
        return SNOOPY_FILTER_PASS;
    } else {
        return SNOOPY_FILTER_DROP;
    }
}
