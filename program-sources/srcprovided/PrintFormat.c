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

#include <PrintFormat.h>
#include <Dictionary.h>
#include <PrintFormat.h>

/** Initialize a print format
 * @param format a print format
 * @warning initialized print format should be finalized with PrintFormat_finalize()
 */
void provided_PrintFormat_init(PrintFormat * format) {
    REGISTRY_USINGFUNCTION;

    format->name = duplicateString("");
    format->header = duplicateString("");
    format->row = duplicateString("");
    format->footer = duplicateString("");
}

/** Finalize a print format
 * @param format the print format
 */
void provided_PrintFormat_finalize(PrintFormat * format) {
    REGISTRY_USINGFUNCTION;

    free(format->name);
    free(format->header);
    free(format->row);
    free(format->footer);
}

static char * readLine(FILE * fichier) {
    char buf[1024];
    char * result = duplicateString("");
    size_t len;

    while (fgets(buf, 1024, fichier) != NULL) {
        char * temp = concatenateString(result, buf);
        free(result);
        result = temp;

        len = stringLength(buf);
        if (len > 0)
            if (buf[len - 1] == '\n') {
                return result;
            }
    }
    return result;
}

/** Load a print format from a file
 * @param format an initialized print format to fill
 * @param filename the file name
 */
void provided_PrintFormat_loadFromFile(PrintFormat * format, const char * filename) {
    char * line;
    FILE * fichier;

    REGISTRY_USINGFUNCTION;

    fichier = fopen(filename, "rt");
    if (fichier == NULL)
        fatalError("PrintFormat_LoadFromFile");

    line = readLine(fichier);
    if (icaseStartWith(".NAME ", line)) {
        free(format->name);
        line[stringLength(line) - 1] = '\0'; /* suppression du saut de ligne */
        format->name = duplicateString(line + 6);
    }
    free(line);

    free(format->header);
    format->header = duplicateString("");
    free(format->row);
    format->row = duplicateString("");
    free(format->footer);
    format->footer = duplicateString("");

    line = readLine(fichier);
    if (icaseStartWith(".HEADER", line)) {
        free(line);
        line = readLine(fichier);

        while (!icaseStartWith(".ROW", line)) {
            char * temp = concatenateString(format->header, line);
            free(format->header);
            format->header = temp;
            free(line);
            line = readLine(fichier);
        }
        format->header[stringLength(format->header) - 1] = '\0'; /* suppression du saut de ligne */
    }

    free(line);
    line = readLine(fichier);
    while (!icaseStartWith(".FOOTER", line)) {
        char * temp = concatenateString(format->row, line);
        free(format->row);
        format->row = temp;
        free(line);
        line = readLine(fichier);
    }
    format->row[stringLength(format->row) - 1] = '\0'; /* suppression du saut de ligne */

    free(line);
    line = readLine(fichier);
    while (!icaseStartWith(".END", line)) {
        char * temp = concatenateString(format->footer, line);
        free(format->footer);
        format->footer = temp;
        free(line);
        line = readLine(fichier);
    }
    format->footer[stringLength(format->footer) - 1] = '\0'; /* suppression du saut de ligne */
    free(line);

    fclose(fichier);
}
