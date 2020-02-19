/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <stdlib.h>
#include <string.h>
#include "saydx.h"
#include "commontypes.h"
#include "treewalker.h"
#include "eventhandler.h"
#include "treepacker.h"


#define BLOB_CHUNK_SIZE 4096


error_t * serialize(node_t *root, const char *mode, void **serialdata, int *serialsize)
{
    blob_t blob;
    eventhandler_t eventhandler;
    treewalkerinp_t treewalkerinp;
    treewalker_t treewalker;

    blob_init(&blob, BLOB_CHUNK_SIZE);
    eventhandler_init_treepacker(&eventhandler, &blob);
    treewalkerinp.eventhandler = eventhandler;
    treewalker_init(&treewalker, &treewalkerinp);
    treewalker_walk(&treewalker, root, 0);
    treewalker_final(&treewalker);
    blob_transfer_dataptr(&blob, serialdata);
    *serialsize = blob.size;
    blob_final(&blob);
    RETURN_WITHOUT_ERROR();
}


error_t * deserialize(void *serialdata, int serialsize, const char *mode, node_t **root)
{
    printf("DESERIALIZATION NOT WORKING YET\n");
    exit(1);
}




