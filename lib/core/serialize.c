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


error_t * serialize(node_t *root, serializer_t *serializer, void **serialdata, size_t *serialsize)
{
    blob_t blob;
    treewalkerinp_t treewalkerinp;
    treewalker_t treewalker;

    blob_init(&blob, BLOB_CHUNK_SIZE);
    treepacker_t *treepacker = treepacker_create(&blob, serializer);
    eventhandler_t *eventhandler = treepacker_cast_to_eventhandler(treepacker);

    treewalkerinp.eventhandler = eventhandler;
    treewalker_init(&treewalker, &treewalkerinp);
    treewalker_walk(&treewalker, root, 0);
    treewalker_final(&treewalker);
    blob_transfer_dataptr(&blob, serialdata);
    *serialsize = blob.size;
    blob_final(&blob);
    eventhandler_destroy(eventhandler);
    RETURN_WITHOUT_ERROR();
}


error_t * deserialize(const void *serialdata, size_t serialsize, deserializer_t *deserializer,
                      node_t **root)
{

    treeunpacker_t treeunpacker;
    treebuilder_t *treebuilder = treebuilder_create();
    eventhandler_t *eventhandler = treebuilder_cast_to_eventhandler(treebuilder);

    treeunpacker_init(&treeunpacker, eventhandler, deserializer);
    treeunpacker_unpack(&treeunpacker, serialdata, serialsize);
    treebuilder_transfer_tree(treebuilder, root);
    eventhandler_destroy(eventhandler);
    RETURN_WITHOUT_ERROR();
}
