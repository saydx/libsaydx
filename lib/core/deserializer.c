/*
 * Copyright (c) 2020 Bálint Aradi, Universität Bremen. All rights reserved.
 * Use of this source code is governed by a BSD-style
 * license that can be found in the LICENSE file.
 */

#include <stdlib.h>
#include "deserializer.h"


void deserializer_destroy(deserializer_t *deserializer)
{
    deserializer->final(deserializer->handler);
    free(deserializer);
}
