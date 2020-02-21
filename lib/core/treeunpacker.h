/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */
#ifndef SAYDX_TREEUNPACKER_H
#define SAYDX_TREEUNPACKER_H

#include "eventhandler.h"
#include "deserializer.h"
#include "commontypes.h"

typedef struct {
    deserializer_t *deserializer;
    eventhandler_t *eventhandler;
} treeunpacker_t;


void treeunpacker_init(treeunpacker_t *treeunpacker, eventhandler_t *eventhandler,
                       deserializer_t *deserializer);

error_t *treeunpacker_unpack(treeunpacker_t *this, const void *blob, size_t blobsize);

#endif
