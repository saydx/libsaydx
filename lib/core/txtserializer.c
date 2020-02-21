/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <string.h>
#include "txtserializer.h"

#define BUFFER_SIZE 100


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


void txtserializer_final(void *handler)
{
    // Nothing to destruct
}


void serializer_init_txtserializer(serializer_t *this)
{
    this->handler = NULL;
    this->add_int4 = txtserializer_add_int4;
    this->add_int4v = txtserializer_add_int4v;
    this->add_string = txtserializer_add_string;
    this->add_byte = txtserializer_add_byte;
    this->add_raw_data = txtserializer_add_raw_data;
    this->final = txtserializer_final;
}
