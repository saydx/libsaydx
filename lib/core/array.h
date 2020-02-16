/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef SAYDX_ARRAY_H
#define SAYDX_ARRAY_H

#include "saydx.h"
#include "commontypes.h"


//
// array_t
//

typedef struct _array_t {
    int nrefs;
    char *typename;
    int rank;
    int *shape;
    void *rawdata;
} _array_t;


array_t * array_create(const line_t *line);
array_t * array_reference(array_t *this);
// array_destroy() defined in library header file
int array_size(array_t *this);

void array_as_i4(array_t *this, int *rank, int **shape, int **dataptr);

#endif
