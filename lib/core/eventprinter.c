/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <stdlib.h>
#include "eventprinter.h"
#include "array.h"


typedef struct _eventprinter_t {
    int level;
} _eventprinter_t;


eventprinter_t * eventprinter_create()
{
    eventprinter_t *this = MALLOC_OR_DIE(sizeof(*this));
    this->level = 0;
    return this;
}


void eventprinter_destroy(void *handler)
{
    eventprinter_t *eventprinter = handler;
    free(handler);
}


void eventprinter_print_indentation(int level)
{
    for (int ii = 0; ii < level; ++ii) {
        printf("  ");
    }
}


void eventprinter_print_attributes(attributes_t *attribs, int indentlevel)
{
    for (attribute_t *pattrib = attribs->attributes;
         pattrib < attribs->attributes + attribs->nattributes;
         ++pattrib) {
        eventprinter_print_indentation(indentlevel);
        printf("&%s=%s\n", pattrib->name, pattrib->value);
    }
}


error_t * eventprinter_start_processing(void *handler, const char *filename)
{
    eventprinter_t *eventprinter = handler;

    printf("-> START PROCESSING: %s\n", filename);
    RETURN_WITHOUT_ERROR();
}


error_t * eventprinter_end_processing(void *handler, const char *filename)
{
    printf("<- END PROCESSING: %s\n", filename);
    RETURN_WITHOUT_ERROR();
}


error_t * eventprinter_open_container_node(void *handler, const char *tagname,
                                           attributes_t *attribs)
{
    eventprinter_t *eventprinter = handler;

    eventprinter_print_indentation(eventprinter->level);
    printf("OPEN CONTAINER NODE: %s\n", tagname);
    eventprinter_print_attributes(attribs, eventprinter->level);
    eventprinter->level++;
    RETURN_WITHOUT_ERROR();
}


error_t * eventprinter_close_container_node(void *handler)
{
    eventprinter_t *eventprinter = handler;

    eventprinter->level--;
    eventprinter_print_indentation(eventprinter->level);
    printf("CLOSE CONTAINER NODE\n");
    RETURN_WITHOUT_ERROR();
}


error_t * eventprinter_open_data_node(void *handler, const char *tagname, attributes_t *attribs)
{
    eventprinter_t *eventprinter = handler;

    eventprinter_print_indentation(eventprinter->level);
    printf("OPEN DATA NODE: %s\n", tagname);
    eventprinter_print_attributes(attribs, eventprinter->level);
    eventprinter->level++;
    RETURN_WITHOUT_ERROR();
}


error_t * eventprinter_close_data_node(void *handler)
{
    eventprinter_t *eventprinter = handler;

    eventprinter->level--;
    eventprinter_print_indentation(eventprinter->level);
    printf("CLOSE DATA NODE\n");
    RETURN_WITHOUT_ERROR();
}


error_t * eventprinter_receive_array(void *handler, array_t *array)
{
    eventprinter_t *eventprinter = handler;

    eventprinter_print_indentation(eventprinter->level);
    printf("RECEIVED ARRAY (type: %s, items: %d)\n", array->typename, array_size(array));
    RETURN_WITHOUT_ERROR();
}



eventhandler_t * eventprinter_cast_to_eventhandler(eventprinter_t *eventprinter)
{
    eventhandler_t *eventhandler = MALLOC_OR_DIE(sizeof(*eventhandler));
    eventhandler->handler = eventprinter;
    eventhandler->start_processing = &eventprinter_start_processing;
    eventhandler->end_processing = &eventprinter_end_processing;
    eventhandler->open_container_node = &eventprinter_open_container_node;
    eventhandler->close_container_node = &eventprinter_close_container_node;
    eventhandler->open_data_node = &eventprinter_open_data_node;
    eventhandler->close_data_node = &eventprinter_close_data_node;
    eventhandler->receive_array = &eventprinter_receive_array;
    eventhandler->final = &eventprinter_destroy;
    return eventhandler;
}

