/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef SAYDX_EVENTHANDLER_H
#define SAYDX_EVENTHANDLER_H

#include "commontypes.h"

typedef error_t * (*eventhandler_start_processing_t)(void *, const char *);
typedef error_t * (*eventhandler_end_processing_t)(void *, const char *);
typedef error_t * (*eventhandler_open_container_node_t)(void *, const char *, attributes_t *);
typedef error_t * (*eventhandler_open_data_node_t)(void *, const char *, attributes_t *);
typedef error_t * (*eventhandler_close_container_node_t)(void *, const char *);
typedef error_t * (*eventhandler_close_data_node_t)(void *);
typedef error_t * (*eventhandler_receive_array_t)(void *, array_t *);
//typedef void (*eventhandler_final_t)(void *);

typedef struct {
    void *handler;
    eventhandler_start_processing_t start_processing;
    eventhandler_end_processing_t end_processing;
    eventhandler_open_container_node_t open_container_node;
    eventhandler_close_container_node_t close_container_node;
    eventhandler_open_data_node_t open_data_node;
    eventhandler_close_data_node_t close_data_node;
    eventhandler_receive_array_t receive_array;
    //eventhandler_final_t final;
} eventhandler_t;


#endif
