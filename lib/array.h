/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef SAYDX_ARRAY_H
#define SAYDX_ARRAY_H

#include "commontypes.h"


typedef struct {
    int nrefs;
    char *typename;
    int rank;
    int *shape;
    void *rawdata;
} array_t;

array_t * array_create(const line_t *line);
array_t * array_reference(array_t *this);
void array_destroy(array_t *this);
int array_size(array_t *this);

void array_as_i4(array_t *this, int *rank, int **shape, int **dataptr);

#endif
