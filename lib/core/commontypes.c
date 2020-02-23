/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <stdlib.h>
#include <string.h>

#include "commontypes.h"


//
// traceback_t
//

traceback_t * traceback_create(const char *file, int line)
{
    traceback_t *trb = malloc(sizeof(*trb));
    if (!trb)
        return NULL;
    trb->line = line;
    trb->file = malloc(sizeof(*trb->file) * (strlen(file) + 1));
    if (!trb->file) {
        free(trb);
        return NULL;
    }
    strcpy(trb->file, file);
    trb->next = NULL;
    return trb;
}


traceback_t *traceback_add(traceback_t *trb, const char *file, int line)
{
    trb->next = traceback_create(file, line);
    return trb->next;
}


void traceback_write(traceback_t *trb, FILE *file)
{
    while (trb) {
        fprintf(file, "%s:%d\n", trb->file, trb->line);
        trb = trb->next;
    }
}


void traceback_destroy(traceback_t *trb)
{
    while (trb) {
        free(trb->file);
        traceback_t *oldtrb = trb;
        trb = trb->next;
        free(oldtrb);
    }
}


//
// error_t
//

error_t * error_create(const char *message, const char *file, int line)
{
    error_t *err = malloc(sizeof(*err));
    if (!err) {
        fprintf(stderr, "FATAL ERROR: Error could not be created\n");
        exit(-1);
    }
    err->traceback = traceback_create(file, line);
    if (!err->traceback) {
        fprintf(stderr, "FATAL ERROR: Error traceback could not be created\n");
        exit(-1);
    }
    err->lasttrace = err->traceback;
    err->message = malloc((strlen(message) + 1) * sizeof(*err->message));
    if (!err->message) {
        fprintf(stderr, "FATAL ERROR: Error message could not be created\n");
        exit(-1);
    }
    strcpy(err->message, message);
    return err;
}


void error_add_traceback(error_t *err, const char *file, int line)
{
    err->lasttrace = traceback_add(err->lasttrace, file, line);
    if (!err->lasttrace) {
        fprintf(stderr, "FATAL ERROR: Traceback could not be added to error\n");
        exit(-1);
    }
}


void error_write(error_t *err, FILE *file)
{
    if (!file) {
        file = stderr;
    }
    fprintf(file, "Error: %s\n", err->message);
    fprintf(file, "Traceback:\n");
    traceback_write(err->traceback, file);
}


void error_destroy(error_t *err)
{
    traceback_destroy(err->traceback);
    free(err->message);
    free(err);
}


void *malloc_or_die(size_t allocsize, const char *file, int line)
{
    void *target = malloc(allocsize);
    if (!target) {
        fprintf(stderr, "Allocation failed (%s:%d, size:%ld)\n", file, line, allocsize);
        exit(-1);
    }
    return target;
}


void *realloc_or_die(void *ptr, size_t allocsize, const char *file, int line)
{
    void *target = realloc(ptr, allocsize);
    if (!target) {
        fprintf(stderr, "Re-allocation failed (%s:%d, size:%ld)\n", file, line, allocsize);
        exit(-1);
    }
    return target;
}


//
// line_t
//

void line_init(line_t *this, size_t initsize)
{
    this->size = initsize;
    this->length = 0;
    this->content = MALLOC_OR_DIE(initsize * sizeof(*this->content));
    this->content[0] = '\0';
}


void line_final(line_t *this)
{
    free(this->content);
}


void line_minimize_storage(line_t *this)
{
    if (this->size > this->length + 1) {
        this->size = this->length + 1;
        this->content = REALLOC_OR_DIE(this->content, this->size * sizeof(*this->content));
    }
}


//
// linereader_t
//

error_t * linereader_init(linereader_t *this, const char *fname, size_t chunksize)
{
    this->chunksize = chunksize;
    this->file = fopen(fname, "r");
    if (!this->file) {
        RETURN_WITH_ERROR("Unable to open file '%s'", fname);
    }
    RETURN_WITHOUT_ERROR();
}


void linereader_final(linereader_t *this)
{
    fclose(this->file);
}


void linereader_read_line(linereader_t *this, line_t *line)
{
    char buffer[this->chunksize];
    size_t bufferlen;
    char *result;

    line_init(line, this->chunksize);
    do {
        result = fgets(buffer, this->chunksize, this->file);
        if (!result) {
            return;
        }
        bufferlen = strlen(buffer);
        line->length += bufferlen;
        if (line->length + 1 > line->size) {
            line->size = (int) ((float) line->length * 1.5) + 1;
            line->content = REALLOC_OR_DIE(line->content, line->size * sizeof(*line->content));
        }
        strcat(line->content, buffer);
    } while (bufferlen + 1 == this->chunksize && buffer[this->chunksize - 2] != '\n');
    line->length--;
    line->content[line->length] = '\0';
    line_minimize_storage(line);
}


//
// attribute_t
//

void attribute_final(attribute_t *this)
{
    free(this->name);
    free(this->value);
}


//
// attributes_t
//

attributes_t * attributes_create(size_t initsize)
{
    attributes_t *this;

    this = MALLOC_OR_DIE(sizeof(*this));
    this->nrefs = 1;
    this->size = initsize;
    this->nattributes = 0;
    this->attributes = MALLOC_OR_DIE(initsize * sizeof(*this->attributes));
    return this;
}


void attributes_append(attributes_t *this, attribute_t *newattrib)
{
    this->nattributes++;
    if (this->nattributes > this->size) {
        this->attributes = REALLOC_OR_DIE(this->attributes, (int) ((float) this->size * 1.5) + 1);
    }
    this->attributes[this->nattributes - 1] = *newattrib;
}


void attributes_set(attributes_t *this, attribute_t *newattrib)
{
    for (int iattrib = 0; iattrib < this->nattributes; iattrib++) {
        if (!strcmp(this->attributes[iattrib].name, newattrib->name)) {
            attribute_final(this->attributes + iattrib);
            this->attributes[iattrib] = *newattrib;
            return;
        }
    }
    attributes_append(this, newattrib);
}


void attributes_destroy(attributes_t *this)
{
    int iattrib;

    this->nrefs--;
    if (this->nrefs) {
        return;
    }
    for (iattrib = 0; iattrib < this->nattributes; ++iattrib) {
        attribute_final(this->attributes + iattrib);
    }
    free(this->attributes);
    free(this);
}


attributes_t * attributes_reference(attributes_t *this)
{
    this->nrefs++;
    return this;
}


//
// string_t
//

void string_final(string_t *this)
{
    free(this->content);
}



//
// blob_t
//

void blob_init(blob_t *this, size_t chunksize)
{
    this->chunksize = chunksize;
    this->data = MALLOC_OR_DIE(chunksize * sizeof(*this->data));
    this->_allocsize = chunksize;
    this->size = 0;
}


void blob_final(blob_t *this)
{
    if (this->data) {
        printf("B FINAL2\n");
        free(this->data);
    }
}


void blob_add_bytes(blob_t *this, const void *data, size_t datasize)
{
    int newsize = this->size + datasize;
    if (newsize > this->_allocsize) {
        int newallocsize = ((newsize / this->chunksize) + 1) * this->chunksize;
        this->data = REALLOC_OR_DIE(this->data, newallocsize);
    }
    memcpy(this->data + this->size, data, datasize);
    this->size = newsize;
}


void blob_transfer_dataptr(blob_t *this, void **dataptr)
{
    *dataptr = this->data;
    this->data = NULL;
}
