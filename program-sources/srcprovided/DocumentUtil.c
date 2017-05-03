/**
 * Copyright 2010 Sébastien Aupetit <sebtic@projectsforge.org>
 *
 * This source code is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this source code. If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id$
 */

#include <DocumentUtil.h>

/** Create a new string on the heap which represents the parameter as a number in basis 36.
 * @param id the number to convert
 * @return a new string
 * @note The string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * provided_computeDocumentNumber(long id) {
    char buf[1024];
    int pos = 0;
    int len = 0;
    long temp = id;
    long val;

    REGISTRY_USINGFUNCTION;

    while (temp > 0) {
        len++;
        temp = temp / 36;
    }

    while (id > 0) {
        val = id % 36;
        if (val < 10)
            buf[len - 1 - pos] = (char) ('0' + val);
        else
            buf[len - 1 - pos] = (char) ('A' + val - 10);
        pos++;
        id = id / 36;
    }
    buf[pos] = '\0';
    return duplicateString(buf);
}

/** Create a new string on the heap which represents the date in the format DD/MM/YYYY.
 * @param day the day
 * @param month the month
 * @param year the year
 * @return a new string
 * @note The string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * provided_formatDate(int day, int month, int year) {
    char buf[1024];

    REGISTRY_USINGFUNCTION;
    snprintf(buf, 1024, "%02d/%02d/%4d", day, month, year);
    return duplicateString(buf);
}

/** Write a string in a binary file
 * @param str the string
 * @param file the file
 */
void provided_writeString(const char * str, FILE * file) {
    size_t len;

    REGISTRY_USINGFUNCTION;

    len = stringLength(str);
    if (fwrite(&len, sizeof(len), 1, file) != 1)
        fatalError("writeString");
    if (len > 0)
        if (fwrite(str, len, 1, file) != 1)
            fatalError("writeString");
}

/** Read a string from a binary file
 * @param file the file
 * @return a new string created on the heap which contains the read string
 * @see writeString()
 */
char * provided_readString(FILE * file) {
    size_t len;
    char * result;

    REGISTRY_USINGFUNCTION;

    if (fread(&len, sizeof(len), 1, file) != 1)
        fatalError("readString");
    result = (char*) malloc(len + 1);
    if (len > 0)
        if (fread(result, len, 1, file) != 1)
            fatalError("readString");
    result[len] = '\0';
    return result;
}
