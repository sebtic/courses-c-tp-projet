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

#include <Config.h>
#include <MyString.h>
#include <CatalogDB.h>
#include <CatalogRecord.h>
#include <CustomerDB.h>
#include <CustomerRecord.h>
#include <Dictionary.h>
#include <Document.h>
#include <DocumentRowList.h>
#include <DocumentUtil.h>
#include <EncryptDecrypt.h>
#include <MyString.h>
#include <OperatorTable.h>
#include <PrintFormat.h>

#undef strcmp
#undef strlen
#undef strcpy
#undef strncpy
#undef strdup
#undef strcasecmp
#undef tolower
#undef toupper
#undef strcat
#undef strncat
#undef index
#undef strstr
#include <string.h>

/** A registry record */
typedef struct _RegistryEntry
{
  const char * name;
  const char * file;
  int displayed;
  long usage;
} RegistryEntry;

/** The registry table */
static RegistryEntry * registry;
/** the size of the registry */
static size_t registryCount;

void Registry_init(void)
{
  registry = NULL;
  registryCount = 0;
  atexit(Registry_dumpUsage);
}

static RegistryEntry * Registry_getEntry(const char * name)
{
  size_t i;

  for(i = 0; i < registryCount; ++i)
  {
    if (strcmp(registry[i].name, name) == 0)
    {
      return &registry[i];
    }
  }
  return NULL;
}

/** Declare the usage of a provided function
 * @internal
 * @param name the name of the function
 */
void Registry_usingFunction(const char * name, const char * file)
{
  RegistryEntry * entry = Registry_getEntry(name);

  if (entry == NULL)
  {
    registry = (RegistryEntry *) realloc(registry, sizeof(RegistryEntry) * (size_t) (registryCount + 1));
    if (registry == NULL)
      fatalError("Out of memory while updating registry of provided functions");
    registry[registryCount].name = name;
    registry[registryCount].displayed = 0;
    registry[registryCount].file = file + strlen("srcprovided/");
    registry[registryCount].usage = 0;
    entry = &registry[registryCount];
    registryCount++;
  }

  entry->usage++;

  if (CatalogDB_create == NULL)
    fatalError("Function pointer undefined");
  if (CatalogDB_open == NULL)
    fatalError("Function pointer undefined");
  if (CatalogDB_openOrCreate == NULL)
    fatalError("Function pointer undefined");
  if (CatalogDB_close == NULL)
    fatalError("Function pointer undefined");
  if (CatalogDB_getRecordCount == NULL)
    fatalError("Function pointer undefined");
  if (CatalogDB_appendRecord == NULL)
    fatalError("Function pointer undefined");
  if (CatalogDB_insertRecord == NULL)
    fatalError("Function pointer undefined");
  if (CatalogDB_removeRecord == NULL)
    fatalError("Function pointer undefined");
  if (CatalogDB_readRecord == NULL)
    fatalError("Function pointer undefined");
  if (CatalogDB_writeRecord == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_isValueValid_code == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_isValueValid_positiveNumber == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_setValue_code == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_setValue_designation == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_setValue_unity == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_setValue_basePrice == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_setValue_sellingPrice == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_setValue_rateOfVAT == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_getValue_code == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_getValue_designation == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_getValue_unity == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_getValue_basePrice == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_getValue_sellingPrice == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_getValue_rateOfVAT == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_init == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_finalize == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_read == NULL)
    fatalError("Function pointer undefined");
  if (CatalogRecord_write == NULL)
    fatalError("Function pointer undefined");
  if (CustomerDB_create == NULL)
    fatalError("Function pointer undefined");
  if (CustomerDB_open == NULL)
    fatalError("Function pointer undefined");
  if (CustomerDB_openOrCreate == NULL)
    fatalError("Function pointer undefined");
  if (CustomerDB_close == NULL)
    fatalError("Function pointer undefined");
  if (CustomerDB_getRecordCount == NULL)
    fatalError("Function pointer undefined");
  if (CustomerDB_appendRecord == NULL)
    fatalError("Function pointer undefined");
  if (CustomerDB_insertRecord == NULL)
    fatalError("Function pointer undefined");
  if (CustomerDB_removeRecord == NULL)
    fatalError("Function pointer undefined");
  if (CustomerDB_readRecord == NULL)
    fatalError("Function pointer undefined");
  if (CustomerDB_writeRecord == NULL)
    fatalError("Function pointer undefined");
  if (CustomerRecord_setValue_name == NULL)
    fatalError("Function pointer undefined");
  if (CustomerRecord_setValue_address == NULL)
    fatalError("Function pointer undefined");
  if (CustomerRecord_setValue_postalCode == NULL)
    fatalError("Function pointer undefined");
  if (CustomerRecord_setValue_town == NULL)
    fatalError("Function pointer undefined");
  if (CustomerRecord_getValue_name == NULL)
    fatalError("Function pointer undefined");
  if (CustomerRecord_getValue_address == NULL)
    fatalError("Function pointer undefined");
  if (CustomerRecord_getValue_postalCode == NULL)
    fatalError("Function pointer undefined");
  if (CustomerRecord_getValue_town == NULL)
    fatalError("Function pointer undefined");
  if (CustomerRecord_init == NULL)
    fatalError("Function pointer undefined");
  if (CustomerRecord_finalize == NULL)
    fatalError("Function pointer undefined");
  if (CustomerRecord_read == NULL)
    fatalError("Function pointer undefined");
  if (CustomerRecord_write == NULL)
    fatalError("Function pointer undefined");
  if (Dictionary_create == NULL)
    fatalError("Function pointer undefined");
  if (Dictionary_destroy == NULL)
    fatalError("Function pointer undefined");
  if (Dictionary_getEntry == NULL)
    fatalError("Function pointer undefined");
  if (Dictionary_setStringEntry == NULL)
    fatalError("Function pointer undefined");
  if (Dictionary_setNumberEntry == NULL)
    fatalError("Function pointer undefined");
  if (Dictionary_format == NULL)
    fatalError("Function pointer undefined");
  if (Document_init == NULL)
    fatalError("Function pointer undefined");
  if (Document_finalize == NULL)
    fatalError("Function pointer undefined");
  if (Document_saveToFile == NULL)
    fatalError("Function pointer undefined");
  if (Document_loadFromFile == NULL)
    fatalError("Function pointer undefined");
  if (DocumentRow_init == NULL)
    fatalError("Function pointer undefined");
  if (DocumentRow_finalize == NULL)
    fatalError("Function pointer undefined");
  if (DocumentRow_create == NULL)
    fatalError("Function pointer undefined");
  if (DocumentRow_destroy == NULL)
    fatalError("Function pointer undefined");
  if (DocumentRowList_init == NULL)
    fatalError("Function pointer undefined");
  if (DocumentRowList_finalize == NULL)
    fatalError("Function pointer undefined");
  if (DocumentRowList_get == NULL)
    fatalError("Function pointer undefined");
  if (DocumentRowList_getRowCount == NULL)
    fatalError("Function pointer undefined");
  if (DocumentRowList_pushBack == NULL)
    fatalError("Function pointer undefined");
  if (DocumentRowList_insertBefore == NULL)
    fatalError("Function pointer undefined");
  if (DocumentRowList_insertAfter == NULL)
    fatalError("Function pointer undefined");
  if (DocumentRowList_removeRow == NULL)
    fatalError("Function pointer undefined");
  if (DocumentRow_writeRow == NULL)
    fatalError("Function pointer undefined");
  if (DocumentRow_readRow == NULL)
    fatalError("Function pointer undefined");
  if (computeDocumentNumber == NULL)
    fatalError("Function pointer undefined");
  if (formatDate == NULL)
    fatalError("Function pointer undefined");
  if (writeString == NULL)
    fatalError("Function pointer undefined");
  if (readString == NULL)
    fatalError("Function pointer undefined");
  if (encrypt == NULL)
    fatalError("Function pointer undefined");
  if (decrypt == NULL)
    fatalError("Function pointer undefined");
  if (stringLength == NULL)
    fatalError("Function pointer undefined");
  if (compareString == NULL)
    fatalError("Function pointer undefined");
  if (icaseCompareString == NULL)
    fatalError("Function pointer undefined");
  if (copyString == NULL)
    fatalError("Function pointer undefined");
  if (copyStringWithLength == NULL)
    fatalError("Function pointer undefined");
  if (duplicateString == NULL)
    fatalError("Function pointer undefined");
  if (icaseStartWith == NULL)
    fatalError("Function pointer undefined");
  if (icaseEndWith == NULL)
    fatalError("Function pointer undefined");
  if (concatenateString == NULL)
    fatalError("Function pointer undefined");
  if (toLowerChar == NULL)
    fatalError("Function pointer undefined");
  if (toUpperChar == NULL)
    fatalError("Function pointer undefined");
  if (makeUpperCaseString == NULL)
    fatalError("Function pointer undefined");
  if (makeLowerCaseString == NULL)
    fatalError("Function pointer undefined");
  if (indexOfChar == NULL)
    fatalError("Function pointer undefined");
  if (indexOfString == NULL)
    fatalError("Function pointer undefined");
  if (subString == NULL)
    fatalError("Function pointer undefined");
  if (insertString == NULL)
    fatalError("Function pointer undefined");
  if (OperatorTable_create == NULL)
    fatalError("Function pointer undefined");
  if (OperatorTable_destroy == NULL)
    fatalError("Function pointer undefined");
  if (OperatorTable_loadFromFile == NULL)
    fatalError("Function pointer undefined");
  if (OperatorTable_saveToFile == NULL)
    fatalError("Function pointer undefined");
  if (OperatorTable_getRecordCount == NULL)
    fatalError("Function pointer undefined");
  if (OperatorTable_getName == NULL)
    fatalError("Function pointer undefined");
  if (OperatorTable_getPassword == NULL)
    fatalError("Function pointer undefined");
  if (OperatorTable_findOperator == NULL)
    fatalError("Function pointer undefined");
  if (OperatorTable_setOperator == NULL)
    fatalError("Function pointer undefined");
  if (OperatorTable_removeRecord == NULL)
    fatalError("Function pointer undefined");
  if (PrintFormat_init == NULL)
    fatalError("Function pointer undefined");
  if (PrintFormat_finalize == NULL)
    fatalError("Function pointer undefined");
  if (PrintFormat_loadFromFile == NULL)
    fatalError("Function pointer undefined");

}

static int compareEntry(const void * p1, const void * p2)
{
  /* The actual arguments to this function are "pointers to
   pointers to char", but strcmp() arguments are "pointers
   to char", hence the following cast plus dereference */

  return strcmp(*(char * const *) p1, *(char * const *) p2);
}

/** Display the usage of the provided functions
 */
void Registry_dumpUsage(void)
{
  size_t i, j;
  const char * *list;

  if (isSpecified("disable-dump-usage"))
    return;

  list = (const char**) malloc((size_t) registryCount * sizeof(const char*));

  for(i = 0; i < registryCount; ++i)
  {
    list[i] = registry[i].name;
  }

  qsort(list, (size_t) registryCount, sizeof(char*), compareEntry);

  if (!isSpecified("synthetic-registry") && !isSpecified("reduce-dump-usage"))
    printf("\nRegistry dump started (reduce verbosity with reduce-dump-usage or hide with disable-dump-usage)\n");
  for(i = 0; i < registryCount; ++i)
  {
    RegistryEntry * entry = Registry_getEntry(list[i]);
    if (entry->usage > 0)
    {
      if (isSpecified("synthetic-registry"))
      {
        if (isSpecified("auto-eval"))
          printf("##AUISFGWLOSP %s %s PSOLWGFSIUA##\n", entry->name, entry->file);
        else
          printf("%s %s\n", entry->name, entry->file);
      }
      else
      {
        if (!isSpecified("reduce-dump-usage"))
          printf("    Function %s has been used %ld times\n", entry->name, entry->usage);
      }
    }
  }
  if (!isSpecified("synthetic-registry") && !isSpecified("reduce-dump-usage"))
    printf("Registry dump done\n\n");

  if (!isSpecified("synthetic-registry"))
    printf("Registry dump by file name (hide with disable-dump-usage)\n");
  for(i = 0; i < registryCount; ++i)
  {
    int count = 0;
    if (registry[i].displayed == 0)
    {
      for(j = i; j < registryCount; ++j)
      {
        if (strcmp(registry[i].file, registry[j].file) == 0)
        {
          registry[j].displayed = 1;
          count++;
        }
      }
      if (!isSpecified("synthetic-registry"))
        printf("    File %s : %d functions remaining\n", registry[i].file, count);
    }
  }
  if (!isSpecified("synthetic-registry"))
    printf("Registry dump by file name done\n\n");

  free(list);

  if (isSpecified("auto-eval"))
    printf("##AUISFGWLOSP REGISTRY DONE PSOLWGFSIUA##");
  free(registry);
}

long int Registry_getUsage(const char * name) {
  RegistryEntry * entry = Registry_getEntry(name);
  fprintf(stderr,"Unkonwn function %s\n", name);
  if (entry == NULL)
    return 0;
  else
    return entry->usage;
}
