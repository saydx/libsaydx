/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <stdlib.h>
#include "eventprinter.h"


void eventprinter_init(eventprinter_t *this)
{
    this->level = 0;
}


void eventprinter_final(eventprinter_t *this)
{}


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


error_t * eventprinter_close_container_node(void *handler, const char *tagname)
{
    eventprinter_t *eventprinter = handler;

    eventprinter->level--;
    eventprinter_print_indentation(eventprinter->level);
    printf("CLOSE CONTAINER NODE: %s\n", tagname);
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


//int main()
//{
//    eventprinter_t eventprinter;
//    msdparser_input_t msdinput;
//    msdparser_t msdparser;
//    eventprinter_t treebuilder;
//  
//    msdinput.eventhandler.handler = &treebuilder;
//    msdinput.eventhandler.start_processing = &eventprinter_start_processing;
//    msdinput.eventhandler.end_processing = &eventprinter_end_processing;
//    msdinput.eventhandler.open_container_node = &eventprinter_open_container_node;
//    msdinput.eventhandler.open_data_node = &eventprinter_open_data_node;
//    msdinput.eventhandler.close_container_node = &eventprinter_close_container_node;
//    msdinput.eventhandler.close_data_node = &eventprinter_close_data_node;
//    msdinput.eventhandler.receive_array = &eventprinter_receive_array;
//    msdparser_init(&msdparser, &msdinput);
//    msdparser_parse_file(&msdparser, "test.msd");
//    msdparser_final(&msdparser);
//    return 0;
//}


