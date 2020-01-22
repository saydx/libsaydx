/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef SAYDX_TREEBUILDER_H
#define SAYDX_TREEBUILDER_H

#include <stdlib.h>
#include "node.h"


typedef struct {
    node_t *root, *curnode;
    int level;
} treebuilder_t;


error_t * treebuilder_start_processing(void *handler, const char *filename);
error_t * treebuilder_end_processing(void *handler, const char *filename);
error_t * treebuilder_open_container_node(void *handler, const char *tagname,
                                          attributes_t *attribs);
error_t * treebuilder_close_container_node(void *handler, const char *tagname);
error_t * treebuilder_open_data_node(void *handler, const char *tagname, attributes_t *attribs);
error_t * treebuilder_close_data_node(void *handler);
error_t * treebuilder_receive_array(void *handler, array_t *array);

//void node_list_init();
//void node_list_final();
//void node_list_add_node();

void treebuilder_init(treebuilder_t *this);
void treebuilder_final(treebuilder_t *this);


#endif
