/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef SAYDX_SERIALIZER_H
#define SAYDX_SERIALIZER_H

#include "commontypes.h"

typedef void (*serializer_add_int4_t)(void *, blob_t *, int);
typedef void (*serializer_add_int4v_t)(void *, blob_t *, const int *, size_t);
typedef void (*serializer_add_string_t)(void *, blob_t *, const char *);
typedef void (*serializer_add_byte_t)(void *, blob_t *, unsigned char);
typedef void (*serializer_final_t)(void *);

typedef struct {
    void *handler;
    serializer_add_int4_t add_int4;
    serializer_add_int4v_t add_int4v;
    serializer_add_string_t add_string;
    serializer_add_byte_t add_byte;
    serializer_final_t final;
} serializer_t;


#endif
