/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <stdlib.h>
#include <string.h>
#include "query.h"


void _get_child(query_t *query, node_t *node, const char *name, node_t **child)
{
    *child = NULL;
    if (!node) {
        return;
    }
    if (!strcmp(name, ".")) {
        *child = node;
        return;
    }
    node_t **items = node->children->items;
    node_t *item;
    for (int ichild = 0; ichild < node->children->size; ++ichild) {
        item = items[ichild];
        if (!strcmp(name, item->name)) {
            *child = item;
            return;
        }
    }
}


void query_get_child(query_t *query, node_t *node, const char *name, bool required, node_t **child)
{
    _get_child(query, node, name, child);
    if (!*child && required) {
        fprintf(stderr, "Could not found child '%s'\n", name);
        exit(-1);
    }
}


void query_get_child_data(query_t *query, node_t *node, const char *name, node_t **child,
                          array_t **data)
{
    query_get_child(query, node, name, true, child);
    if (!(*child)->array) {
        fprintf(stderr, "Child '%s has no data'\n", name);
        exit(-1);
    }
    *data = array_reference((*child)->array);
}


void query_get_child_data_i4(query_t *query, node_t *node, const char *name, node_t **child,
                             int *data)
{
    array_t *array;
    query_get_child_data(query, node, name, child, &array);

    int rank, *shape;
    int *dataptr;
    array_as_i4(array, &rank, &shape, &dataptr);
    *data = *dataptr;
    array_destroy(array);
}
