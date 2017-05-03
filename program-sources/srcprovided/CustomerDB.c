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

#include <CustomerDB.h>

CustomerDB * provided_CustomerDB_create(const char * filename) {
    CustomerDB * customerDB;

    REGISTRY_USINGFUNCTION;

    customerDB = (CustomerDB*) malloc(sizeof(CustomerDB));
    if (customerDB == NULL)
        fatalError("CustomerDB_Create: malloc failed");

    customerDB->file = fopen(filename, "w+b");
    if (customerDB->file == NULL) {
        free(customerDB);
        return NULL;
    }

    customerDB->recordCount = 0;
    fseek(customerDB->file, 0, SEEK_SET);
    if (fwrite(&customerDB->recordCount, sizeof(customerDB->recordCount), 1, customerDB->file) != 1) {
        fclose(customerDB->file);
        free(customerDB);
        fatalError("CustomerDB_Create: fwrite failed");
    }
    return customerDB;
}

CustomerDB * provided_CustomerDB_open(const char * filename) {
    CustomerDB * customerDB;

    REGISTRY_USINGFUNCTION;

    customerDB = (CustomerDB*) malloc(sizeof(CustomerDB));
    if (customerDB == NULL)
        fatalError("CustomerDB_Open: malloc failed");

    customerDB->file = fopen(filename, "r+b");
    if (customerDB->file == NULL) {
        free(customerDB);
        return NULL;
    }

    fseek(customerDB->file, 0, SEEK_SET);
    if (fread(&customerDB->recordCount, sizeof(customerDB->recordCount), 1, customerDB->file) != 1) {
        fclose(customerDB->file);
        free(customerDB);
        fatalError("CustomerDB_Open: fread failed");
    }
    return customerDB;
}

CustomerDB * provided_CustomerDB_openOrCreate(const char * filename) {
    CustomerDB * customerDB;

    REGISTRY_USINGFUNCTION;

    customerDB = CustomerDB_open(filename);
    if (customerDB == NULL)
        customerDB = CustomerDB_create(filename);
    return customerDB;
}

void provided_CustomerDB_close(CustomerDB * customerDB) {
    REGISTRY_USINGFUNCTION;

    if (customerDB != NULL) {
        fseek(customerDB->file, 0, SEEK_SET);
        if (fwrite(&customerDB->recordCount, sizeof(customerDB->recordCount), 1, customerDB->file)
                != 1)
            fatalError("CustomerDB_Close: fwrite failed");
        fclose(customerDB->file);
        free(customerDB);
    }
}

int provided_CustomerDB_getRecordCount(CustomerDB * customerDB) {
    REGISTRY_USINGFUNCTION;

    if (customerDB != NULL)
        return customerDB->recordCount;
    else
        return 0;
}

void provided_CustomerDB_appendRecord(CustomerDB * customerDB, CustomerRecord *record) {
    REGISTRY_USINGFUNCTION;

    CustomerDB_insertRecord(customerDB, CustomerDB_getRecordCount(customerDB), record);
}

void provided_CustomerDB_insertRecord(CustomerDB * customerDB, int recordIndex,
        CustomerRecord * record) {
    REGISTRY_USINGFUNCTION;

    if (customerDB != NULL) {
        int i;
        CustomerRecord temp;
        CustomerRecord_init(&temp);
        for (i = CustomerDB_getRecordCount(customerDB); i > recordIndex; --i) {
            CustomerDB_readRecord(customerDB, i - 1, &temp);
            CustomerDB_writeRecord(customerDB, i, &temp);
        }
        CustomerDB_writeRecord(customerDB, recordIndex, record);
        CustomerRecord_finalize(&temp);
    }
}

void provided_CustomerDB_removeRecord(CustomerDB * customerDB, int recordIndex) {
    REGISTRY_USINGFUNCTION;

    if (customerDB != NULL) {
        CustomerRecord temp;
        int i;
        CustomerRecord_init(&temp);
        for (i = recordIndex; i < CustomerDB_getRecordCount(customerDB) - 1; ++i) {
            CustomerDB_readRecord(customerDB, i + 1, &temp);
            CustomerDB_writeRecord(customerDB, i, &temp);
        }
        customerDB->recordCount--;
        CustomerRecord_finalize(&temp);
    }
}

void provided_CustomerDB_readRecord(CustomerDB * customerDB, int recordIndex,
        CustomerRecord * record) {
    REGISTRY_USINGFUNCTION;

    if (customerDB != NULL) {
        if (recordIndex < 0 || recordIndex >= customerDB->recordCount)
            fatalError("CustomerDB_GetRecord: invalid index");
        fseek(customerDB->file, (long) (sizeof(customerDB->recordCount) + CUSTOMERRECORD_SIZE
                * (long unsigned)recordIndex), SEEK_SET);
        CustomerRecord_read(record, customerDB->file);
    }
}

void provided_CustomerDB_writeRecord(CustomerDB * customerDB, int recordIndex,
        CustomerRecord * record) {
    REGISTRY_USINGFUNCTION;

    if (customerDB != NULL) {
        if (recordIndex < 0 || recordIndex > customerDB->recordCount)
            fatalError("CustomerDB_SetRecord: invalid index");
        fseek(customerDB->file, (long) (sizeof(customerDB->recordCount) + CUSTOMERRECORD_SIZE
                * (long unsigned)recordIndex), SEEK_SET);
        CustomerRecord_write(record, customerDB->file);
        if (recordIndex == customerDB->recordCount)
            customerDB->recordCount++;
    }
}
