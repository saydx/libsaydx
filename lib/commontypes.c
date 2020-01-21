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


void *malloc_or_die(size_t allocsize, char *file, int line)
{
    void *target = malloc(allocsize);
    if (!target) {
        fprintf(stderr, "Allocation failed (%s:%d, size:%ld)\n", file, line, allocsize);
        exit(-1);
    }
    return target;
}


void *realloc_or_die(void *ptr, size_t allocsize, char *file, int line)
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

void attribute_init(attribute_t *this, const line_t *line)
{
    if (line->content[0] != '&') {
        printf("ERROR: Attribute line must start with '&'!\n");
        exit(1);
    }
    char *separator = strchr(line->content + 1, (int) ':');
    if (!separator) {
        printf("ERROR: Missing attribute separator!\n");
        exit(1);
    }
    int wordlen = separator - line->content - 1;
    this->name = MALLOC_OR_DIE((wordlen + 1) * sizeof(*this->name));
    memcpy(this->name, line->content + 1, wordlen * sizeof(*this->name));
    this->name[wordlen] = '\0';
    wordlen = line->length - wordlen - 2;
    this->value = MALLOC_OR_DIE((wordlen + 1) * sizeof(*this->value));
    memcpy(this->value, separator + 1, wordlen * sizeof(*this->value));
    this->value[wordlen] = '\0';
}


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
// array_t
//

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


//
// string_t
//

void string_final(string_t *this)
{
    free(this->content);
}
