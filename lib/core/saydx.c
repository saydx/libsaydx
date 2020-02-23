#include "saydx.h"
#include "commontypes.h"

error_t * read_msd_file(const char *name, node_t **root)
{
    treebuilder_t *treebuilder = treebuilder_create();
    eventhandler_t *eventhandler = treebuilder_cast_to_eventhandler(treebuilder);
    msdparser_input_t msdinp;
    msdinp.eventhandler = eventhandler;
    msdparser_t *msdparser = msdparser_create(&msdinp);
    PROPAGATE_ERROR(
        msdparser_parse_file(msdparser, name));
    msdparser_destroy(msdparser);
    treebuilder_transfer_tree(treebuilder, root);
    eventhandler_destroy(eventhandler);
    RETURN_WITHOUT_ERROR();
}
