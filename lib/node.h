/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef SAYDX_NODE_H
#define SAYDX_NODE_H

#include "commontypes.h"

struct node_t;

typedef struct {
    int size;
    int allocsize;
    struct node_t **items;
} node_list_t;


typedef struct node_t {
    char *name;
    attributes_t *attributes;
    array_t *array;
    struct node_t *parent;
    node_list_t *children;
} node_t;

void node_init(node_t *this, const char *name, int nodetype, attributes_t *attributes);
void node_final(node_t *this);
void node_append_child(node_t *this, node_t *child);

void node_list_init(node_list_t *this, int initsize);
void node_list_final(node_list_t *this);
void node_list_append(node_list_t *this, node_t *node);


#endif
