/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef SAYDX_QUERY_H
#define SAYDX_QUERY_H

#include <stdbool.h>
#include "node.h"
#include "array.h"

typedef struct {
} query_t;


void query_get_child(query_t *query, node_t *node, const char *name, bool required, node_t **child);
void query_get_child_child(query_t *query, node_t *node, const char *name, node_t **child,
                           node_t **child_child);
void query_get_child_data(query_t *query, node_t *node, const char *name,
                          node_t **child, array_t **data);
void query_get_child_data_i4(query_t *query, node_t *node, const char *name, node_t **child,
                             int *data);
//void query_get_child_data_i4_ptr(query_t *query, node_t *node, const char *name, node_t **child,
//                                 int *rank, int **shape, int **data);
//

// void query_get_node_data_r8();
// void query_get_node_data_r8_r1();
// void query_get_node_data_r8_r2();
// void query_get_node_data_l();
// void query_get_node_data_s();

#endif
