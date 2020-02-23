/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <stdlib.h>
#include <string.h>
#include "treebuilder.h"
#include "commontypes.h"
#include "node.h"
#include "eventhandler.h"


typedef struct _treebuilder_t {
    node_t *root, *curnode;
    int level;
} _treebuilder_t;


treebuilder_t * treebuilder_create()
{
    treebuilder_t *this = MALLOC_OR_DIE(sizeof(*this));
    this->root = MALLOC_OR_DIE(sizeof(*this->root));
    node_init(this->root, "ROOT", 1, NULL);
    this->curnode = this->root;
    this->level = 0;
    return this;
}


void treebuilder_destroy(void *handler)
{
    treebuilder_t *this = handler;
    if (this->root) {
        node_final(this->root);
        free(this->root);
    }
    free(this);
}


void treebuilder_transfer_tree(treebuilder_t *this, node_t **target)
{
    *target = this->root;
    this->root = NULL;
}


error_t * treebuilder_start_processing(void *handler, const char *filename)
{
    treebuilder_t *treebuilder = handler;

    treebuilder->level = 0;
    printf("-> START PROCESSING: %s\n", filename);
    RETURN_WITHOUT_ERROR();
}


error_t * treebuilder_end_processing(void *handler, const char *filename)
{
    printf("<- END PROCESSING: %s\n", filename);
    RETURN_WITHOUT_ERROR();
}


error_t * treebuilder_open_container_node(void *handler, const char *tagname, attributes_t *attribs)
{
    treebuilder_t *treebuilder = handler;
    node_t *node;

    node = MALLOC_OR_DIE(sizeof(*node));
    node_init(node, tagname, 1, attribs);
    node_append_child(treebuilder->curnode, node);
    treebuilder->curnode = node;
    RETURN_WITHOUT_ERROR();
}


error_t * treebuilder_close_container_node(void *handler)
{
    treebuilder_t *treebuilder = handler;

    treebuilder->curnode = treebuilder->curnode->parent;
    RETURN_WITHOUT_ERROR();
}


error_t * treebuilder_open_data_node(void *handler, const char *tagname, attributes_t *attribs)
{
    treebuilder_t *treebuilder = handler;

    node_t *node;

    node = MALLOC_OR_DIE(sizeof(*node));
    node_init(node, tagname, 2, attribs);
    node_append_child(treebuilder->curnode, node);
    treebuilder->curnode = node;
    RETURN_WITHOUT_ERROR();
}


error_t * treebuilder_close_data_node(void *handler)
{
    treebuilder_t *treebuilder = handler;

    treebuilder->curnode = treebuilder->curnode->parent;
    RETURN_WITHOUT_ERROR();
}


error_t * treebuilder_receive_array(void *handler, array_t *array)
{
    treebuilder_t *treebuilder = handler;

    treebuilder->curnode->array = array_reference(array);
    RETURN_WITHOUT_ERROR();
}


eventhandler_t * treebuilder_cast_to_eventhandler(treebuilder_t *treebuilder)
{
    eventhandler_t *eventhandler = MALLOC_OR_DIE(sizeof(*eventhandler));
    eventhandler->handler = treebuilder;
    eventhandler->start_processing = treebuilder_start_processing;
    eventhandler->end_processing = treebuilder_end_processing;
    eventhandler->open_container_node = treebuilder_open_container_node;
    eventhandler->close_container_node = treebuilder_close_container_node;
    eventhandler->open_data_node = treebuilder_open_data_node;
    eventhandler->close_data_node = treebuilder_close_data_node;
    eventhandler->receive_array = treebuilder_receive_array;
    eventhandler->final = treebuilder_destroy;
    return eventhandler;
}
