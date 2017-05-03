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

#include <CatalogRecord.h>

/** Static function which test if a code only contains numbers and letters
 * @param  value the value to test
 * @return true if the code is valid, false otherwise
 */
int provided_CatalogRecord_isValueValid_code(const char * value) {
    const char * p;

    REGISTRY_USINGFUNCTION;

    p = value;
    while (*p != '\0') {
        if (!(('0' <= *p && *p <= '9') || ('a' <= *p && *p <= 'z') || ('A' <= *p && *p <= 'Z')))
            return 0;
        p++;
    }
    return p != value;
}

/** Static function which test if the value is a positive number
 * @param  value the value to test
 * @return true if the valie is valid, false otherwise
 */
int provided_CatalogRecord_isValueValid_positiveNumber(const char * value) {
    char * end;
    double val;

    REGISTRY_USINGFUNCTION;

    end = NULL;
    val = strtod(value, &end);
    return (end != value && *end == '\0' && val >= 0);
}

/** Static function to set the code field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void provided_CatalogRecord_setValue_code(CatalogRecord * record, const char * value) {
    REGISTRY_USINGFUNCTION;

    free(record->code);
    record->code = duplicateString(value);
}

/** Static function to set the designation field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void provided_CatalogRecord_setValue_designation(CatalogRecord * record, const char * value) {
    REGISTRY_USINGFUNCTION;

    free(record->designation);
    record->designation = duplicateString(value);
}

/** Static function to set the unity field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void provided_CatalogRecord_setValue_unity(CatalogRecord * record, const char * value) {
    REGISTRY_USINGFUNCTION;

    free(record->unity);
    record->unity = duplicateString(value);
}

/** Static function to set the basePrice field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void provided_CatalogRecord_setValue_basePrice(CatalogRecord * record, const char * value) {
    REGISTRY_USINGFUNCTION;

    record->basePrice = atof(value);
}

/** Static function to set the sellingPrice field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void provided_CatalogRecord_setValue_sellingPrice(CatalogRecord * record, const char * value) {
    REGISTRY_USINGFUNCTION;

    record->sellingPrice = atof(value);
}

/** Static function to set the rateOfVAT field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void provided_CatalogRecord_setValue_rateOfVAT(CatalogRecord * record, const char * value) {
    REGISTRY_USINGFUNCTION;

    record->rateOfVAT = atof(value);
}

/** Static function which create a copy string on the head of the code field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * provided_CatalogRecord_getValue_code(CatalogRecord * record) {
    REGISTRY_USINGFUNCTION;

    return duplicateString(record->code);
}

/** Static function which create a copy string on the head of the designation field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * provided_CatalogRecord_getValue_designation(CatalogRecord * record) {
    REGISTRY_USINGFUNCTION;

    return duplicateString(record->designation);
}

/** Static function which create a copy string on the head of the unity field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * provided_CatalogRecord_getValue_unity(CatalogRecord * record) {
    REGISTRY_USINGFUNCTION;

    return duplicateString(record->unity);
}

/** Static function which create a copy string on the head of the basePrice field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * provided_CatalogRecord_getValue_basePrice(CatalogRecord * record) {
    char buf[1024];

    REGISTRY_USINGFUNCTION;

    snprintf(buf, 1024, "%.2f", record->basePrice);
    return duplicateString(buf);
}

/** Static function which create a copy string on the head of the sellingPrice field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * provided_CatalogRecord_getValue_sellingPrice(CatalogRecord * record) {
    char buf[1024];

    REGISTRY_USINGFUNCTION;

    snprintf(buf, 1024, "%.2f", record->sellingPrice);
    return duplicateString(buf);
}

/** Static function which create a copy string on the head of the rateOfVAT field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * provided_CatalogRecord_getValue_rateOfVAT(CatalogRecord * record) {
    char buf[1024];

    REGISTRY_USINGFUNCTION;

    snprintf(buf, 1024, "%.2f", record->rateOfVAT);
    return duplicateString(buf);
}

/** Initialize a record
 * @param[in] record a pointer to a record to initialize
 * @warning every initialized record must be finalized with CatalogRecord_finalize()
 */
void provided_CatalogRecord_init(CatalogRecord * record) {
    REGISTRY_USINGFUNCTION;

    record->code = duplicateString("");
    record->designation = duplicateString("");
    record->unity = duplicateString("");
    record->basePrice = 0;
    record->sellingPrice = 0;
    record->rateOfVAT = 0;
}

/** Finalize a record.
 * @param[in] record a pointer to a record to finalize
 */
void provided_CatalogRecord_finalize(CatalogRecord * record) {
    REGISTRY_USINGFUNCTION;

    if (record->code != NULL)
        free(record->code);
    if (record->designation != NULL)
        free(record->designation);
    if (record->unity != NULL)
        free(record->unity);
}

/** Read a record from a file
 * @param record a pointer to an initialized record on which to store data
 * @param file the file from which the data are read
 */
void provided_CatalogRecord_read(CatalogRecord * record, FILE * file) {
    char buffer[CATALOGRECORD_MAXSTRING_SIZE];

    REGISTRY_USINGFUNCTION;

    if (fread(buffer, CATALOGRECORD_CODE_SIZE, 1, file) != 1)
        fatalError("CatalogRecord_Read: fread");
    if (record->code != NULL)
        free(record->code);
    record->code = duplicateString(buffer);

    if (fread(buffer, CATALOGRECORD_DESIGNATION_SIZE, 1, file) != 1)
        fatalError("CatalogRecord_Read: fread");
    if (record->designation != NULL)
        free(record->designation);
    record->designation = duplicateString(buffer);

    if (fread(buffer, CATALOGRECORD_UNITY_SIZE, 1, file) != 1)
        fatalError("CatalogRecord_Read: fread");
    if (record->unity != NULL)
        free(record->unity);
    record->unity = duplicateString(buffer);

    if (fread(&record->basePrice, sizeof(record->basePrice), 1, file) != 1)
        fatalError("CatalogRecord_Read: fread");

    if (fread(&record->sellingPrice, sizeof(record->sellingPrice), 1, file) != 1)
        fatalError("CatalogRecord_Read: fread");

    if (fread(&record->rateOfVAT, sizeof(record->rateOfVAT), 1, file) != 1)
        fatalError("CatalogRecord_Read: fread");
}

static void zeroTab(char * tab, int size) {
    while (size > 0) {
        *tab = '\0';
        tab++;
        size--;
    }
}

/** Write a record to a file
 * @param record a pointer to a record
 * @param file the file to which the data are written
 */
void provided_CatalogRecord_write(CatalogRecord * record, FILE * file) {
    char buffer[CATALOGRECORD_MAXSTRING_SIZE];

    REGISTRY_USINGFUNCTION;

    zeroTab(buffer, CATALOGRECORD_MAXSTRING_SIZE);

    copyStringWithLength(buffer, record->code, CATALOGRECORD_CODE_SIZE);
    if (fwrite(buffer, CATALOGRECORD_CODE_SIZE, 1, file) != 1)
        fatalError("CatalogRecord_Write: fwrite");

    copyStringWithLength(buffer, record->designation, CATALOGRECORD_DESIGNATION_SIZE);
    if (fwrite(buffer, CATALOGRECORD_DESIGNATION_SIZE, 1, file) != 1)
        fatalError("CatalogRecord_Write: fwrite");

    copyStringWithLength(buffer, record->unity, CATALOGRECORD_UNITY_SIZE);
    if (fwrite(buffer, CATALOGRECORD_UNITY_SIZE, 1, file) != 1)
        fatalError("CatalogRecord_Write: fwrite");

    if (fwrite(&record->basePrice, sizeof(record->basePrice), 1, file) != 1)
        fatalError("CatalogRecord_Write: fwrite");

    if (fwrite(&record->sellingPrice, sizeof(record->sellingPrice), 1, file) != 1)
        fatalError("CatalogRecord_Write: fwrite");

    if (fwrite(&record->rateOfVAT, sizeof(record->rateOfVAT), 1, file) != 1)
        fatalError("CatalogRecord_Write: fwrite");
}
