/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

//#include "commontypes.h"
//#include "msdparser.h"
//#include "eventprinter.h"
//#include "treewalker.h"
//#include "treebuilder.h"

int main()
{
//    msdparser_input_t msdinp;
//    msdparser_t msdparser;
//    eventprinter_t eventprinter;
//    treebuilder_t treebuilder;
//    treewalkerinp_t treewalkerinp;
//    treewalker_t treewalker;
//    error_t *err;
//
//    treebuilder_init(&treebuilder);
//    msdinp.eventhandler.handler = &treebuilder;
//    msdinp.eventhandler.start_processing = &treebuilder_start_processing;
//    msdinp.eventhandler.end_processing = &treebuilder_end_processing;
//    msdinp.eventhandler.open_container_node = &treebuilder_open_container_node;
//    msdinp.eventhandler.open_data_node = &treebuilder_open_data_node;
//    msdinp.eventhandler.close_container_node = &treebuilder_close_container_node;
//    msdinp.eventhandler.close_data_node = &treebuilder_close_data_node;
//    msdinp.eventhandler.receive_array = &treebuilder_receive_array;
//    msdparser_init(&msdparser, &msdinp);
//    err = msdparser_parse_file(&msdparser, "test1.msd");
//    if (err) {
//        fprintf(stderr, "ERROR OCCURED\n");
//        error_write(err, stderr);
//        exit(1);
//    }
//    msdparser_final(&msdparser);
//
//    printf("PARSING DONE\n");
//    eventprinter_init(&eventprinter);
//    treewalkerinp.eventhandler.handler = &eventprinter;
//    treewalkerinp.eventhandler.start_processing = &eventprinter_start_processing;
//    treewalkerinp.eventhandler.end_processing = &eventprinter_end_processing;
//    treewalkerinp.eventhandler.open_container_node = &eventprinter_open_container_node;
//    treewalkerinp.eventhandler.open_data_node = &eventprinter_open_data_node;
//    treewalkerinp.eventhandler.close_container_node = &eventprinter_close_container_node;
//    treewalkerinp.eventhandler.close_data_node = &eventprinter_close_data_node;
//    treewalkerinp.eventhandler.receive_array = &eventprinter_receive_array;
//    treewalker_init(&treewalker, &treewalkerinp);
//    treewalker_walk(&treewalker, treebuilder.root, 1);
//
//    treewalker_final(&treewalker);
//    treebuilder_final(&treebuilder);
//    eventprinter_final(&eventprinter);
//    return 0;
//
}
