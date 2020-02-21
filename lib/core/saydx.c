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
    eventhandler_init_treebuilder(&msdinp.eventhandler, &treebuilder);
    msdparser_init(&msdparser, &msdinp);
    PROPAGATE_ERROR(
        msdparser_parse_file(&msdparser, name));
    msdparser_final(&msdparser);
    treebuilder_transfer_tree(&treebuilder, root);
    treebuilder_final(&treebuilder);
}
