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

#include <DocumentRowList.h>
#include <DocumentUtil.h>

/** Initialize a row
 * @param row the row
 * @warning an initialized row must be finalized by DocumentRow_finalize() to free all resources
 */
void provided_DocumentRow_init(DocumentRow * row)
{
  REGISTRY_USINGFUNCTION;

  row->code = duplicateString("");
  row->designation = duplicateString("");
  row->quantity = 0;
  row->unity = duplicateString("");
  row->basePrice = 0;
  row->sellingPrice = 0;
  row->discount = 0;
  row->rateOfVAT = 0;
  row->next = NULL;
}

/** Finalize a row
 * @param row the row
 * @warning document must have been initialized
 */
void provided_DocumentRow_finalize(DocumentRow * row)
{
  REGISTRY_USINGFUNCTION;

  free(row->code);
  free(row->designation);
  free(row->unity);
}

/** Create a new row on the heap and initialize it
 * @return the new row
 */
DocumentRow * provided_DocumentRow_create(void)
{
  DocumentRow * row;

  REGISTRY_USINGFUNCTION;

  row = (DocumentRow *) malloc(sizeof(DocumentRow));
  if (row == NULL)
    fatalError("DocumentRow_Create");
  DocumentRow_init(row);
  return row;
}

/** Finalize and destroy a row previously created on the heap
 * @param row the row
 */
void provided_DocumentRow_destroy(DocumentRow * row)
{
  REGISTRY_USINGFUNCTION;

  DocumentRow_finalize(row);
  free(row);
}

/** Initialize a list of rows
 * @param list the address of the pointer on the first cell of the list
 */
void provided_DocumentRowList_init(DocumentRow ** list)
{
  REGISTRY_USINGFUNCTION;

  *list = NULL;
}

/** Finalize a list of rows
 * @param list the address of the pointer on the first cell of the list
 * @note Each row of the list are destroyer using DocumentRow_destroy()
 */
void provided_DocumentRowList_finalize(DocumentRow ** list)
{
  REGISTRY_USINGFUNCTION;

  while (*list != NULL)
  {
    DocumentRowList_removeRow(list, *list);
  }
}

/** Get a pointer on the rowIndex-th row of the list
 * @param list the pointer on the first cell of the list
 * @param rowIndex the index of the requested row
 * @return a pointer on the rowIndex-th row of the list or NULL if the list contains less rows than the requested one
 */
DocumentRow * provided_DocumentRowList_get(DocumentRow * list, int rowIndex)
{
  DocumentRow * cur;
  int i;

  REGISTRY_USINGFUNCTION;

  cur = list;
  i = 0;

  while (cur != NULL && i != rowIndex)
  {
    cur = cur->next;
    i++;
  }
  return cur;
}

/**
 * Get the number of rows in the list
 * @param list the pointer on the first cell of the list
 */
int provided_DocumentRowList_getRowCount(DocumentRow * list)
{
  int count = 0;

  REGISTRY_USINGFUNCTION;

  while (list != NULL)
  {
    count++;
    list = list->next;
  }
  return count;
}

/** Add a row at the end of the list
 * @param list the address of the pointer on the first cell of the list
 * @param row the row to add
 */
void provided_DocumentRowList_pushBack(DocumentRow ** list, DocumentRow * row)
{
  REGISTRY_USINGFUNCTION;

  if (*list == NULL)
    *list = row;
  else
  {
    DocumentRow * cur = *list;
    while (cur->next != NULL)
      cur = cur->next;
    cur->next = row;
  }
}

/** Insert a row before a given row
 * @param list the address of the pointer on the first cell of the list
 * @param position a pointer on the positioning row
 * @param row the row to insert
 */
void provided_DocumentRowList_insertBefore(DocumentRow ** list, DocumentRow * position, DocumentRow * row)
{
  REGISTRY_USINGFUNCTION;

  if (position != NULL)
  {
    if (position == *list)
    {
      row->next = position;
      *list = row;
    }
    else
    {
      DocumentRow * previous = *list;
      while (previous != NULL && previous->next != position)
        previous = previous->next;
      if (previous != NULL)
      {
        row->next = previous->next;
        previous->next = row;
      }
    }
  }
}

/** Insert a row after a given row
 * @param list the address of the pointer on the first cell of the list
 * @param position a pointer on the positioning row
 * @param row the row to insert
 */
void provided_DocumentRowList_insertAfter(DocumentRow ** UNUSED(list), DocumentRow * position, DocumentRow * row)
{
  REGISTRY_USINGFUNCTION;

  row->next = position->next;
  position->next = row;
}

/** Remove a row from the list
 * @param list the address of the pointer on the first cell of the list
 * @param position the row to remove
 */
void provided_DocumentRowList_removeRow(DocumentRow ** list, DocumentRow * position)
{
  REGISTRY_USINGFUNCTION;

  if (*list == position)
  {
    *list = position->next;
    DocumentRow_destroy(position);
  }
  else
  {
    DocumentRow * previous = *list;
    while (previous != NULL && previous->next != position)
      previous = previous->next;
    if (previous != NULL)
      previous->next = position->next;
    DocumentRow_destroy(position);
  }
}

/** Write a row in a binary file
 * @param row the row
 * @param file the opened file
 */
void provided_DocumentRow_writeRow(DocumentRow * row, FILE * file)
{
  REGISTRY_USINGFUNCTION;

  writeString(row->code, file);
  writeString(row->designation, file);
  if (fwrite(&row->quantity, sizeof(row->quantity), 1, file) != 1)
    fatalError("writeRow");
  writeString(row->unity, file);
  if (fwrite(&row->basePrice, sizeof(row->basePrice), 1, file) != 1)
    fatalError("writeRow");
  if (fwrite(&row->sellingPrice, sizeof(row->sellingPrice), 1, file) != 1)
    fatalError("writeRow");
  if (fwrite(&row->discount, sizeof(row->discount), 1, file) != 1)
    fatalError("writeRow");
  if (fwrite(&row->rateOfVAT, sizeof(row->rateOfVAT), 1, file) != 1)
    fatalError("writeRow");
}

/** Read a row from a file
 * @param file the opened file
 * @return a new row created on the heap filled with the data
 */
DocumentRow * provided_DocumentRow_readRow(FILE * fichier)
{
  DocumentRow * row;

  REGISTRY_USINGFUNCTION;

  row = DocumentRow_create();
  free(row->code);
  row->code = readString(fichier);
  free(row->designation);
  row->designation = readString(fichier);
  if (fread(&row->quantity, sizeof(row->quantity), 1, fichier) != 1)
    fatalError("readRow");
  free(row->unity);
  row->unity = readString(fichier);
  if (fread(&row->basePrice, sizeof(row->basePrice), 1, fichier) != 1)
    fatalError("readRow");
  if (fread(&row->sellingPrice, sizeof(row->sellingPrice), 1, fichier) != 1)
    fatalError("readRow");
  if (fread(&row->discount, sizeof(row->discount), 1, fichier) != 1)
    fatalError("readRow");
  if (fread(&row->rateOfVAT, sizeof(row->rateOfVAT), 1, fichier) != 1)
    fatalError("readRow");
  return row;
}
