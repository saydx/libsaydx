/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */
#ifndef SAYDX_TREEPACKER_H
#define SAYDX_TREEPACKER_H

#include "eventhandler.h"
#include "commontypes.h"
#include "serializer.h"

void eventhandler_init_treepacker(eventhandler_t *eventhandler, blob_t *blob,
                                  serializer_t *serializer);

#endif
