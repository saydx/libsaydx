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

void node_init(node_t *this, const char *name, int nodetype, attributes_t *attributes)
{
    this->name = MALLOC_OR_DIE((strlen(name) + 1) * sizeof(*this->name));
    strcpy(this->name, name);
    this->parent = NULL;
    if (attributes) {
        this->attributes = attributes_reference(attributes);
    } else {
        this->attributes = attributes_create(0);
    }
    this->array = NULL;
    switch (nodetype) {
    case 1:
        this->children = MALLOC_OR_DIE(sizeof(*this->children));
        node_list_init(this->children, 4);
        break;
    case 2:
        this->children = NULL;
        break;
    }
}


void node_final(node_t *this)
{
    free(this->name);
    attributes_destroy(this->attributes);
    if (this->children) {
        for (int ichild = 0; ichild < this->children->size; ++ichild) {
            node_final(this->children->items[ichild]);
            free(this->children->items[ichild]);
        }
        node_list_final(this->children);
        free(this->children);
    }
    if (this->array) {
        array_destroy(this->array);
    }
}

char * node_get_name(node_t *this)
{
    return this->name;
}


void node_append_child(node_t *this, node_t *child)
{
    child->parent = this;
    node_list_append(this->children, child);
}


//
// node_list_t
//

void node_list_init(node_list_t *this, int initsize)
{
    this->size = 0;
    this->allocsize = initsize;
    this->items = MALLOC_OR_DIE(this->allocsize * sizeof(*this->items));
}


void node_list_final(node_list_t *this)
{
    free(this->items);
}


void node_list_append(node_list_t *this, node_t *node)
{
    this->size++;
    if (this->size > this->allocsize) {
        this->allocsize = (int) ((float) this->size * 1.5) + 1;
        this->items = realloc(this->items, this->allocsize * sizeof(*this->items));
    }
    this->items[this->size - 1] = node;
}
