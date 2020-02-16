/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include "commontypes.h"
#include "array.h"
#include "msdparser.h"
#include "treebuilder.h"
#include "query.h"

int main()
{
    msdparser_input_t msdinp;
    msdparser_t msdparser;
    treebuilder_t treebuilder;
    query_t query;
    error_t *err;

    treebuilder_init(&treebuilder);
    msdinp.eventhandler.handler = &treebuilder;
    msdinp.eventhandler.start_processing = &treebuilder_start_processing;
    msdinp.eventhandler.end_processing = &treebuilder_end_processing;
    msdinp.eventhandler.open_container_node = &treebuilder_open_container_node;
    msdinp.eventhandler.open_data_node = &treebuilder_open_data_node;
    msdinp.eventhandler.close_container_node = &treebuilder_close_container_node;
    msdinp.eventhandler.close_data_node = &treebuilder_close_data_node;
    msdinp.eventhandler.receive_array = &treebuilder_receive_array;
    msdparser_init(&msdparser, &msdinp);
    err = msdparser_parse_file(&msdparser, "test1.msd");
    if (err) {
        fprintf(stderr, "ERROR OCCURED\n");
        error_write(err, stderr);
        exit(1);
    }
    msdparser_final(&msdparser);

    printf("PARSING DONE, QUERYING TREE\n");

    node_t *root = treebuilder.root;
    node_t *child;

    query_get_child(&query, root, "slakodef_version", false, &child);
    printf("Child '%s' of node '%s' ", "slakodef_version", root->name);
    printf(child ? "FOUND\n" : "NOT FOUND\n");
    
    query_get_child(&query, root, "non_existent_tag", false, &child);
    printf("Child '%s' of node '%s' ", "non_existent_tag", root->name);
    printf(child ? "FOUND\n" : "NOT FOUND\n");

    array_t *data;
    query_get_child_data(&query, root, "slakodef_version", &child, &data);
    printf("Data child '%s' found\n", "slakodef_version");
    array_destroy(data);

    query_get_child_data(&query, root, "slakodef_version", &child, &data);
    printf("Data child '%s' found (2)\n", "slakodef_version");
    array_destroy(data);

    int skdver;
    query_get_child_data_i4(&query, root, "slakodef_version", &child, &skdver);
    printf("Integer data of '%s': %d\n", "slakodef_version", skdver);

    treebuilder_final(&treebuilder);
    return 0;

}
