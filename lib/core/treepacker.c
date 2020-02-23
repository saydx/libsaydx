/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "stdlib.h"
#include "treepacker.h"
#include "array.h"

typedef struct _treepacker_t {
    blob_t *blob;
    serializer_t *serializer;
} _treepacker_t;


error_t * treepacker_start_processing(void *handler, const char *filename)
{
    RETURN_WITHOUT_ERROR();
}


error_t * treepacker_end_processing(void *handler, const char *filename)
{
    RETURN_WITHOUT_ERROR();
}


void _treepacker_attributes(serializer_t *serializer, void *shandler, blob_t *blob,
                            attributes_t *attribs)
{
    serializer->add_int4(shandler, blob, attribs->size);
    for (int iattrib = 0; iattrib < attribs->size; ++iattrib) {
        serializer->add_string(shandler, blob, attribs->attributes[iattrib].name);
        serializer->add_string(shandler, blob, attribs->attributes[iattrib].value);
    }
}


error_t * treepacker_open_container_node(void *handler, const char *tagname, attributes_t *attribs)
{
    treepacker_t *this = handler;
    serializer_t *serializer = this->serializer;
    void *shandler = this->serializer->handler;

    serializer->add_int4(shandler, this->blob, 1);
    serializer->add_string(shandler, this->blob, tagname);
    _treepacker_attributes(serializer, shandler, this->blob, attribs);
    RETURN_WITHOUT_ERROR();
}


error_t * treepacker_close_container_node(void *handler)
{
    treepacker_t *this = handler;
    serializer_t *serializer = this->serializer;
    void *shandler = this->serializer->handler;

    serializer->add_int4(shandler, this->blob, -1);
    RETURN_WITHOUT_ERROR();
}


error_t * treepacker_open_data_node(void *handler, const char *tagname, attributes_t *attribs)
{
    treepacker_t *this = handler;
    serializer_t *serializer = this->serializer;
    void *shandler = this->serializer->handler;

    serializer->add_int4(shandler, this->blob, 2);
    serializer->add_string(shandler, this->blob, tagname);
    _treepacker_attributes(serializer, shandler, this->blob, attribs);
    RETURN_WITHOUT_ERROR();
}


error_t * treepacker_close_data_node(void *handler)
{
    treepacker_t *this = handler;
    serializer_t *serializer = this->serializer;
    void *shandler = this->serializer->handler;

    serializer->add_int4(shandler, this->blob, -2);
    RETURN_WITHOUT_ERROR();
}


error_t * treepacker_receive_array(void *handler, array_t *array)
{
    treepacker_t *this = handler;
    serializer_t *serializer = this->serializer;
    void *shandler = this->serializer->handler;

    serializer->add_int4(shandler, this->blob, 3);
    serializer->add_string(shandler, this->blob, array->typename);
    serializer->add_int4(shandler, this->blob, array->rank);
    if (array->rank > 0) {
        serializer->add_int4v(shandler, this->blob, array->shape, array->rank);
    }
    serializer->add_raw_data(shandler, this->blob, array->rawdata, array_byte_size(array));
    RETURN_WITHOUT_ERROR();
}



treepacker_t * treepacker_create(blob_t *blob, serializer_t *serializer)
{
    treepacker_t * treepacker = MALLOC_OR_DIE(sizeof(*treepacker));
    treepacker->blob = blob;
    treepacker->serializer = serializer;
    return treepacker;
}


void treepacker_destroy(void *handler)
{
    treepacker_t *treepacker = handler;
    free(treepacker);
}


eventhandler_t * treepacker_cast_to_eventhandler(treepacker_t *treepacker)
{
    eventhandler_t *eventhandler = MALLOC_OR_DIE(sizeof(*eventhandler));
    eventhandler->handler = treepacker;
    eventhandler->start_processing = treepacker_start_processing;
    eventhandler->end_processing = treepacker_end_processing;
    eventhandler->open_container_node = treepacker_open_container_node;
    eventhandler->close_container_node = treepacker_close_container_node;
    eventhandler->open_data_node = treepacker_open_data_node;
    eventhandler->close_data_node = treepacker_close_data_node;
    eventhandler->receive_array = treepacker_receive_array;
    eventhandler->final = treepacker_destroy;
    return eventhandler;
}
