/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "treewalker.h"
#include "eventprinter.h"
#include "treebuilder.h"

void treewalker_init(treewalker_t *this, treewalkerinp_t *input)
{
    this->eventhandler = input->eventhandler;
}


void treewalker_final(treewalker_t *this)
{
}


void treewalker_walk(treewalker_t *this, node_t *root, int includeroot)
{
    if (includeroot) {
        if (root->children) {
            this->eventhandler.open_container_node(
                this->eventhandler.handler, root->name, root->attributes);
        } else {
            this->eventhandler.open_data_node(
                this->eventhandler.handler, root->name, root->attributes);
            this->eventhandler.receive_array(this->eventhandler.handler, root->array);
            this->eventhandler.close_data_node(this->eventhandler.handler);
        }
    }
    if (root->children) {
        for (int ichild = 0; ichild < root->children->size; ++ichild) {
            treewalker_walk(this, root->children->items[ichild], 1);
        }
    }
    if (includeroot && root->children) {
        this->eventhandler.close_container_node(this->eventhandler.handler);
    }
}
