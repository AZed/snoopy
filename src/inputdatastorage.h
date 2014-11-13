/*
 * SNOOPY LOGGER
 *
 * File: inputdatastorage.h
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
#include "snoopy.h"



/*
 * Store execution data for inputs to consume
 */
extern const char *snoopy_inputdatastorage_filename;
extern char **snoopy_inputdatastorage_argv;
extern char **snoopy_inputdatastorage_envp;



/*
 * Functions to use to do the actual storing
 */
void snoopy_inputdatastorage_store_filename (
    const char *filename
);
void snoopy_inputdatastorage_store_argv (
    char * argv[]
);
void snoopy_inputdatastorage_store_envp (
    char * envp[]
);
