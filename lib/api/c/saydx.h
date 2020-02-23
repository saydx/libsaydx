/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#ifndef SAYDX_H
#define SAYDX_H

#include <stdio.h>
#include <stdbool.h>


//
// array_t
//

struct _array_t;
typedef struct _array_t array_t;

void array_destroy(array_t *this);


//
// node_t
//

struct _node_t;
typedef struct _node_t node_t;

char *node_get_name(node_t *this);
void node_destroy(node_t *this);

//
// attributes_t
//
struct _attributes_t;
typedef struct _attributes_t attributes_t;


//
// error_t
//

struct _error_t;
typedef struct _error_t error_t;

void error_write(error_t *error, FILE *file);


//
// query_t
//

struct _query_t;
typedef struct _query_t query_t;

query_t * query_create();
void query_destroy(query_t *query);

error_t * query_get_child(query_t *query, node_t *node, const char *name, bool required,
                          node_t **child);
error_t * query_get_child_data(query_t *query, node_t *node, const char *name,
                               node_t **child, array_t **data);
error_t * query_get_child_data_i4(query_t *query, node_t *node, const char *name, node_t **child,
                                  int *data);
error_t * query_get_child_data_i4p(query_t *query, node_t *node, const char *name, node_t **child,
                                   int *rank, int **shape, int **data);


//! Reads an msd file into a saydx-tree.
//!
//! \param[in] name  Name of the file to read.
//! \param[out] root  Root node of the parsed tree.
//! \return Error, if parsing failed for some reasons.
//!
error_t * read_msd_file(const char *name, node_t **root);


//
// eventhandler_t
//
struct _eventhandler_t;
typedef struct _eventhandler_t eventhandler_t;
void eventhandler_destroy(eventhandler_t *eventhandler);


//
// eventprinter_t
//
struct _eventprinter_t;
typedef struct _eventprinter_t eventprinter_t;
eventprinter_t * eventprinter_create();
eventhandler_t * eventprinter_cast_to_eventhandler(eventprinter_t *);


//
// treebuilder_t
//
struct _treebuilder_t;
typedef struct _treebuilder_t treebuilder_t;
treebuilder_t * treebuilder_create();
eventhandler_t * treebuilder_cast_to_eventhandler(treebuilder_t *);
void treebuilder_transfer_tree(treebuilder_t *this, node_t **target);


//
// msdparser_input_t
//
typedef struct {
    eventhandler_t *eventhandler;
} msdparser_input_t;


//
// msdparser_t
//
struct _msd_parser_t;
typedef struct _msdparser_t msdparser_t;

msdparser_t  * msdparser_create(msdparser_input_t *msdparser_input);
error_t * msdparser_parse_file(msdparser_t *this, const char *fname);
void msdparser_destroy(msdparser_t *msdparser);


//
// serializer_t
//
struct _serializer_t;
typedef struct _serializer_t serializer_t;
void serializer_destroy(serializer_t *serializer);


//
// txtserializer_t
//
struct _txtserializer_t;
typedef struct _txtserializer_t txtserializer_t;

txtserializer_t *txtserializer_create();
serializer_t * txtserializer_cast_to_serializer(txtserializer_t *txtserializer);


//
// deserializer_t
//
struct _deserializer_t;
typedef struct _deserializer_t deserializer_t;
void deserializer_destroy(deserializer_t *deserializer);


//
// txtdeserializer_t
//
struct _txtdeserializer_t;
typedef struct _txtdeserializer_t txtdeserializer_t;

txtdeserializer_t *txtdeserializer_create();
deserializer_t * txtdeserializer_cast_to_deserializer(txtdeserializer_t *txtdeserializer);


// Serialize data into binary data.
error_t * serialize(node_t *root, serializer_t *serializer, void **serialdata, size_t *serialsize);

// Deserialize binary data into tree.
error_t * deserialize(const void *serialdata, size_t serialsize, deserializer_t *deserializer,
                      node_t **root);


#endif
