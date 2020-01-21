#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "msdparser.h"

#define CHUNK_SIZE 4096


//
// msdheaderline_t
//

error_t * msdheaderline_init(msdheaderline_t *this, const line_t *line)
{
    char *separator;
    char firstchar;
    int tagnamelen;

    this->nattributes = 0;
    firstchar = line->content[0];
    if (firstchar == '{' || firstchar == '[') {
        this->headertype = (firstchar == '{') ? 1 : 2;
        separator = strchr(line->content + 1, (int) '|');
        if (!separator) {
            tagnamelen = line->length - 1;
        } else {
            tagnamelen = separator - line->content - 1;
            sscanf(separator + 1, "%d", &(this->nattributes));
        }
        this->tagname = MALLOC_OR_DIE((tagnamelen + 1) * sizeof(char));
        memcpy(this->tagname, line->content + 1, tagnamelen * sizeof(char));
        this->tagname[tagnamelen] = '\0';
    } else if (firstchar == '}' || firstchar == ']') {
        this->headertype = (firstchar == '}') ? -1 : -2;
        if (this->headertype == -1) {
            this->tagname = MALLOC_OR_DIE(line->length * sizeof(*this->tagname));
            memcpy(this->tagname, line->content + 1, (line->length - 1) * sizeof(*this->tagname));
            this->tagname[line->length - 1] = '\0';
        } else {
            this->tagname =  MALLOC_OR_DIE(sizeof(*this->tagname));
            this->tagname[0] = '\0';
        }
    } else {
        RETURN_WITH_ERROR("Invalid header line '%s'", line->content);
    }
    RETURN_WITHOUT_ERROR();
}


void msdheaderline_final(msdheaderline_t *this)
{
    free(this->tagname);
}


//
// msdparser_t
//

void msdparser_init(msdparser_t *this, msdparser_input_t *input)
{
    this->eventhandler = input->eventhandler;
}


void msdparser_final(msdparser_t *this)
{
}


error_t * msdparser_parse_file(msdparser_t *this, char *fname)
{
    linereader_t linereader;
    error_t *error;

    PROPAGATE_ERROR(
        linereader_init(&linereader, fname, CHUNK_SIZE));
    this->eventhandler.start_processing(this->eventhandler.handler, fname);
    error = msdparser_parse_open_file(this, &linereader);
    if (error) {
        linereader_final(&linereader);
        PROPAGATE_ERROR(error);
    }
    this->eventhandler.end_processing(this->eventhandler.handler, fname);
    linereader_final(&linereader);
    RETURN_WITHOUT_ERROR();
}


error_t * msdparser_parse_open_file(msdparser_t *this, linereader_t *linereader)
{
    msdheaderline_t headerline;
    attributes_t *attributes = NULL;
    array_t *array = NULL;
    error_t *error = NULL;
    line_t line;

    while (1) {
        linereader_read_line(linereader, &line);
        if (!line.length) {
            line_final(&line);
            break;
        }
        PROPAGATE_ERROR(
            msdheaderline_init(&headerline, &line));
        line_final(&line);

        switch (headerline.headertype) {
        case 1:
            attributes = attributes_create(headerline.nattributes);
            error = msdparser_read_attributes(this, linereader, headerline.nattributes, attributes);
            if (error)
                break;
            this->eventhandler.open_container_node(
                this->eventhandler.handler, headerline.tagname, attributes);
            attributes_destroy(attributes);
            break;
        case 2:
            attributes = attributes_create(headerline.nattributes);
            error = msdparser_read_attributes(this, linereader, headerline.nattributes, attributes);
            if (error)
                break;
            this->eventhandler.open_data_node(
                this->eventhandler.handler, headerline.tagname, attributes);
            attributes_destroy(attributes);
            PROPAGATE_ERROR(
                msdparser_read_array(this, linereader, &array));
            this->eventhandler.receive_array(this->eventhandler.handler, array);
            array_destroy(array);
            break;
        case -1:
            this->eventhandler.close_container_node(this->eventhandler.handler, headerline.tagname);
            break;
        case -2:
            this->eventhandler.close_data_node(this->eventhandler.handler);
        }
        msdheaderline_final(&headerline);
        if (error) {
            PROPAGATE_ERROR(error);
        }
    }
    RETURN_WITHOUT_ERROR();
}


error_t * msdparser_read_attributes(
    msdparser_t *this, linereader_t *linereader, int nattribs, attributes_t *attributes)
{
    line_t line;
    attribute_t attrib;

    for (int iattrib = 0; iattrib < nattribs; ++iattrib) {
        linereader_read_line(linereader, &line);
        if (!line.length) {
            line_final(&line);
            break;
        }
        if (line.content[0] != '&') {
            line_final(&line);
            RETURN_WITH_ERROR("Attribute line ('%s') without starting '&'", line.content);
        }
        char *separator = strchr(line.content + 1, (int) ':');
        if (!separator) {
            line_final(&line);
            RETURN_WITH_ERROR("Attribute line ('%s') without separator", line.content);
        }
        int wordlen = separator - line.content - 1;
        attrib.name = MALLOC_OR_DIE((wordlen + 1) * sizeof(*attrib.name));
        memcpy(attrib.name, line.content + 1, wordlen * sizeof(*attrib.name));
        attrib.name[wordlen] = '\0';
        wordlen = line.length - wordlen - 2;
        attrib.value = MALLOC_OR_DIE((wordlen + 1) * sizeof(*attrib.value));
        memcpy(attrib.value, separator + 1, wordlen * sizeof(*attrib.value));
        attrib.value[wordlen] = '\0';
        attributes_append(attributes, &attrib);
        line_final(&line);
    }
    RETURN_WITHOUT_ERROR();
}


error_t * msdparser_read_array(msdparser_t *this, linereader_t *linereader,
                          array_t **array)
{
    line_t line;
    int nitems, idim;

    linereader_read_line(linereader, &line);
    *array = array_create(&line);
    if ((*array)->rank) {
        nitems = 1;
        for (idim = 0; idim < (*array)->rank; ++idim) {
            nitems *= (*array)->shape[idim];
        }
    }
    else {
        nitems = 1;
    }
    PROPAGATE_ERROR(
        msdparser_read_raw_data(*array, linereader, nitems));
    line_final(&line);
    RETURN_WITHOUT_ERROR();
}


error_t * msdparser_read_raw_data(
    array_t *desc, linereader_t *linereader, int nitems)
{
    line_t line;
    data_reader_t datareader;
    size_t datasize;

    if (strcmp(desc->typename, "i4") == 0) {
        datasize = sizeof(int);
        datareader = &read_i4_data;
    } else if (strcmp(desc->typename, "r8") == 0) {
        datasize = sizeof(double);
        datareader = &read_r8_data;
    } else if (strcmp(desc->typename, "l") == 0) {
        datasize = sizeof(int);
        datareader = &read_logical_data;
    } else if (strcmp(desc->typename, "s") == 0) {
        datasize = sizeof(string_t);
        datareader = &read_string_data;
    } else {
        RETURN_WITH_ERROR("Invalid data type '%s'", desc->typename);
    }

    desc->rawdata = MALLOC_OR_DIE(nitems * datasize);
    int items = 0;
    int itemsread;
    while (items < nitems) {
        linereader_read_line(linereader, &line);
        itemsread = datareader(desc->rawdata, &line, items, nitems - items);
        items += itemsread;
        line_final(&line);
    }
    RETURN_WITHOUT_ERROR();
}


int read_i4_data(void *rawdata, const line_t *line, int items, int nitems)
{
    int *data = rawdata;
    data += items;
    char *startptr = line->content;
    char *endptr;
    long buffer = strtol(startptr, &endptr, 10);
    int iitem = 0;
    while (endptr != startptr && iitem < nitems) {
        data[iitem] = (int) buffer;
        iitem++;
        startptr = endptr;
        buffer = strtol(startptr, &endptr, 10);
    }
    return iitem;
}


int read_r8_data(void *rawdata, const line_t *line, int items, int nitems)
{
    double *data = rawdata;
    data += items;
    char *startptr = line->content;
    char *endptr;
    double buffer = strtod(startptr, &endptr);
    int iitem = 0;
    while (endptr != startptr && iitem < nitems) {
        data[iitem] = buffer;
        iitem++;
        startptr = endptr;
        buffer = strtod(startptr, &endptr);
    }
    return iitem;
}


int read_logical_data(void *rawdata, const line_t *line, int items, int nitems)
{
    int *data = rawdata;
    data += items;
    char *pchar = line->content;
    int iitem = 0;
    while (iitem < nitems) {
        while (*pchar == ' ') pchar++;
        if (*pchar == 0) return iitem;
        data[iitem] = (*pchar == 'T');
        pchar++;
        iitem++;
    }
    return iitem;
}


int read_string_data(void *rawdata, const line_t *line, int items, int nitems)
{
    string_t *data = rawdata;
    data += items;
    data->length = line->length;
    data->content = MALLOC_OR_DIE(data->length * sizeof(*data->content));
    memcpy(data->content, line->content + 1, (line->length - 1) * sizeof(*data->content));
    data->content[data->length - 1] = '\0';
    return 1;
}
