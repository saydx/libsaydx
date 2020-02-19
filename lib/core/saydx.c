#include "saydx.h"

#include "commontypes.h"
#include "msdparser.h"
#include "treebuilder.h"


error_t * read_msd_file(const char *name, node_t **root)
{
    msdparser_input_t msdinp;
    msdparser_t msdparser;
    treebuilder_t treebuilder;

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
    PROPAGATE_ERROR(
        msdparser_parse_file(&msdparser, name));
    msdparser_final(&msdparser);
    treebuilder_transfer_tree(&treebuilder, root);
    treebuilder_final(&treebuilder);
}
