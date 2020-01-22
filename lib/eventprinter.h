/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef SAYDX_EVENTPRINTER_H
#define SAYDX_EVENTPRINTER_H

#include "commontypes.h"

typedef struct {
    int level;
} eventprinter_t;

void eventprinter_init(eventprinter_t *this);
void eventprinter_final(eventprinter_t *this);
error_t * eventprinter_start_processing(void *handler, const char *filename);
error_t * eventprinter_end_processing(void *handler, const char *filename);
error_t * eventprinter_open_container_node(void *handler, const char *tagname,
                                           attributes_t *attribs);
error_t * eventprinter_close_container_node(void *handler, const char *tagname);
error_t * eventprinter_open_data_node(void *handler, const char *tagname, attributes_t *attribs);
error_t * eventprinter_close_data_node(void *handler);
error_t * eventprinter_receive_array(void *handler, array_t *array);

#endif
