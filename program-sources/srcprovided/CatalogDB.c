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

#include <CatalogDB.h>

/** Create a database of products
 * @param filename the file name of the database
 * @return a pointer on a CatalogDB representing the opened database, NULL otherwise
 */
CatalogDB * provided_CatalogDB_create(const char * filename)
{
  CatalogDB * catalogDB;

  REGISTRY_USINGFUNCTION;

  catalogDB = (CatalogDB*) malloc(sizeof(CatalogDB));
  if (catalogDB == NULL)
    fatalError("CatalogDB_Create: malloc failed");

  catalogDB->file = fopen(filename, "w+b");
  if (catalogDB->file == NULL)
  {
    free(catalogDB);
    return NULL;
  }

  catalogDB->recordCount = 0;
  fseek(catalogDB->file, 0, SEEK_SET);
  if (fwrite(&catalogDB->recordCount, sizeof(catalogDB->recordCount), 1, catalogDB->file) != 1)
  {
    fclose(catalogDB->file);
    free(catalogDB);
    fatalError("CatalogDB_Create: fwrite failed");
  }
  return catalogDB;
}

/** Open an existing database of products
 * @param filename the file name of the database
 * @return a pointer on a CatalogDB representing the opened database, NULL otherwise
 */
CatalogDB * provided_CatalogDB_open(const char * filename)
{
  CatalogDB * catalogDB;
  REGISTRY_USINGFUNCTION;

  catalogDB = (CatalogDB*) malloc(sizeof(CatalogDB));
  if (catalogDB == NULL)
    fatalError("CatalogDB_Open: malloc failed");

  catalogDB->file = fopen(filename, "r+b");
  if (catalogDB->file == NULL)
  {
    free(catalogDB);
    return NULL;
  }

  fseek(catalogDB->file, 0, SEEK_SET);
  if (fread(&catalogDB->recordCount, sizeof(catalogDB->recordCount), 1, catalogDB->file) != 1)
  {
    fclose(catalogDB->file);
    free(catalogDB);
    fatalError("CatalogDB_Open: fread failed");
  }
  return catalogDB;
}

/** Open if exists or create otherwise a database of products
 * @param filename the file name of the database
 * @return a pointer on a CatalogDB representing the opened database, NULL otherwise
 */
CatalogDB * provided_CatalogDB_openOrCreate(const char * filename)
{
  CatalogDB * catalogDB;

  REGISTRY_USINGFUNCTION;

  catalogDB = CatalogDB_open(filename);
  if (catalogDB == NULL)
    catalogDB = CatalogDB_create(filename);
  return catalogDB;
}

/** Flush cached data, close a database and free the structure representing the opened database
 * @param catalogDB a pointer on a CatalogDB representing the opened database
 */
void provided_CatalogDB_close(CatalogDB * catalogDB)
{
  REGISTRY_USINGFUNCTION;

  if (catalogDB != NULL)
  {
    fseek(catalogDB->file, 0, SEEK_SET);
    if (fwrite(&catalogDB->recordCount, sizeof(catalogDB->recordCount), 1, catalogDB->file) != 1)
      fatalError("CatalogDB_Close: fwrite failed");
    fclose(catalogDB->file);
    free(catalogDB);
  }
}

/** Get the number of records of the database
 * @param catalogDB the database
 * @return the number of records
 */
int provided_CatalogDB_getRecordCount(CatalogDB * catalogDB)
{
  REGISTRY_USINGFUNCTION;

  if (catalogDB != NULL)
    return catalogDB->recordCount;
  else
    return 0;
}

/** Append the specified record at the end of the database
 * @param catalogDB the database
 * @param record the record
 */
void provided_CatalogDB_appendRecord(CatalogDB * catalogDB, CatalogRecord *record)
{
  REGISTRY_USINGFUNCTION;

  CatalogDB_insertRecord(catalogDB, CatalogDB_getRecordCount(catalogDB), record);
}

/** Insert the specified record at the given position in the database
 * @param catalogDB the database
 * @param recordIndex the insertion position
 * @param record the record
 */
void provided_CatalogDB_insertRecord(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record)
{
  REGISTRY_USINGFUNCTION;

  if (catalogDB != NULL)
  {
    int i;
    CatalogRecord temp;
    CatalogRecord_init(&temp);
    for(i = CatalogDB_getRecordCount(catalogDB); i > recordIndex; --i)
    {
      CatalogDB_readRecord(catalogDB, i - 1, &temp);
      CatalogDB_writeRecord(catalogDB, i, &temp);
    }
    CatalogDB_writeRecord(catalogDB, recordIndex, record);
    CatalogRecord_finalize(&temp);
  }
}

/** Remove a record at a given position from the database
 * @param catalogDB the database
 * @param recordIndex the removal position
 */
void provided_CatalogDB_removeRecord(CatalogDB * catalogDB, int recordIndex)
{
  REGISTRY_USINGFUNCTION;

  if (catalogDB != NULL)
  {
    CatalogRecord temp;
    int i;
    CatalogRecord_init(&temp);
    for(i = recordIndex; i < CatalogDB_getRecordCount(catalogDB) - 1; ++i)
    {
      CatalogDB_readRecord(catalogDB, i + 1, &temp);
      CatalogDB_writeRecord(catalogDB, i, &temp);
    }
    catalogDB->recordCount--;
    CatalogRecord_finalize(&temp);
  }
}

/** Read a record from the database
 * @param catalogDB the database
 * @param recordIndex the position of the record to read
 * @param record the record to fill with data
 */
void provided_CatalogDB_readRecord(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record)
{
  REGISTRY_USINGFUNCTION;

  if (catalogDB != NULL)
  {
    if (recordIndex < 0 || recordIndex >= catalogDB->recordCount)
      fatalError("CatalogDB_GetRecord: invalid index");
    fseek(catalogDB->file, (long) (sizeof(catalogDB->recordCount) + CATALOGRECORD_SIZE * (unsigned long)recordIndex), SEEK_SET);
    CatalogRecord_read(record, catalogDB->file);
  }
}

/** Write a record from the database
 * @param catalogDB the database
 * @param recordIndex the position of the record to write
 * @param record the record containing the data
 */
void provided_CatalogDB_writeRecord(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record)
{
  REGISTRY_USINGFUNCTION;

  if (catalogDB != NULL)
  {
    if (recordIndex < 0 || recordIndex > catalogDB->recordCount)
      fatalError("CatalogDB_SetRecord: invalid index");
    fseek(catalogDB->file, (long) (sizeof(catalogDB->recordCount) + CATALOGRECORD_SIZE * (unsigned long)recordIndex), SEEK_SET);
    CatalogRecord_write(record, catalogDB->file);
    if (recordIndex == catalogDB->recordCount)
      catalogDB->recordCount++;
  }
}
