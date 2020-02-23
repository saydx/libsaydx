/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef SAYDX_TREEWALKER_H
#define SAYDX_TREEWALKER_H

#include "msdparser.h"
#include "treebuilder.h"

typedef struct {
    eventhandler_t *eventhandler;
} treewalkerinp_t;

typedef struct {
    eventhandler_t *eventhandler;
} treewalker_t;

void treewalker_init(treewalker_t *this, treewalkerinp_t *input);
void treewalker_final(treewalker_t *this);
void treewalker_walk(treewalker_t *this, node_t *root, int includeroot);

#endif
