/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef SAYDX_TREEBUILDER_H
#define SAYDX_TREEBUILDER_H

#include <stdlib.h>
#include "node.h"
#include "eventhandler.h"


typedef struct {
    node_t *root, *curnode;
    int level;
} treebuilder_t;


void eventhandler_init_treebuilder(eventhandler_t *eventhandler, treebuilder_t *treebuilder);

void treebuilder_init(treebuilder_t *this);
void treebuilder_final(treebuilder_t *this);
void treebuilder_transfer_tree(treebuilder_t *this, node_t **target);


#endif
