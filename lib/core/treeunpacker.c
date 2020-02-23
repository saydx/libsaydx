/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */
#include <stdlib.h>
#include "treeunpacker.h"
#include "commontypes.h"
#include "eventhandler.h"
#include "deserializer.h"
#include "txtdeserializer.h"
#include "array.h"


void _unpack_attributes(deserializer_t *deserializer, void **curpos, attributes_t **attribs)
{
    void *dhandler = deserializer->handler;
    int nattrib;

    deserializer->get_int4(dhandler, curpos, &nattrib);
    *attribs = attributes_create(nattrib);
    attribute_t *attrib;
    for (int iattrib = 0; iattrib < nattrib; ++iattrib) {
        attrib = MALLOC_OR_DIE(sizeof(*attrib));
        deserializer->get_string(dhandler, curpos, &attrib->name);
        deserializer->get_string(dhandler, curpos, &attrib->value);
        attributes_append(*attribs, attrib);
    }
}


error_t *treeunpacker_unpack(treeunpacker_t *this, const void *blob, size_t blobsize)
{
    deserializer_t *deserializer = this->deserializer;
    void *dhandler = this->deserializer->handler;
    eventhandler_t *eventhandler = this->eventhandler;
    void *ehandler = this->eventhandler->handler;
    int action;
    void *curpos = (void *) blob;
    char *lastpos = (char *) curpos + blobsize;

    char *str;
    int nattrib;
    attributes_t *attribs;
    array_t *array;
    size_t rawdatasize;

    eventhandler->start_processing(ehandler, "<BLOB>");

    while ((char *) curpos < lastpos) {
        deserializer->get_int4(deserializer, &curpos, &action);

        switch (action) {

        case OPEN_CONTAINER_NODE_EVENT:
            deserializer->get_string(dhandler, &curpos, &str);
            _unpack_attributes(deserializer, &curpos, &attribs);
            eventhandler->open_container_node(ehandler, str, attribs);
            free(str);
            attributes_destroy(attribs);
            break;

        case CLOSE_CONTAINER_NODE_EVENT:
            eventhandler->close_container_node(ehandler);
            break;

        case OPEN_DATA_NODE_EVENT:
            deserializer->get_string(dhandler, &curpos, &str);
            _unpack_attributes(deserializer, &curpos, &attribs);
            eventhandler->open_data_node(ehandler, str, attribs);
            free(str);
            attributes_destroy(attribs);
            break;

        case CLOSE_DATA_NODE_EVENT:
            eventhandler->close_container_node(ehandler);
            break;

        case RECEIVE_DATA_EVENT:
            array = array_create();
            deserializer->get_string(dhandler, &curpos, &array->typename);
            deserializer->get_int4(dhandler, &curpos, &array->rank);
            if (array->rank > 0) {
                deserializer->get_int4v(dhandler, &curpos, array->rank, &array->shape);
            }
            deserializer->get_raw_data(dhandler, &curpos, &array->rawdata, &rawdatasize);
            eventhandler->receive_array(ehandler, array);
            array_destroy(array);
        }
    }

    eventhandler->end_processing(ehandler, "<BLOB>");
}


void treeunpacker_init(treeunpacker_t *treeunpacker, eventhandler_t *eventhandler,
                       deserializer_t *deserializer)
{
    treeunpacker->deserializer = deserializer;
    treeunpacker->eventhandler = eventhandler;
}
