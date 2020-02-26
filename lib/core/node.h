/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef SAYDX_NODE_H
#define SAYDX_NODE_H

#include "saydx.h"

#include "commontypes.h"
#include "array.h"


typedef struct {
    int size;
    int allocsize;
    struct _node_t **items;
} node_list_t;


typedef struct _node_t {
    int nrefs;
    char *name;
    attributes_t *attributes;
    array_t *array;
    struct _node_t *parent;
    node_list_t *children;
} _node_t;



//
// node_t and node_list_t defined in library header file
//

// node_create defined in the library header file
// node_destroy() defined library header file

void node_append_child(node_t *this, node_t *child);

node_list_t * node_list_create(int initsize);
void node_list_destroy(node_list_t *this);
void node_list_append(node_list_t *this, node_t *node);


#endif
