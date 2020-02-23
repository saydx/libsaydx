/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <stdio.h>
#include <stdlib.h>
#include "saydx.h"

void check_error(error_t *error)
{
    if (error) {
        fprintf(stderr, "ERROR OCCURED\n");
        error_write(error, stderr);
        exit(1);
    }
}


int main()
{
    treebuilder_t *treebuilder = treebuilder_create();
    eventhandler_t *eventhandler = treebuilder_cast_to_eventhandler(treebuilder);

    msdparser_input_t msdinp;
    msdinp.eventhandler = eventhandler;
    msdparser_t *msdparser = msdparser_create(&msdinp);
    error_t *err = msdparser_parse_file(msdparser, "test2.msd");
    check_error(err);
    msdparser_destroy(msdparser);

    node_t *root;
    treebuilder_transfer_tree(treebuilder, &root);
    eventhandler_destroy(eventhandler);

    query_t *query = query_create();
    node_t *child;

    err = query_get_child(query, root, "slakodef_version", false, &child);
    check_error(err);
    printf("Child '%s' of node '%s' ", "slakodef_version", node_get_name(root));
    printf(child ? "FOUND\n" : "NOT FOUND\n");

    err = query_get_child(query, root, "non_existent_tag", false, &child);
    check_error(err);
    printf("Child '%s' of node '%s' ", "non_existent_tag", node_get_name(root));
    printf(child ? "FOUND\n" : "NOT FOUND\n");

    array_t *data;
    err = query_get_child_data(query, root, "slakodef_version", &child, &data);
    check_error(err);
    printf("Data child '%s' found\n", "slakodef_version");
    array_destroy(data);

    err = query_get_child_data(query, root, "slakodef_version", &child, &data);
    check_error(err);
    printf("Data child '%s' found (2)\n", "slakodef_version");
    array_destroy(data);

    int skdver;
    err = query_get_child_data_i4(query, root, "slakodef_version", &child, &skdver);
    check_error(err);
    printf("Integer data of '%s': %d\n", "slakodef_version", skdver);

    err = query_get_child(query, root, "two_center_params", true, &child);
    check_error(err);
    printf("Child '%s': of node '%s' found.\n", node_get_name(child), node_get_name(root));

    node_t *child2;
    int rank, asize;
    int *shape, *i4data;
    err = query_get_child_data_i4p(query, child, "some_integer_array", &child2, &rank, &shape,
                                   &i4data);
    check_error(err);
    printf("Child '%s' contains i4 rank %d array with shape: ", node_get_name(child2), rank);
    asize = 1;
    if (rank > 0) {
        printf("%d", shape[0]);
        asize *= shape[0];
        for (int ii = 1; ii < rank; ++ii) {
            printf(", %d", shape[ii]);
            asize *= shape[ii];
        }
        printf("\n");
    }
    printf("  Data contained: ");
    printf("%d", i4data[0]);
    for (int ii = 1; ii < asize; ++ii) {
        printf(", %d", i4data[ii]);
    }
    printf("\n");

    query_destroy(query);
    node_destroy(root);

    return 0;

}
