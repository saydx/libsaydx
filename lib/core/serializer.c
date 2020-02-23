/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <stdlib.h>
#include "serializer.h"


void serializer_destroy(serializer_t *serializer)
{
    serializer->final(serializer->handler);
    free(serializer);
}
