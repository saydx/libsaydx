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
#include "serializer.h"
#include "txtserializer.h"
#include "deserializer.h"
#include "txtdeserializer.h"
#include "treeunpacker.h"


#define BLOB_CHUNK_SIZE 4096


error_t * serialize(node_t *root, const char *mode, void **serialdata, int *serialsize)
{
    blob_t blob;
    serializer_t serializer;
    eventhandler_t eventhandler;
    treewalkerinp_t treewalkerinp;
    treewalker_t treewalker;

    blob_init(&blob, BLOB_CHUNK_SIZE);
    serializer_init_txtserializer(&serializer);
    eventhandler_init_treepacker(&eventhandler, &blob, &serializer);

    treewalkerinp.eventhandler = eventhandler;
    treewalker_init(&treewalker, &treewalkerinp);
    treewalker_walk(&treewalker, root, 0);
    treewalker_final(&treewalker);
    blob_transfer_dataptr(&blob, serialdata);
    *serialsize = blob.size;
    blob_final(&blob);
    RETURN_WITHOUT_ERROR();
}


error_t * deserialize(const void *serialdata, int serialsize, const char *mode, node_t **root)
{

    eventhandler_t eventhandler;
    deserializer_t deserializer;
    treeunpacker_t treeunpacker;
    treebuilder_t treebuilder;

    treebuilder_init(&treebuilder);
    eventhandler_init_treebuilder(&eventhandler, &treebuilder);
    deserializer_init_txtdeserializer(&deserializer);
    treeunpacker_init(&treeunpacker, &eventhandler, &deserializer);
    treeunpacker_unpack(&treeunpacker, serialdata, serialsize);
    treebuilder_transfer_tree(&treebuilder, root);
    treebuilder_final(&treebuilder);
    RETURN_WITHOUT_ERROR();
}
