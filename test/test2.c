/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <stdio.h>
#include <stdlib.h>
#include "saydx.h"


int main()
{
    node_t *root;
    error_t *err;

    err = read_msd_file("test1.msd", &root);
    if (err) {
        fprintf(stderr, "ERROR OCCURED\n");
        error_write(err, stderr);
        exit(1);
    }

    node_t *child;
    query_t *query = query_create();

    query_get_child(query, root, "slakodef_version", false, &child);
    printf("Child '%s' of node '%s' ", "slakodef_version", node_get_name(root));
    printf(child ? "FOUND\n" : "NOT FOUND\n");

    query_get_child(query, root, "non_existent_tag", false, &child);
    printf("Child '%s' of node '%s' ", "non_existent_tag", node_get_name(root));
    printf(child ? "FOUND\n" : "NOT FOUND\n");

    array_t *data;
    query_get_child_data(query, root, "slakodef_version", &child, &data);
    printf("Data child '%s' found\n", "slakodef_version");
    array_destroy(data);

    query_get_child_data(query, root, "slakodef_version", &child, &data);
    printf("Data child '%s' found (2)\n", "slakodef_version");
    array_destroy(data);

    int skdver;
    query_get_child_data_i4(query, root, "slakodef_version", &child, &skdver);
    printf("Integer data of '%s': %d\n", "slakodef_version", skdver);

    query_destroy(query);

    node_final(root);
    free(root);

    return 0;

}
