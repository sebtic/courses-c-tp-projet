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

#include <OperatorTable.h>
#include <EncryptDecrypt.h>

/** The operator database file name */
const char * OPERATORDB_FILENAME = BASEPATH "/data/Operator.db";

/**
 * Create an empty table of operators.
 * @return the new table
 * @relates OperatorTable
 */
OperatorTable * provided_OperatorTable_create()
{
  OperatorTable * table;

  REGISTRY_USINGFUNCTION;

  table = (OperatorTable*) malloc(sizeof(OperatorTable));
  if (table != NULL)
  {
    table->recordCount = 0;
    table->records = NULL;
  }
  return table;
}

/** Free a table of operators.
 * @param table a pointer to the table to free
 * @relates OperatorTable
 */
void provided_OperatorTable_destroy(OperatorTable * table)
{
  int i;

  REGISTRY_USINGFUNCTION;

  if (table != NULL)
  {
    for(i = 0; i < table->recordCount; ++i)
    {
      free(table->records[i][0]);
      free(table->records[i][1]);
      free(table->records[i]);
    }
    free(table->records);
    free(table);
  }
}

/** Load a table of operators from a file.
 * @param filename the file name
 * @return the new table
 * @relates OperatorTable
 */
OperatorTable * provided_OperatorTable_loadFromFile(const char * filename)
{
  char buf[128];
  char namebuf[OPERATORTABLE_MAXNAMESIZE];
  char passwordbuf[OPERATORTABLE_MAXPASSWORDSIZE];
  OperatorTable * table;
  FILE * fichier;

  REGISTRY_USINGFUNCTION;

  table = OperatorTable_create();
  fichier = fopen(filename, "rt");
  if (fichier != NULL)
  {
    int i;
    int nbEnr;
    if (fgets(buf, 128, fichier) == NULL)
      fatalError("OperatorDB_LoadFromFile: fgets");

    nbEnr = atoi(buf);
    for(i = 0; i < nbEnr; ++i)
    {
      if (fgets(namebuf, OPERATORTABLE_MAXNAMESIZE, fichier) == NULL)
        fatalError("OperatorDB_LoadFromFile: fgets");
      if (fgets(passwordbuf, OPERATORTABLE_MAXPASSWORDSIZE, fichier) == NULL)
        fatalError("OperatorDB_LoadFromFile: fgets");
      /* suppression des sauts de lignes finaux */
      if (namebuf[stringLength(namebuf) - 1] == '\n')
        namebuf[stringLength(namebuf) - 1] = '\0';
      if (passwordbuf[stringLength(passwordbuf) - 1] == '\n')
        passwordbuf[stringLength(passwordbuf) - 1] = '\0';
      decrypt(OperatorCryptKey, namebuf);
      decrypt(OperatorCryptKey, passwordbuf);
      OperatorTable_setOperator(table, namebuf, passwordbuf);
    }
    fclose(fichier);
  }
  return table;
}

/** Save a table of operators to a file.
 * @param table the table of operators
 * @param filename the file name
 * @relates OperatorTable
 */
void provided_OperatorTable_saveToFile(OperatorTable * table, const char * filename)
{
  char namebuf[OPERATORTABLE_MAXNAMESIZE];
  char passwordbuf[OPERATORTABLE_MAXPASSWORDSIZE];
  int i;
  FILE * fichier;

  REGISTRY_USINGFUNCTION;

  fichier = fopen(filename, "wt");
  if (fichier != NULL)
  {
    fprintf(fichier, "%d\n", (int) table->recordCount);
    for(i = 0; i < OperatorTable_getRecordCount(table); ++i)
    {
      copyStringWithLength(namebuf, OperatorTable_getName(table, i),
      OPERATORTABLE_MAXNAMESIZE);
      copyStringWithLength(passwordbuf, OperatorTable_getPassword(table, i),
      OPERATORTABLE_MAXNAMESIZE);
      encrypt(OperatorCryptKey, namebuf);
      encrypt(OperatorCryptKey, passwordbuf);
      fprintf(fichier, "%s\n%s\n", namebuf, passwordbuf);
    }
    fclose(fichier);
  }
}

/** Get the number of records of a table of operators
 * @param table the table of operators
 * @return the number of records
 * @relates OperatorTable
 */
int provided_OperatorTable_getRecordCount(OperatorTable * table)
{
  REGISTRY_USINGFUNCTION;

  return table->recordCount;
}

/** Get the name of a record of a table of operators.
 * @param table the table of operators
 * @param recordIndex the record number
 * @return the name of the operator
 * @relates OperatorTable
 */
const char * provided_OperatorTable_getName(OperatorTable * table, int recordNum)
{
  REGISTRY_USINGFUNCTION;

  if (recordNum < 0 || recordNum > table->recordCount)
    fatalError("OperatorTable_GetName");
  return table->records[recordNum][0];
}

/** Get the password of a record of a table of operators.
 * @param table the table of operators
 * @param recordIndex the record index
 * @return the paswword of the operator
 * @relates OperatorTable
 */
const char * provided_OperatorTable_getPassword(OperatorTable * table, int recordNum)
{
  REGISTRY_USINGFUNCTION;

  if (recordNum < 0 || recordNum > table->recordCount)
    fatalError("OperatorTable_GetPassword");
  return table->records[recordNum][1];
}

/** Get the record index associated with an operator name.
 * @param table the table of operators
 * @param name the name of the operator
 * @return the index of the operator or -1 if the operator is not in the table
 * @relates OperatorTable
 */
int provided_OperatorTable_findOperator(OperatorTable * table, const char * name)
{
  int i;

  REGISTRY_USINGFUNCTION;
  for(i = 0; i < OperatorTable_getRecordCount(table); ++i)
    if (icaseCompareString(OperatorTable_getName(table, i), name) == 0)
      return i;
  return -1;
}

/** Define or change the password of an operator
 * @param table the table of operators
 * @param name the name of the operator
 * @param password the password of the operator
 * @return the index of the operator in the table
 * @relates OperatorTable
 */
int provided_OperatorTable_setOperator(OperatorTable * table, const char * name, const char * password)
{
  int i;

  REGISTRY_USINGFUNCTION;

  for(i = 0; i < OperatorTable_getRecordCount(table); ++i)
    if (icaseCompareString(OperatorTable_getName(table, i), name) == 0)
    {
      free(table->records[i][1]);
      table->records[i][1] = duplicateString(password);
      return i;
    }

  table->records = (char***) realloc(table->records, sizeof(char**) * ((unsigned int) table->recordCount + 1));
  table->records[table->recordCount] = (char**) malloc(sizeof(char*) * 2);
  table->records[table->recordCount][0] = duplicateString(name);
  table->records[table->recordCount][1] = duplicateString(password);
  table->recordCount++;
  return table->recordCount - 1;
}

/** Remove an operator from the table.
 * @param table the table of operators
 * @param recordIndex the index of the record to remove
 * @relates OperatorTable
 */
void provided_OperatorTable_removeRecord(OperatorTable * table, int recordIndex)
{
  int i;

  REGISTRY_USINGFUNCTION;

  free(table->records[recordIndex][0]);
  free(table->records[recordIndex][1]);
  free(table->records[recordIndex]);
  for(i = recordIndex + 1; i < table->recordCount; ++i)
  {
    table->records[i - 1] = table->records[i];
  }
  table->records = (char***) realloc(table->records, sizeof(char**) * ((unsigned int) table->recordCount - 1));
  table->recordCount--;
}

