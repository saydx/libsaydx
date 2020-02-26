/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <stdlib.h>
#include <string.h>
#include "node.h"

//
// node_t
//

node_t * node_create(const char *name, int nodetype, attributes_t *attributes)
{
    node_t *node = MALLOC_OR_DIE(sizeof(*node));
    node->nrefs = 1;
    node->name = MALLOC_OR_DIE((strlen(name) + 1) * sizeof(*node->name));
    strcpy(node->name, name);
    node->parent = NULL;
    if (attributes) {
        node->attributes = attributes_reference(attributes);
    } else {
        node->attributes = attributes_create(0);
    }
    node->array = NULL;
    switch (nodetype) {
    case 1:
        node->children = node_list_create(4);
        break;
    case 2:
        node->children = NULL;
        break;
    }
    return node;
}


void _node_final(node_t *this)
{
    free(this->name);
    attributes_dereference(this->attributes);
    if (this->children) {
        node_list_destroy(this->children);
    }
    if (this->array) {
        array_dereference(this->array);
    }
}


node_t * node_reference(node_t *this)
{
    this->nrefs++;
    return this;
}


void node_dereference(node_t *this)
{
    if (!this) {
        return;
    }
    this->nrefs--;
    if (this->nrefs) {
        return;
    }
    _node_final(this);
    free(this);
}


char * node_get_name(node_t *this)
{
    return this->name;
}


bool node_has_parent(node_t *this)
{
    return this->parent != NULL;
}


void node_append_child(node_t *this, node_t *child)
{
    child->parent = this;
    node_list_append(this->children, child);
}


//
// node_list_t
//

node_list_t * node_list_create(int initsize)
{
    node_list_t *this = MALLOC_OR_DIE(sizeof(*this));
    this->size = 0;
    this->allocsize = initsize;
    this->items = MALLOC_OR_DIE(this->allocsize * sizeof(*this->items));
    return this;
}


void node_list_destroy(node_list_t *this)
{
    for (int ichild = 0; ichild < this->size; ++ichild) {
        node_dereference(this->items[ichild]);
    }
    free(this->items);
    free(this);
}


void node_list_append(node_list_t *this, node_t *node)
{
    this->size++;
    if (this->size > this->allocsize) {
        this->allocsize = (int) ((float) this->size * 1.5) + 1;
        this->items = realloc(this->items, this->allocsize * sizeof(*this->items));
    }
    this->items[this->size - 1] = node_reference(node);
}
