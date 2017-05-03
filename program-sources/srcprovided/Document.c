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

#include <Document.h>
#include <DocumentUtil.h>
#include <DocumentRowList.h>
#include <CustomerRecord.h>

/** Initialize a document
 * @param document a pointer to a document
 * @warning An initialized document must be finalized by Document_finalize() to free all resources
 */
void provided_Document_init(Document * document)
{
  REGISTRY_USINGFUNCTION;

  CustomerRecord_init(&document->customer);
  document->editDate = duplicateString("");
  document->expiryDate = duplicateString("");
  document->docNumber = duplicateString("");
  document->object = duplicateString("");
  document->operator = duplicateString("");
  DocumentRowList_init(&document->rows);
  document->typeDocument = QUOTATION;
}

/** Finalize a document
 * @param document the document to finalize
 * @warning document must have been initialized
 */
void provided_Document_finalize(Document * document)
{
  REGISTRY_USINGFUNCTION;

  CustomerRecord_finalize(&document->customer);
  free(document->editDate);
  free(document->expiryDate);
  free(document->docNumber);
  free(document->object);
  free(document->operator);
  DocumentRowList_finalize(&document->rows);
}

/** Save the content of a document to a file
 * @param document the document
 * @param filename the file name
 * @warning document must have been initialized
 */
void provided_Document_saveToFile(Document * document, const char * filename)
{
  DocumentRow * cur;
  int count;
  FILE * fichier;

  REGISTRY_USINGFUNCTION;

  count = DocumentRowList_getRowCount(document->rows);
  fichier = fopen(filename, "wb");

  CustomerRecord_write(&document->customer, fichier);
  writeString(document->docNumber, fichier);
  writeString(document->editDate, fichier);
  writeString(document->object, fichier);
  writeString(document->operator, fichier);
  writeString(document->expiryDate, fichier);

  if (fwrite(&document->typeDocument, sizeof(document->typeDocument), 1, fichier) != 1)
    fatalError("Document_SaveToFile");

  if (fwrite(&count, sizeof(count), 1, fichier) != 1)
    fatalError("Document_SaveToFile");

  cur = document->rows;
  while (cur != NULL)
  {
    DocumentRow_writeRow(cur, fichier);
    cur = cur->next;
  }

  fclose(fichier);
}

/** Load the content of a document from a file
 * @param document the document to fill
 * @param filename the file name
 * @warning document must have been initialized
 */
void provided_Document_loadFromFile(Document * document, const char * filename)
{
  int count, i;
  FILE * fichier;

  REGISTRY_USINGFUNCTION;

  fichier = fopen(filename, "rb");

  CustomerRecord_read(&document->customer, fichier);
  free(document->docNumber);
  document->docNumber = readString(fichier);
  free(document->editDate);
  document->editDate = readString(fichier);
  free(document->object);
  document->object = readString(fichier);
  free(document->operator);
  document->operator = readString(fichier);
  free(document->expiryDate);
  document->expiryDate = readString(fichier);

  if (fread(&document->typeDocument, sizeof(document->typeDocument), 1, fichier) != 1)
    fatalError("Document_LoadFromFile");

  if (fread(&count, sizeof(count), 1, fichier) != 1)
    fatalError("Document_LoadFromFile");

  DocumentRowList_finalize(&document->rows);
  for(i = 0; i < count; ++i)
    DocumentRowList_pushBack(&document->rows, DocumentRow_readRow(fichier));

  (void) fclose(fichier);
}

