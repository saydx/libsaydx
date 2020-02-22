/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */


#include <stdlib.h>
#include "saydx.h"

int main()
{

    eventprinter_t *eventprinter = eventprinter_create();
    eventhandler_t *eventhandler = eventprinter_cast_to_eventhandler(eventprinter);
    msdparser_input_t msdinp;
    msdinp.eventhandler = eventhandler;
    msdparser_t * msdparser = msdparser_create(&msdinp);

    error_t *err = msdparser_parse_file(msdparser, "test1.msd");
    if (err) {
        fprintf(stderr, "ERROR OCCURED\n");
        error_write(err, stderr);
        exit(1);
    }
    eventhandler_destroy(eventhandler);
    msdparser_destroy(msdparser);
    return 0;
}
