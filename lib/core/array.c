/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <stdlib.h>
#include <string.h>
#include "array.h"


void array_destroy(array_t *this)
{
    string_t *pstring;
    int iitem;

    this->nrefs--;
    if (this->nrefs) {
        return;
    }
    if (this->rawdata) {
        if (strcmp(this->typename, "s") == 0) {
            for (iitem = 0, pstring = this->rawdata; iitem < array_size(this); ++iitem, ++pstring) {
                string_final(pstring);
            }
        }
        free(this->rawdata);
    }
    free(this->typename);
    if (this->rank > 0) {
        free(this->shape);
    }
    free(this);
}


array_t * array_reference(array_t *this)
{
    this->nrefs++;
    return this;
}


int array_size(array_t *this)
{
    int nitems = 1;
    for (int irank = 0; irank < this->rank; irank++) {
        nitems *= this->shape[irank];
    }
    return nitems;
}


size_t array_byte_size(array_t *this)
{
    int nitems = array_size(this);
    if (strcmp(this->typename, "i4") == 0) {
        return nitems * sizeof(int);
    } else if (strcmp(this->typename, "r8") == 0) {
        return nitems * sizeof(double);
    } else if (strcmp(this->typename, "l") == 0) {
        return nitems * sizeof(int);
    } else if (strcmp(this->typename, "s") == 0) {
        fprintf(stderr, "array_byte_size not working for string type yet!\n");
        exit(-1);
    }
}



void _get_values(array_t *this, int *rank, int **shape, void **data, bool compat) {
    if (compat) {
        *rank = this->rank;
        *shape = this->shape;
        *data = this->rawdata;
    } else {
        *rank = -1;
        *shape = NULL;
        *data = NULL;
    }
}


void array_as_i4(array_t *this, int *rank, int **shape, int **dataptr)
{
    void *rawdataptr;

    bool compat = !strcmp(this->typename, "i4");
    _get_values(this, rank, shape, &rawdataptr, compat);
    *dataptr = rawdataptr;
}
