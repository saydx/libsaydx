/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <string.h>
#include <stdlib.h>
#include "txtserializer.h"

#define BUFFER_SIZE 100


struct _txtserializer_t {
} _txt_serializer_t;


void txtserializer_add_int4(void *handler, blob_t * blob, int data)
{
    char buffer[BUFFER_SIZE];
    snprintf(buffer, BUFFER_SIZE, "%d", data);
    blob_add_bytes(blob, buffer, strlen(buffer));
    blob_add_bytes(blob, "|", 1);
}


void txtserializer_add_int4v(void *handler, blob_t * blob, const int *data, size_t datasize)
{
    char buffer[BUFFER_SIZE];
    for (size_t ii = 0; ii < datasize; ++ii) {
        snprintf(buffer, BUFFER_SIZE, "%d", data[ii]);
        blob_add_bytes(blob, buffer, strlen(buffer));
        blob_add_bytes(blob, ii < datasize - 1 ? "," : "|", 1);
    }
}


void txtserializer_add_string(void *handler, blob_t * blob, const char *data)
{
    txtserializer_add_int4(handler, blob, strlen(data));
    blob_add_bytes(blob, data, strlen(data));
    blob_add_bytes(blob, "|", 1);
}


void txtserializer_add_byte(void *handler, blob_t * blob, const unsigned char data)
{
    blob_add_bytes(blob, &data, 1);
    blob_add_bytes(blob, "|", 1);
}


void txtserializer_add_raw_data(void *handler, blob_t * blob, const void *data, size_t nbyte)
{
    char buffer[3];
    const unsigned char *bytedata = data;
    
    txtserializer_add_int4(handler, blob, nbyte);
    for (size_t ii = 0; ii < nbyte; ++ii) {
        snprintf(buffer, BUFFER_SIZE, "%03u", bytedata[ii]);
        blob_add_bytes(blob, buffer, strlen(buffer));
        blob_add_bytes(blob, ii < nbyte - 1 ? "," : "|", 1);
    }
}


txtserializer_t * txtserializer_create()
{
    txtserializer_t *txtserializer = MALLOC_OR_DIE(sizeof(*txtserializer));
    return txtserializer;
}


void txtserializer_destroy(void *handler)
{
    txtserializer_t *txtserializer = handler;
    free(txtserializer);
}


serializer_t * txtserializer_cast_to_serializer(txtserializer_t *txtserializer)
{
    serializer_t *serializer = MALLOC_OR_DIE(sizeof(*serializer));
    serializer->handler = txtserializer;
    serializer->add_int4 = txtserializer_add_int4;
    serializer->add_int4v = txtserializer_add_int4v;
    serializer->add_string = txtserializer_add_string;
    serializer->add_byte = txtserializer_add_byte;
    serializer->add_raw_data = txtserializer_add_raw_data;
    serializer->final = txtserializer_destroy;
}
