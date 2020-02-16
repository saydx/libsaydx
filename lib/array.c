/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <stdlib.h>
#include <string.h>
#include "array.h"

array_t * array_create(const line_t *line)
{
    array_t *this;
    char *separator, *datatype;
    int typenamelen;
    int ii;

    if (line->content[0] != '@') {
        printf("ERROR, invalid data line must start with '@'");
        return NULL;
    }

    this = MALLOC_OR_DIE(sizeof(*this));
    this->nrefs = 1;

    datatype = line->content + 1;
    separator = strchr(datatype, (int) '|');
    if (separator) {
        typenamelen = separator - line->content - 1;
    } else {
        typenamelen = line->length - 1;
    }
    this->rank = 0;
    if (separator) {
        this->rank++;
        for (ii = 0; ii < line->length - typenamelen - 2; ++ii) {
            if (*(separator + ii + 1) == ',') {
                this->rank++;
            }
        }
        this->shape = MALLOC_OR_DIE(this->rank * sizeof(*this->shape));
        this->shape[0] = atoi(separator + 1);
        char *pchar = separator + 1;
        for (int irank = 1; irank < this->rank; ++irank) {
            while (*pchar != ',') pchar++;
            this->shape[irank] = atoi(pchar + 1);
        }
    }
    this->typename = (char *) MALLOC_OR_DIE((typenamelen + 1) * sizeof(char));
    memcpy(this->typename, line->content + 1, typenamelen * sizeof(char));
    this->typename[typenamelen] = '\0';
    this->rawdata = NULL;
    return this;
}


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



void _get_values(array_t *this, int *rank, int **shape, void **data, int compat) {
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
    int compat = strcmp(this->typename, "i8");
    _get_values(this, rank, shape, dataptr, compat);
}
