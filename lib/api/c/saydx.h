/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef SAYDX_H
#define SAYDX_H

#include <stdio.h>
#include <stdbool.h>


//
// array_t
//

struct _array_t;
typedef struct _array_t array_t;

void array_destroy(array_t *this);


//
// node_t
//

struct _node_t;
typedef struct _node_t node_t;

char *node_get_name(node_t *this);
void node_final(node_t *this);


//
// error_t
//

struct _error_t;
typedef struct _error_t error_t;

void error_write(error_t *error, FILE *file);


//
// query_t
//

struct _query_t;
typedef struct _query_t query_t;

query_t * query_create();
void query_destroy(query_t *query);

error_t * query_get_child(query_t *query, node_t *node, const char *name, bool required,
                          node_t **child);
error_t * query_get_child_data(query_t *query, node_t *node, const char *name,
                               node_t **child, array_t **data);
error_t * query_get_child_data_i4(query_t *query, node_t *node, const char *name, node_t **child,
                                  int *data);
error_t * query_get_child_data_i4p(query_t *query, node_t *node, const char *name, node_t **child,
                                   int *rank, int **shape, int **data);


//! Reads an msd file into a saydx-tree.
//!
//! \param[in] name  Name of the file to read.
//! \param[out] root  Root node of the parsed tree.
//! \return Error, if parsing failed for some reasons.
//!
error_t * read_msd_file(const char *name, node_t **root);

#endif
