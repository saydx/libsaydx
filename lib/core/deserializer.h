/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef SAYDX_DESERIALIZER_H
#define SAYDX_DESERIALIZER_H

#include "commontypes.h"

typedef void (*deserializer_get_int4_t)(void *, void **, int *);
typedef void (*deserializer_get_int4v_t)(void *, void **, size_t, int **);
typedef void (*deserializer_get_string_t)(void *, void **, char **);
typedef void (*deserializer_get_byte_t)(void *, void **, unsigned char *);
typedef void (*deserializer_get_raw_data_t)(void *, void **, void **, size_t *);
typedef void (*deserializer_final_t)(void *);

typedef struct _deserializer_t {
    void *handler;
    deserializer_get_int4_t get_int4;
    deserializer_get_int4v_t get_int4v;
    deserializer_get_string_t get_string;
    deserializer_get_byte_t get_byte;
    deserializer_get_raw_data_t get_raw_data;
    deserializer_final_t final;
} _deserializer_t;


#endif
