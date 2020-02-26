/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <stdlib.h>
#include <string.h>
#include "saydx.h"
#include "query.h"
#include "commontypes.h"
#include "node.h"


query_t * query_create()
{
    query_t *query;

    query = MALLOC_OR_DIE(sizeof(*query));
    return query;
}


void query_destroy(query_t *query)
{
    if (query) {
        free(query);
    }
}


void _get_child(query_t *query, node_t *node, const char *name, node_t **child)
{
    *child = NULL;
    if (!node) {
        return;
    }
    if (!strcmp(name, ".")) {
        *child = node_reference(node);
        return;
    }
    node_t **items = node->children->items;
    node_t *item;
    for (int ichild = 0; ichild < node->children->size; ++ichild) {
        item = items[ichild];
        if (!strcmp(name, item->name)) {
            *child = node_reference(item);
            return;
        }
    }
}


error_t * query_get_child(query_t *query, node_t *node, const char *name, bool required,
                          node_t **child)
{
    _get_child(query, node, name, child);
    if (!*child && required) {
        RETURN_WITH_ERROR("Could not found child '%s' of node '%s'", name, node_get_name(node));
    }
    RETURN_WITHOUT_ERROR();
}


error_t * query_get_child_data(query_t *query, node_t *node, const char *name, node_t **child,
                               array_t **data)
{
    PROPAGATE_ERROR(query_get_child(query, node, name, true, child));
    *data = array_reference((*child)->array);
    RETURN_WITHOUT_ERROR();
}


error_t * query_get_child_data_i4(query_t *query, node_t *node, const char *name, node_t **child,
                                  int *data)
{
    array_t *array;
    PROPAGATE_ERROR(query_get_child_data(query, node, name, child, &array));

    int rank, *shape;
    int *dataptr;
    array_as_i4(array, &rank, &shape, &dataptr);
    if (!dataptr) {
        node_dereference(*child);
        RETURN_WITH_ERROR("Data in node '%s' could not converted to type '%s'", node_get_name(node),
                          "i4");
    }
    if (rank != 0) {
        node_dereference(*child);
        RETURN_WITH_ERROR("Data in node '%s' has incompatible rank (expected '%d', got '%d')",
                          node_get_name(node), 0, rank);
    }
    *data = *dataptr;
    array_dereference(array);
    RETURN_WITHOUT_ERROR();
}


error_t * query_get_child_data_i4p(query_t *query, node_t *node, const char *name, node_t **child,
                                   int *rank, int **shape, int **data)
{
    array_t *array;
    PROPAGATE_ERROR(query_get_child_data(query, node, name, child, &array));

    array_as_i4(array, rank, shape, data);
    if (!data) {
        node_dereference(*child);
        RETURN_WITH_ERROR("Data in node '%s' could not converted to type '%s'", node_get_name(node),
                          "i4");
    }
    array_dereference(array);
    RETURN_WITHOUT_ERROR();
}
