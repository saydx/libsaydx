/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <stdlib.h>
#include <string.h>
#include "treebuilder.h"


void treebuilder_init(treebuilder_t *this)
{
    this->root = MALLOC_OR_DIE(sizeof(*this->root));
    node_init(this->root, "ROOT", 1, NULL);
    this->curnode = this->root;
    this->level = 0;
}


void treebuilder_final(treebuilder_t *this)
{
    if (this->root) {
        node_final(this->root);
        free(this->root);
    }
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


error_t * treebuilder_close_container_node(void *handler, const char *tagname)
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

