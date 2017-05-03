/**
 * Copyright 2111 Sébastien Aupetit <sebtic@projectsforge.org>
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
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

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

CatalogDB * (*CatalogDB_create)(const char * filename);
CatalogDB * (*CatalogDB_open)(const char * filename);
CatalogDB * (*CatalogDB_openOrCreate)(const char * filename);
void (*CatalogDB_close)(CatalogDB * catalogDB);
int (*CatalogDB_getRecordCount)(CatalogDB * catalogDB);
void (*CatalogDB_appendRecord)(CatalogDB * catalogDB, CatalogRecord *record);
void (*CatalogDB_insertRecord)(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record);
void (*CatalogDB_removeRecord)(CatalogDB * catalogDB, int recordIndex);
void (*CatalogDB_readRecord)(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record);
void (*CatalogDB_writeRecord)(CatalogDB * catalogDB, int recordIndex, CatalogRecord * record);

int (*CatalogRecord_isValueValid_code)(const char * value);
int (*CatalogRecord_isValueValid_positiveNumber)(const char * value);
void (*CatalogRecord_setValue_code)(CatalogRecord * record, const char * value);
void (*CatalogRecord_setValue_designation)(CatalogRecord * record, const char * value);
void (*CatalogRecord_setValue_unity)(CatalogRecord * record, const char * value);
void (*CatalogRecord_setValue_basePrice)(CatalogRecord * record, const char * value);
void (*CatalogRecord_setValue_sellingPrice)(CatalogRecord * record, const char * value);
void (*CatalogRecord_setValue_rateOfVAT)(CatalogRecord * record, const char * value);
char * (*CatalogRecord_getValue_code)(CatalogRecord * record);
char * (*CatalogRecord_getValue_designation)(CatalogRecord * record);
char * (*CatalogRecord_getValue_unity)(CatalogRecord * record);
char * (*CatalogRecord_getValue_basePrice)(CatalogRecord * record);
char * (*CatalogRecord_getValue_sellingPrice)(CatalogRecord * record);
char * (*CatalogRecord_getValue_rateOfVAT)(CatalogRecord * record);
void (*CatalogRecord_init)(CatalogRecord * record);
void (*CatalogRecord_finalize)(CatalogRecord * record);
void (*CatalogRecord_read)(CatalogRecord * record, FILE * file);
void (*CatalogRecord_write)(CatalogRecord * record, FILE * file);

CustomerDB * (*CustomerDB_create)(const char * filename);
CustomerDB * (*CustomerDB_open)(const char * filename);
CustomerDB * (*CustomerDB_openOrCreate)(const char * filename);
void (*CustomerDB_close)(CustomerDB * customerDB);
int (*CustomerDB_getRecordCount)(CustomerDB * customerDB);
void (*CustomerDB_appendRecord)(CustomerDB * customerDB, CustomerRecord *record);
void (*CustomerDB_insertRecord)(CustomerDB * customerDB, int recordIndex, CustomerRecord * record);
void (*CustomerDB_removeRecord)(CustomerDB * customerDB, int recordIndex);
void (*CustomerDB_readRecord)(CustomerDB * customerDB, int recordIndex, CustomerRecord * record);
void (*CustomerDB_writeRecord)(CustomerDB * customerDB, int recordIndex, CustomerRecord * record);

void (*CustomerRecord_setValue_name)(CustomerRecord * record, const char * value);
void (*CustomerRecord_setValue_address)(CustomerRecord * record, const char * value);
void (*CustomerRecord_setValue_postalCode)(CustomerRecord * record, const char * value);
void (*CustomerRecord_setValue_town)(CustomerRecord * record, const char * value);
char * (*CustomerRecord_getValue_name)(CustomerRecord * record);
char * (*CustomerRecord_getValue_address)(CustomerRecord * record);
char * (*CustomerRecord_getValue_postalCode)(CustomerRecord * record);
char * (*CustomerRecord_getValue_town)(CustomerRecord * record);
void (*CustomerRecord_init)(CustomerRecord * record);
void (*CustomerRecord_finalize)(CustomerRecord * record);
void (*CustomerRecord_read)(CustomerRecord * record, FILE * file);
void (*CustomerRecord_write)(CustomerRecord * record, FILE * file);

Dictionary * (*Dictionary_create)(void);
void (*Dictionary_destroy)(Dictionary * dictionary);
DictionaryEntry * (*Dictionary_getEntry)(Dictionary * dictionary, const char * name);
void (*Dictionary_setStringEntry)(Dictionary * dictionary, const char * name, const char * value);
void (*Dictionary_setNumberEntry)(Dictionary * dictionary, const char * name, double value);
char * (*Dictionary_format)(Dictionary * dictionary, const char * format);
void (*Document_init)(Document * document);
void (*Document_finalize)(Document * document);
void (*Document_saveToFile)(Document * document, const char * filename);
void (*Document_loadFromFile)(Document * document, const char * filename);

void (*DocumentRow_init)(DocumentRow * row);
void (*DocumentRow_finalize)(DocumentRow * row);
DocumentRow * (*DocumentRow_create)(void);
void (*DocumentRow_destroy)(DocumentRow * row);
void (*DocumentRow_writeRow)(DocumentRow * row, FILE * file);
DocumentRow * (*DocumentRow_readRow)(FILE * fichier);

void (*DocumentRowList_init)(DocumentRow ** list);
void (*DocumentRowList_finalize)(DocumentRow ** list);
DocumentRow * (*DocumentRowList_get)(DocumentRow * list, int rowIndex);
int (*DocumentRowList_getRowCount)(DocumentRow * list);
void (*DocumentRowList_pushBack)(DocumentRow ** list, DocumentRow * row);
void (*DocumentRowList_insertBefore)(DocumentRow ** list, DocumentRow * position, DocumentRow * row);
void (*DocumentRowList_insertAfter)(DocumentRow ** list, DocumentRow * position, DocumentRow * row);
void (*DocumentRowList_removeRow)(DocumentRow ** list, DocumentRow * position);

char * (*computeDocumentNumber)(long id);
char * (*formatDate)(int day, int month, int year);
void (*writeString)(const char * str, FILE * file);
char * (*readString)(FILE * file);

void (*encrypt)(const char * key, char * str);
void (*decrypt)(const char * key, char * str);

size_t (*stringLength)(const char * str);
int (*compareString)(const char * str1, const char * str2);
int (*icaseCompareString)(const char * str1, const char * str2);
void (*copyString)(char * dest, const char * src);
void (*copyStringWithLength)(char * dest, const char * src, size_t destSize);
char * (*duplicateString)(const char * str);
int (*icaseStartWith)(const char * start, const char * str);
int (*icaseEndWith)(const char * end, const char * str);
char * (*concatenateString)(const char * str1, const char * str2);
char (*toLowerChar)(char c);
char (*toUpperChar)(char c);
void (*makeUpperCaseString)(char * str);
void (*makeLowerCaseString)(char * str);
const char * (*indexOfChar)(const char *str, char c);
const char * (*indexOfString)(const char *meule_de_foin, const char *aiguille);
char * (*subString)(const char * start, const char * end);
char * (*insertString)(const char * src, int insertPosition, const char * toBeInserted, int insertLength);

OperatorTable * (*OperatorTable_create)(void);
void (*OperatorTable_destroy)(OperatorTable * table);
OperatorTable * (*OperatorTable_loadFromFile)(const char * filename);
void (*OperatorTable_saveToFile)(OperatorTable * table, const char * filename);
int (*OperatorTable_getRecordCount)(OperatorTable * table);
const char * (*OperatorTable_getName)(OperatorTable * table, int recordIndex);
const char * (*OperatorTable_getPassword)(OperatorTable * table, int recordIndex);
int (*OperatorTable_findOperator)(OperatorTable * table, const char * name);
int (*OperatorTable_setOperator)(OperatorTable * table, const char * name, const char * password);
void (*OperatorTable_removeRecord)(OperatorTable * table, int recordIndex);

void (*PrintFormat_init)(PrintFormat * format);
void (*PrintFormat_finalize)(PrintFormat * format);
void (*PrintFormat_loadFromFile)(PrintFormat * format, const char * filename);

static int Config_argc;
static char ** Config_argv;

void Config_init(int argc, char ** argv)
{
  Config_argc = argc;
  Config_argv = argv;

}

/** Function which displays a message and halt the debugger before terminating the program
 * @param message the message to display
 * @param line the line from which the function was called
 * @param file the file from which the function was called
 */
void debugFatalError(const char * message, long line, const char * file)
{
  fprintf(stderr, "Fatal error: \"%s\" at line %ld in source file %s\n", message, line, file);
  if (!isSpecified("disable-debugtrap"))
  {
    (void) raise(SIGTRAP);
    __builtin_trap();
  }
  exit(1);
}

int isSpecified(const char * name)
{
  int i;
  for(i = 0; i < Config_argc; ++i)
  {
    if (strcmp(Config_argv[i], name) == 0)
    {
      return 1;
    }
  }
  return 0;
}

void setupOverridable(void)
{
  /* default to user implementation */
  CatalogDB_create = user_CatalogDB_create;
  CatalogDB_open = user_CatalogDB_open;
  CatalogDB_openOrCreate = user_CatalogDB_openOrCreate;
  CatalogDB_close = user_CatalogDB_close;
  CatalogDB_getRecordCount = user_CatalogDB_getRecordCount;
  CatalogDB_appendRecord = user_CatalogDB_appendRecord;
  CatalogDB_insertRecord = user_CatalogDB_insertRecord;
  CatalogDB_removeRecord = user_CatalogDB_removeRecord;
  CatalogDB_readRecord = user_CatalogDB_readRecord;
  CatalogDB_writeRecord = user_CatalogDB_writeRecord;
  CatalogRecord_isValueValid_code = user_CatalogRecord_isValueValid_code;
  CatalogRecord_isValueValid_positiveNumber = user_CatalogRecord_isValueValid_positiveNumber;
  CatalogRecord_setValue_code = user_CatalogRecord_setValue_code;
  CatalogRecord_setValue_designation = user_CatalogRecord_setValue_designation;
  CatalogRecord_setValue_unity = user_CatalogRecord_setValue_unity;
  CatalogRecord_setValue_basePrice = user_CatalogRecord_setValue_basePrice;
  CatalogRecord_setValue_sellingPrice = user_CatalogRecord_setValue_sellingPrice;
  CatalogRecord_setValue_rateOfVAT = user_CatalogRecord_setValue_rateOfVAT;
  CatalogRecord_getValue_code = user_CatalogRecord_getValue_code;
  CatalogRecord_getValue_designation = user_CatalogRecord_getValue_designation;
  CatalogRecord_getValue_unity = user_CatalogRecord_getValue_unity;
  CatalogRecord_getValue_basePrice = user_CatalogRecord_getValue_basePrice;
  CatalogRecord_getValue_sellingPrice = user_CatalogRecord_getValue_sellingPrice;
  CatalogRecord_getValue_rateOfVAT = user_CatalogRecord_getValue_rateOfVAT;
  CatalogRecord_init = user_CatalogRecord_init;
  CatalogRecord_finalize = user_CatalogRecord_finalize;
  CatalogRecord_read = user_CatalogRecord_read;
  CatalogRecord_write = user_CatalogRecord_write;
  CustomerDB_create = user_CustomerDB_create;
  CustomerDB_open = user_CustomerDB_open;
  CustomerDB_openOrCreate = user_CustomerDB_openOrCreate;
  CustomerDB_close = user_CustomerDB_close;
  CustomerDB_getRecordCount = user_CustomerDB_getRecordCount;
  CustomerDB_appendRecord = user_CustomerDB_appendRecord;
  CustomerDB_insertRecord = user_CustomerDB_insertRecord;
  CustomerDB_removeRecord = user_CustomerDB_removeRecord;
  CustomerDB_readRecord = user_CustomerDB_readRecord;
  CustomerDB_writeRecord = user_CustomerDB_writeRecord;
  CustomerRecord_setValue_name = user_CustomerRecord_setValue_name;
  CustomerRecord_setValue_address = user_CustomerRecord_setValue_address;
  CustomerRecord_setValue_postalCode = user_CustomerRecord_setValue_postalCode;
  CustomerRecord_setValue_town = user_CustomerRecord_setValue_town;
  CustomerRecord_getValue_name = user_CustomerRecord_getValue_name;
  CustomerRecord_getValue_address = user_CustomerRecord_getValue_address;
  CustomerRecord_getValue_postalCode = user_CustomerRecord_getValue_postalCode;
  CustomerRecord_getValue_town = user_CustomerRecord_getValue_town;
  CustomerRecord_init = user_CustomerRecord_init;
  CustomerRecord_finalize = user_CustomerRecord_finalize;
  CustomerRecord_read = user_CustomerRecord_read;
  CustomerRecord_write = user_CustomerRecord_write;
  Dictionary_create = user_Dictionary_create;
  Dictionary_destroy = user_Dictionary_destroy;
  Dictionary_getEntry = user_Dictionary_getEntry;
  Dictionary_setStringEntry = user_Dictionary_setStringEntry;
  Dictionary_setNumberEntry = user_Dictionary_setNumberEntry;
  Dictionary_format = user_Dictionary_format;
  Document_init = user_Document_init;
  Document_finalize = user_Document_finalize;
  Document_saveToFile = user_Document_saveToFile;
  Document_loadFromFile = user_Document_loadFromFile;
  DocumentRow_init = user_DocumentRow_init;
  DocumentRow_finalize = user_DocumentRow_finalize;
  DocumentRow_create = user_DocumentRow_create;
  DocumentRow_destroy = user_DocumentRow_destroy;
  DocumentRowList_init = user_DocumentRowList_init;
  DocumentRowList_finalize = user_DocumentRowList_finalize;
  DocumentRowList_get = user_DocumentRowList_get;
  DocumentRowList_getRowCount = user_DocumentRowList_getRowCount;
  DocumentRowList_pushBack = user_DocumentRowList_pushBack;
  DocumentRowList_insertBefore = user_DocumentRowList_insertBefore;
  DocumentRowList_insertAfter = user_DocumentRowList_insertAfter;
  DocumentRowList_removeRow = user_DocumentRowList_removeRow;
  DocumentRow_writeRow = user_DocumentRow_writeRow;
  DocumentRow_readRow = user_DocumentRow_readRow;
  computeDocumentNumber = user_computeDocumentNumber;
  formatDate = user_formatDate;
  writeString = user_writeString;
  readString = user_readString;
  encrypt = user_encrypt;
  decrypt = user_decrypt;
  stringLength = user_stringLength;
  compareString = user_compareString;
  icaseCompareString = user_icaseCompareString;
  copyString = user_copyString;
  copyStringWithLength = user_copyStringWithLength;
  duplicateString = user_duplicateString;
  icaseStartWith = user_icaseStartWith;
  icaseEndWith = user_icaseEndWith;
  concatenateString = user_concatenateString;
  toLowerChar = user_toLowerChar;
  toUpperChar = user_toUpperChar;
  makeUpperCaseString = user_makeUpperCaseString;
  makeLowerCaseString = user_makeLowerCaseString;
  indexOfChar = user_indexOfChar;
  indexOfString = user_indexOfString;
  subString = user_subString;
  insertString = user_insertString;
  OperatorTable_create = user_OperatorTable_create;
  OperatorTable_destroy = user_OperatorTable_destroy;
  OperatorTable_loadFromFile = user_OperatorTable_loadFromFile;
  OperatorTable_saveToFile = user_OperatorTable_saveToFile;
  OperatorTable_getRecordCount = user_OperatorTable_getRecordCount;
  OperatorTable_getName = user_OperatorTable_getName;
  OperatorTable_getPassword = user_OperatorTable_getPassword;
  OperatorTable_findOperator = user_OperatorTable_findOperator;
  OperatorTable_setOperator = user_OperatorTable_setOperator;
  OperatorTable_removeRecord = user_OperatorTable_removeRecord;
  PrintFormat_init = user_PrintFormat_init;
  PrintFormat_finalize = user_PrintFormat_finalize;
  PrintFormat_loadFromFile = user_PrintFormat_loadFromFile;

  if (isSpecified("disable-CatalogDB") == 1 || isSpecified("disable-CatalogDB_create") == 1)
    CatalogDB_create = provided_CatalogDB_create;
  if (isSpecified("disable-CatalogDB") == 1 || isSpecified("disable-CatalogDB_open") == 1)
    CatalogDB_open = provided_CatalogDB_open;
  if (isSpecified("disable-CatalogDB") == 1 || isSpecified("disable-CatalogDB_openOrCreate") == 1)
    CatalogDB_openOrCreate = provided_CatalogDB_openOrCreate;
  if (isSpecified("disable-CatalogDB") == 1 || isSpecified("disable-CatalogDB_close") == 1)
    CatalogDB_close = provided_CatalogDB_close;
  if (isSpecified("disable-CatalogDB") == 1 || isSpecified("disable-CatalogDB_getRecordCount") == 1)
    CatalogDB_getRecordCount = provided_CatalogDB_getRecordCount;
  if (isSpecified("disable-CatalogDB") == 1 || isSpecified("disable-CatalogDB_appendRecord") == 1)
    CatalogDB_appendRecord = provided_CatalogDB_appendRecord;
  if (isSpecified("disable-CatalogDB") == 1 || isSpecified("disable-CatalogDB_insertRecord") == 1)
    CatalogDB_insertRecord = provided_CatalogDB_insertRecord;
  if (isSpecified("disable-CatalogDB") == 1 || isSpecified("disable-CatalogDB_removeRecord") == 1)
    CatalogDB_removeRecord = provided_CatalogDB_removeRecord;
  if (isSpecified("disable-CatalogDB") == 1 || isSpecified("disable-CatalogDB_readRecord") == 1)
    CatalogDB_readRecord = provided_CatalogDB_readRecord;
  if (isSpecified("disable-CatalogDB") == 1 || isSpecified("disable-CatalogDB_writeRecord") == 1)
    CatalogDB_writeRecord = provided_CatalogDB_writeRecord;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_isValueValid_code") == 1)
    CatalogRecord_isValueValid_code = provided_CatalogRecord_isValueValid_code;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_isValueValid_positiveNumber") == 1)
    CatalogRecord_isValueValid_positiveNumber = provided_CatalogRecord_isValueValid_positiveNumber;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_setValue_code") == 1)
    CatalogRecord_setValue_code = provided_CatalogRecord_setValue_code;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_setValue_designation") == 1)
    CatalogRecord_setValue_designation = provided_CatalogRecord_setValue_designation;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_setValue_unity") == 1)
    CatalogRecord_setValue_unity = provided_CatalogRecord_setValue_unity;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_setValue_basePrice") == 1)
    CatalogRecord_setValue_basePrice = provided_CatalogRecord_setValue_basePrice;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_setValue_sellingPrice") == 1)
    CatalogRecord_setValue_sellingPrice = provided_CatalogRecord_setValue_sellingPrice;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_setValue_rateOfVAT") == 1)
    CatalogRecord_setValue_rateOfVAT = provided_CatalogRecord_setValue_rateOfVAT;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_getValue_code") == 1)
    CatalogRecord_getValue_code = provided_CatalogRecord_getValue_code;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_getValue_designation") == 1)
    CatalogRecord_getValue_designation = provided_CatalogRecord_getValue_designation;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_getValue_unity") == 1)
    CatalogRecord_getValue_unity = provided_CatalogRecord_getValue_unity;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_getValue_basePrice") == 1)
    CatalogRecord_getValue_basePrice = provided_CatalogRecord_getValue_basePrice;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_getValue_sellingPrice") == 1)
    CatalogRecord_getValue_sellingPrice = provided_CatalogRecord_getValue_sellingPrice;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_getValue_rateOfVAT") == 1)
    CatalogRecord_getValue_rateOfVAT = provided_CatalogRecord_getValue_rateOfVAT;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_init") == 1)
    CatalogRecord_init = provided_CatalogRecord_init;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_finalize") == 1)
    CatalogRecord_finalize = provided_CatalogRecord_finalize;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_read") == 1)
    CatalogRecord_read = provided_CatalogRecord_read;
  if (isSpecified("disable-CatalogRecord") == 1 || isSpecified("disable-CatalogRecord_write") == 1)
    CatalogRecord_write = provided_CatalogRecord_write;
  if (isSpecified("disable-CustomerDB") == 1 || isSpecified("disable-CustomerDB_create") == 1)
    CustomerDB_create = provided_CustomerDB_create;
  if (isSpecified("disable-CustomerDB") == 1 || isSpecified("disable-CustomerDB_open") == 1)
    CustomerDB_open = provided_CustomerDB_open;
  if (isSpecified("disable-CustomerDB") == 1 || isSpecified("disable-CustomerDB_openOrCreate") == 1)
    CustomerDB_openOrCreate = provided_CustomerDB_openOrCreate;
  if (isSpecified("disable-CustomerDB") == 1 || isSpecified("disable-CustomerDB_close") == 1)
    CustomerDB_close = provided_CustomerDB_close;
  if (isSpecified("disable-CustomerDB") == 1 || isSpecified("disable-CustomerDB_getRecordCount") == 1)
    CustomerDB_getRecordCount = provided_CustomerDB_getRecordCount;
  if (isSpecified("disable-CustomerDB") == 1 || isSpecified("disable-CustomerDB_appendRecord") == 1)
    CustomerDB_appendRecord = provided_CustomerDB_appendRecord;
  if (isSpecified("disable-CustomerDB") == 1 || isSpecified("disable-CustomerDB_insertRecord") == 1)
    CustomerDB_insertRecord = provided_CustomerDB_insertRecord;
  if (isSpecified("disable-CustomerDB") == 1 || isSpecified("disable-CustomerDB_removeRecord") == 1)
    CustomerDB_removeRecord = provided_CustomerDB_removeRecord;
  if (isSpecified("disable-CustomerDB") == 1 || isSpecified("disable-CustomerDB_readRecord") == 1)
    CustomerDB_readRecord = provided_CustomerDB_readRecord;
  if (isSpecified("disable-CustomerDB") == 1 || isSpecified("disable-CustomerDB_writeRecord") == 1)
    CustomerDB_writeRecord = provided_CustomerDB_writeRecord;
  if (isSpecified("disable-CustomerRecord") == 1 || isSpecified("disable-CustomerRecord_setValue_name") == 1)
    CustomerRecord_setValue_name = provided_CustomerRecord_setValue_name;
  if (isSpecified("disable-CustomerRecord") == 1 || isSpecified("disable-CustomerRecord_setValue_address") == 1)
    CustomerRecord_setValue_address = provided_CustomerRecord_setValue_address;
  if (isSpecified("disable-CustomerRecord") == 1 || isSpecified("disable-CustomerRecord_setValue_postalCode") == 1)
    CustomerRecord_setValue_postalCode = provided_CustomerRecord_setValue_postalCode;
  if (isSpecified("disable-CustomerRecord") == 1 || isSpecified("disable-CustomerRecord_setValue_town") == 1)
    CustomerRecord_setValue_town = provided_CustomerRecord_setValue_town;
  if (isSpecified("disable-CustomerRecord") == 1 || isSpecified("disable-CustomerRecord_getValue_name") == 1)
    CustomerRecord_getValue_name = provided_CustomerRecord_getValue_name;
  if (isSpecified("disable-CustomerRecord") == 1 || isSpecified("disable-CustomerRecord_getValue_address") == 1)
    CustomerRecord_getValue_address = provided_CustomerRecord_getValue_address;
  if (isSpecified("disable-CustomerRecord") == 1 || isSpecified("disable-CustomerRecord_getValue_postalCode") == 1)
    CustomerRecord_getValue_postalCode = provided_CustomerRecord_getValue_postalCode;
  if (isSpecified("disable-CustomerRecord") == 1 || isSpecified("disable-CustomerRecord_getValue_town") == 1)
    CustomerRecord_getValue_town = provided_CustomerRecord_getValue_town;
  if (isSpecified("disable-CustomerRecord") == 1 || isSpecified("disable-CustomerRecord_init") == 1)
    CustomerRecord_init = provided_CustomerRecord_init;
  if (isSpecified("disable-CustomerRecord") == 1 || isSpecified("disable-CustomerRecord_finalize") == 1)
    CustomerRecord_finalize = provided_CustomerRecord_finalize;
  if (isSpecified("disable-CustomerRecord") == 1 || isSpecified("disable-CustomerRecord_read") == 1)
    CustomerRecord_read = provided_CustomerRecord_read;
  if (isSpecified("disable-CustomerRecord") == 1 || isSpecified("disable-CustomerRecord_write") == 1)
    CustomerRecord_write = provided_CustomerRecord_write;
  if (isSpecified("disable-Dictionary") == 1 || isSpecified("disable-Dictionary_create") == 1)
    Dictionary_create = provided_Dictionary_create;
  if (isSpecified("disable-Dictionary") == 1 || isSpecified("disable-Dictionary_destroy") == 1)
    Dictionary_destroy = provided_Dictionary_destroy;
  if (isSpecified("disable-Dictionary") == 1 || isSpecified("disable-Dictionary_getEntry") == 1)
    Dictionary_getEntry = provided_Dictionary_getEntry;
  if (isSpecified("disable-Dictionary") == 1 || isSpecified("disable-Dictionary_setStringEntry") == 1)
    Dictionary_setStringEntry = provided_Dictionary_setStringEntry;
  if (isSpecified("disable-Dictionary") == 1 || isSpecified("disable-Dictionary_setNumberEntry") == 1)
    Dictionary_setNumberEntry = provided_Dictionary_setNumberEntry;
  if (isSpecified("disable-Dictionary") == 1 || isSpecified("disable-Dictionary_format") == 1)
    Dictionary_format = provided_Dictionary_format;
  if (isSpecified("disable-Document") == 1 || isSpecified("disable-Document_init") == 1)
    Document_init = provided_Document_init;
  if (isSpecified("disable-Document") == 1 || isSpecified("disable-Document_finalize") == 1)
    Document_finalize = provided_Document_finalize;
  if (isSpecified("disable-Document") == 1 || isSpecified("disable-Document_saveToFile") == 1)
    Document_saveToFile = provided_Document_saveToFile;
  if (isSpecified("disable-Document") == 1 || isSpecified("disable-Document_loadFromFile") == 1)
    Document_loadFromFile = provided_Document_loadFromFile;
  if (isSpecified("disable-DocumentRowList") == 1 || isSpecified("disable-DocumentRow_init") == 1)
    DocumentRow_init = provided_DocumentRow_init;
  if (isSpecified("disable-DocumentRowList") == 1 || isSpecified("disable-DocumentRow_finalize") == 1)
    DocumentRow_finalize = provided_DocumentRow_finalize;
  if (isSpecified("disable-DocumentRowList") == 1 || isSpecified("disable-DocumentRow_create") == 1)
    DocumentRow_create = provided_DocumentRow_create;
  if (isSpecified("disable-DocumentRowList") == 1 || isSpecified("disable-DocumentRow_destroy") == 1)
    DocumentRow_destroy = provided_DocumentRow_destroy;
  if (isSpecified("disable-DocumentRowList") == 1 || isSpecified("disable-DocumentRowList_init") == 1)
    DocumentRowList_init = provided_DocumentRowList_init;
  if (isSpecified("disable-DocumentRowList") == 1 || isSpecified("disable-DocumentRowList_finalize") == 1)
    DocumentRowList_finalize = provided_DocumentRowList_finalize;
  if (isSpecified("disable-DocumentRowList") == 1 || isSpecified("disable-DocumentRowList_get") == 1)
    DocumentRowList_get = provided_DocumentRowList_get;
  if (isSpecified("disable-DocumentRowList") == 1 || isSpecified("disable-DocumentRowList_getRowCount") == 1)
    DocumentRowList_getRowCount = provided_DocumentRowList_getRowCount;
  if (isSpecified("disable-DocumentRowList") == 1 || isSpecified("disable-DocumentRowList_pushBack") == 1)
    DocumentRowList_pushBack = provided_DocumentRowList_pushBack;
  if (isSpecified("disable-DocumentRowList") == 1 || isSpecified("disable-DocumentRowList_insertBefore") == 1)
    DocumentRowList_insertBefore = provided_DocumentRowList_insertBefore;
  if (isSpecified("disable-DocumentRowList") == 1 || isSpecified("disable-DocumentRowList_insertAfter") == 1)
    DocumentRowList_insertAfter = provided_DocumentRowList_insertAfter;
  if (isSpecified("disable-DocumentRowList") == 1 || isSpecified("disable-DocumentRowList_removeRow") == 1)
    DocumentRowList_removeRow = provided_DocumentRowList_removeRow;
  if (isSpecified("disable-DocumentRowList") == 1 || isSpecified("disable-DocumentRow_writeRow") == 1)
    DocumentRow_writeRow = provided_DocumentRow_writeRow;
  if (isSpecified("disable-DocumentRowList") == 1 || isSpecified("disable-DocumentRow_readRow") == 1)
    DocumentRow_readRow = provided_DocumentRow_readRow;
  if (isSpecified("disable-DocumentUtil") == 1 || isSpecified("disable-computeDocumentNumber") == 1)
    computeDocumentNumber = provided_computeDocumentNumber;
  if (isSpecified("disable-DocumentUtil") == 1 || isSpecified("disable-formatDate") == 1)
    formatDate = provided_formatDate;
  if (isSpecified("disable-DocumentUtil") == 1 || isSpecified("disable-writeString") == 1)
    writeString = provided_writeString;
  if (isSpecified("disable-DocumentUtil") == 1 || isSpecified("disable-readString") == 1)
    readString = provided_readString;
  if (isSpecified("disable-EncryptDecrypt") == 1 || isSpecified("disable-encrypt") == 1)
    encrypt = provided_encrypt;
  if (isSpecified("disable-EncryptDecrypt") == 1 || isSpecified("disable-decrypt") == 1)
    decrypt = provided_decrypt;
  if (isSpecified("disable-MyString") == 1 || isSpecified("disable-toLowerChar") == 1)
    toLowerChar = provided_toLowerChar;
  if (isSpecified("disable-MyString") == 1 || isSpecified("disable-toUpperChar") == 1)
    toUpperChar = provided_toUpperChar;
  if (isSpecified("disable-MyString") == 1 || isSpecified("disable-compareString") == 1)
    compareString = provided_compareString;
  if (isSpecified("disable-MyString") == 1 || isSpecified("disable-icaseCompareString") == 1)
    icaseCompareString = provided_icaseCompareString;
  if (isSpecified("disable-MyString") == 1 || isSpecified("disable-stringLength") == 1)
    stringLength = provided_stringLength;
  if (isSpecified("disable-MyString") == 1 || isSpecified("disable-copyString") == 1)
    copyString = provided_copyString;
  if (isSpecified("disable-MyString") == 1 || isSpecified("disable-copyStringWithLength") == 1)
    copyStringWithLength = provided_copyStringWithLength;
  if (isSpecified("disable-MyString") == 1 || isSpecified("disable-duplicateString") == 1)
    duplicateString = provided_duplicateString;
  if (isSpecified("disable-MyString") == 1 || isSpecified("disable-icaseStartWith") == 1)
    icaseStartWith = provided_icaseStartWith;
  if (isSpecified("disable-MyString") == 1 || isSpecified("disable-icaseEndWith") == 1)
    icaseEndWith = provided_icaseEndWith;
  if (isSpecified("disable-MyString") == 1 || isSpecified("disable-concatenateString") == 1)
    concatenateString = provided_concatenateString;
  if (isSpecified("disable-MyString") == 1 || isSpecified("disable-indexOfChar") == 1)
    indexOfChar = provided_indexOfChar;
  if (isSpecified("disable-MyString") == 1 || isSpecified("disable-subString") == 1)
    subString = provided_subString;
  if (isSpecified("disable-MyString") == 1 || isSpecified("disable-indexOfString") == 1)
    indexOfString = provided_indexOfString;
  if (isSpecified("disable-MyString") == 1 || isSpecified("disable-makeUpperCaseString") == 1)
    makeUpperCaseString = provided_makeUpperCaseString;
  if (isSpecified("disable-MyString") == 1 || isSpecified("disable-makeLowerCaseString") == 1)
    makeLowerCaseString = provided_makeLowerCaseString;
  if (isSpecified("disable-MyString") == 1 || isSpecified("disable-insertString") == 1)
    insertString = provided_insertString;
  if (isSpecified("disable-OperatorTable") == 1 || isSpecified("disable-OperatorTable_create") == 1)
    OperatorTable_create = provided_OperatorTable_create;
  if (isSpecified("disable-OperatorTable") == 1 || isSpecified("disable-OperatorTable_destroy") == 1)
    OperatorTable_destroy = provided_OperatorTable_destroy;
  if (isSpecified("disable-OperatorTable") == 1 || isSpecified("disable-OperatorTable_loadFromFile") == 1)
    OperatorTable_loadFromFile = provided_OperatorTable_loadFromFile;
  if (isSpecified("disable-OperatorTable") == 1 || isSpecified("disable-OperatorTable_saveToFile") == 1)
    OperatorTable_saveToFile = provided_OperatorTable_saveToFile;
  if (isSpecified("disable-OperatorTable") == 1 || isSpecified("disable-OperatorTable_getRecordCount") == 1)
    OperatorTable_getRecordCount = provided_OperatorTable_getRecordCount;
  if (isSpecified("disable-OperatorTable") == 1 || isSpecified("disable-OperatorTable_getName") == 1)
    OperatorTable_getName = provided_OperatorTable_getName;
  if (isSpecified("disable-OperatorTable") == 1 || isSpecified("disable-OperatorTable_getPassword") == 1)
    OperatorTable_getPassword = provided_OperatorTable_getPassword;
  if (isSpecified("disable-OperatorTable") == 1 || isSpecified("disable-OperatorTable_findOperator") == 1)
    OperatorTable_findOperator = provided_OperatorTable_findOperator;
  if (isSpecified("disable-OperatorTable") == 1 || isSpecified("disable-OperatorTable_setOperator") == 1)
    OperatorTable_setOperator = provided_OperatorTable_setOperator;
  if (isSpecified("disable-OperatorTable") == 1 || isSpecified("disable-OperatorTable_removeRecord") == 1)
    OperatorTable_removeRecord = provided_OperatorTable_removeRecord;
  if (isSpecified("disable-PrintFormat") == 1 || isSpecified("disable-PrintFormat_init") == 1)
    PrintFormat_init = provided_PrintFormat_init;
  if (isSpecified("disable-PrintFormat") == 1 || isSpecified("disable-PrintFormat_finalize") == 1)
    PrintFormat_finalize = provided_PrintFormat_finalize;
  if (isSpecified("disable-PrintFormat") == 1 || isSpecified("disable-PrintFormat_loadFromFile") == 1)
    PrintFormat_loadFromFile = provided_PrintFormat_loadFromFile;
}
