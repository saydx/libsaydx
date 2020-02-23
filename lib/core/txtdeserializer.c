/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <stdlib.h>
#include <string.h>
#include "txtdeserializer.h"


struct _txtdeserializer_t {
} _txtdeserializer_t;


void txtdeserializer_get_int4(void *handler, void **blob, int *data)
{
    char *startptr = *blob;
    char *endptr;
    long buffer = strtol(startptr, &endptr, 10);
    *data = (int) buffer;
    *blob = endptr + 1;
}


void txtdeserializer_get_int4v(void *handler, void **blob, size_t nitem, int **data)
{
    char *startptr = *blob;
    char *endptr;
    long buffer;
    *data = MALLOC_OR_DIE(sizeof(**data * nitem));
    for (size_t ii = 0; ii < nitem; ++ii) {
        buffer = strtol(startptr, &endptr, 10);
        (*data)[ii] = (int) buffer;
        startptr = endptr + 1;
    }
    *blob = startptr;
}


void txtdeserializer_get_string(void *handler, void **blob, char **data)
{
    char *startptr = *blob;
    char *nextpos;
    int len;
    txtdeserializer_get_int4(handler, blob, &len);

    *data = MALLOC_OR_DIE(sizeof(**data) * (len + 1));
    memcpy(*data, *blob, len);
    (*data)[len] = '\0';
    *blob = (char *) *blob + len + 1;
}


void txtdeserializer_get_byte(void *handler, void **blob, unsigned char *data)
{
    int buffer;
    txtdeserializer_get_int4(handler, blob, &buffer);
    *data = (unsigned char) buffer;
}


void txtdeserializer_get_raw_data(void *handler, void **blob, void **data, size_t *nbyte)
{
    int buffer;
    unsigned char *bytedata;

    txtdeserializer_get_int4(handler, blob, &buffer);
    *nbyte = (size_t) buffer;
    bytedata = MALLOC_OR_DIE(*nbyte);
    for (size_t ii = 0; ii < *nbyte; ++ii) {
        txtdeserializer_get_int4(handler, blob, &buffer);
        bytedata[ii] = (unsigned char) buffer;
    }
    *data = bytedata;
}


txtdeserializer_t *txtdeserializer_create()
{
    txtdeserializer_t *this = MALLOC_OR_DIE(sizeof(*this));
    return this;
}


void txtdeserializer_destroy(void *handler)
{
    txtdeserializer_t *this = handler;
    free(this);
}


deserializer_t * txtdeserializer_cast_to_deserializer(txtdeserializer_t *txtdeserializer)
{
    deserializer_t *deserializer = MALLOC_OR_DIE(sizeof(*deserializer));
    deserializer->handler = txtdeserializer;
    deserializer->get_int4 = txtdeserializer_get_int4;
    deserializer->get_int4v = txtdeserializer_get_int4v;
    deserializer->get_string = txtdeserializer_get_string;
    deserializer->get_byte = txtdeserializer_get_byte;
    deserializer->get_raw_data = txtdeserializer_get_raw_data;
    deserializer->final = txtdeserializer_destroy;
}
