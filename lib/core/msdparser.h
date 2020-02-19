/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef SAYDX_MSDPARSER_H
#define SAYDX_MSDPARSER_H

#include <stdio.h>

#include "commontypes.h"
#include "array.h"
#include "eventhandler.h"

//
// msd_header_line_t
//

typedef struct {
    int headertype;
    int nattributes;
    char *tagname;
} msdheaderline_t;

error_t * msdheaderline_init(msdheaderline_t *this, const line_t *line);
void msdheaderline_final(msdheaderline_t *this);


int read_i4_data(void *rawdata, const line_t *, int, int);
int read_r8_data(void *rawdata, const line_t *, int, int);
int read_logical_data(void *, const line_t *, int, int);
int read_string_data(void *rawdata, const line_t *line, int items, int nitems);

typedef int (*data_reader_t)(void *, const line_t *, int, int);


typedef struct {
    eventhandler_t eventhandler;
} msdparser_input_t;


typedef struct {
    FILE *file;
    eventhandler_t eventhandler;
} msdparser_t;


void msdparser_init(msdparser_t *this, msdparser_input_t *input);
void msdparser_final(msdparser_t *this);
error_t * msdparser_parse_file(msdparser_t *this, const char *fname);
error_t * msdparser_parse_open_file(msdparser_t *this, linereader_t *linereader);
error_t * msdparser_read_attributes(
    msdparser_t *this, linereader_t *linereader, int nattribs, attributes_t *attributes);
error_t * msdparser_read_array(msdparser_t *this, linereader_t *linereader, array_t **array);
error_t * msdparser_read_raw_data(array_t *array, linereader_t *linereader, int nitems);

#endif
