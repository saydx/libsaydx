/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef SAYDX_COMMONTYPES_H
#define SAYDX_COMMONTYPES_H

#include <stdio.h>
#include "saydx.h"

void *malloc_or_die(size_t allocsize, char *file, int line);
void *realloc_or_die(void *ptr, size_t allocsize, char *file, int line);


#define MALLOC_OR_DIE(ALLOCSIZE) malloc_or_die((ALLOCSIZE), __FILE__, __LINE__)
#define REALLOC_OR_DIE(PTR, ALLOCSIZE) realloc_or_die((PTR), (ALLOCSIZE), __FILE__, __LINE__)


#define RETURN_WITHOUT_ERROR() {return NULL;}


#define RETURN_WITH_ERROR(...) {\
    {\
        size_t msglen = snprintf(NULL, 0, __VA_ARGS__) + 1;\
        char *msg = MALLOC_OR_DIE(sizeof(*msg) * msglen);\
        sprintf(msg, __VA_ARGS__);\
        error_t *err = error_create(msg, __FILE__, __LINE__);\
        return err;\
    }\
}


#define PROPAGATE_ERROR(ERR) {\
    {\
        error_t *err = (ERR);\
        if (err) {\
            error_add_traceback(err, __FILE__, __LINE__);\
            return err;\
        }\
    }\
}



//
// traceback_t
//

typedef struct traceback_s {
    char *file;
    int line;
    struct traceback_s *next;
} traceback_t;


traceback_t * traceback_create(const char *file, int line);
traceback_t * traceback_add(traceback_t *traceback, const char *file, int line);
void traceback_write(traceback_t *traceback, FILE *file);
void traceback_destroy(traceback_t *traceback);


typedef struct _error_t {
    traceback_t *traceback;
    traceback_t *lasttrace;
    char *message;
} _error_t;

error_t * error_create(const char *message, const char *file, int line);
void error_add_traceback(error_t *error, const char *file, int line);
// error_write() defined in library header file
void error_destroy(error_t *error);


//
// line_t
//

//! Contains a line of variable length
//!
typedef struct {

    //! Size of the allocated space (must be >= than line length)
    size_t size;

    //! Lenght of the line
    size_t length;

    //* Content of the line.
    char *content;

} line_t;


//! Initializes the line
//!
//! \param[inout] this  Allocated line instance
//! \param[in] initsize  Initial line size.
//!
void line_init(line_t * this, size_t initsize);


//! Finalizes line.
//!
//! \param[inout] this  Instance.
//!
void line_final(line_t *this);


//! Minimalizes line storage.
//!
//! \param[inout] this  Instance.
//!
void line_minimize_storage(line_t *this);


//
// linereader_t
//

//!  Contains a line reader
typedef struct {
    size_t chunksize;
    FILE *file;
} linereader_t;

//! Initializes a line reader
error_t * linereader_init(linereader_t *this, const char *fname, size_t chunksize);
void linereader_final(linereader_t *this);
void linereader_read_line(linereader_t *this, line_t *line);


//
// attribute_t
//

typedef struct  {
    char *name;
    char *value;
} attribute_t;


// attribute_t defined in libary header file

void attribute_init(attribute_t *this, const line_t *line);
void attribute_final(attribute_t *this);


//
// attributes_t
//

//
// attributes_t
//

typedef struct {
    int nrefs;
    int nattributes;
    int size;
    attribute_t *attributes;
} attributes_t;

attributes_t * attributes_create(size_t initsize);
attributes_t * attributes_reference(attributes_t *this);
void attributes_set(attributes_t *this, attribute_t *attrib);
void attributes_append(attributes_t *this, attribute_t *attrib);
void attributes_destroy(attributes_t *this);


//
//  string_t
//

typedef struct {
    char *content;
    size_t length;
} string_t;

void string_final(string_t *);


typedef enum dataType {
    INT4_DATA,
    INT8_DATA,
    REAL4_DATA,
    REAL8_DATA,
    COMPLEX4_DATA,
    COMPLEX8_DATA,
    LOGICAL_DATA,
    CHARACTER_DATA,
    STRING_DATA
} dataType;

#endif
