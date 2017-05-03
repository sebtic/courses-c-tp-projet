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

#include <CustomerRecord.h>

/** Static function to set the name field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void provided_CustomerRecord_setValue_name(CustomerRecord * record, const char * value)
{
  REGISTRY_USINGFUNCTION;

  copyStringWithLength(record->name, value, CUSTOMERRECORD_NAME_SIZE);
}

/** Static function to set the address field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void provided_CustomerRecord_setValue_address(CustomerRecord * record, const char * value)
{
  REGISTRY_USINGFUNCTION;

  copyStringWithLength(record->address, value, CUSTOMERRECORD_ADDRESS_SIZE);
}

/** Static function to set the postalCode field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void provided_CustomerRecord_setValue_postalCode(CustomerRecord * record, const char * value)
{
  REGISTRY_USINGFUNCTION;

  copyStringWithLength(record->postalCode, value, CUSTOMERRECORD_POSTALCODE_SIZE);
}

/** Static function to set the town field from a string
 * @param record a pointer to the record
 * @param value the value
 */
void provided_CustomerRecord_setValue_town(CustomerRecord * record, const char * value)
{
  REGISTRY_USINGFUNCTION;

  copyStringWithLength(record->town, value, CUSTOMERRECORD_TOWN_SIZE);
}

/** Static function which create a copy string on the head of the name field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * provided_CustomerRecord_getValue_name(CustomerRecord * record)
{
  REGISTRY_USINGFUNCTION;

  return duplicateString(record->name);
}

/** Static function which create a copy string on the head of the address field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * provided_CustomerRecord_getValue_address(CustomerRecord * record)
{
  REGISTRY_USINGFUNCTION;

  return duplicateString(record->address);
}

/** Static function which create a copy string on the head of the postalCode field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * provided_CustomerRecord_getValue_postalCode(CustomerRecord * record)
{
  REGISTRY_USINGFUNCTION;

  return duplicateString(record->postalCode);
}

/** Static function which create a copy string on the head of the town field value
 * @param record a pointer to the record
 * @return the new string
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 */
char * provided_CustomerRecord_getValue_town(CustomerRecord * record)
{
  REGISTRY_USINGFUNCTION;

  return duplicateString(record->town);
}

static void zeroTab(char * tab, int size)
{
  while (size > 0)
  {
    *tab = '\0';
    tab++;
    size--;
  }
}

/** Initialize a record
 * @param[in] record a pointer to a record to initialize
 * @warning every initialized record must be finalized with CustomerRecord_finalize()
 * @relates CustomerRecord
 */
void provided_CustomerRecord_init(CustomerRecord * record)
{
  REGISTRY_USINGFUNCTION;

  zeroTab(record->name, CUSTOMERRECORD_NAME_SIZE);
  zeroTab(record->address, CUSTOMERRECORD_ADDRESS_SIZE);
  zeroTab(record->postalCode, CUSTOMERRECORD_POSTALCODE_SIZE);
  zeroTab(record->town, CUSTOMERRECORD_TOWN_SIZE);
}

/** Finalize a record.
 * @param[in] record a pointer to a record to finalize
 * @relates CustomerRecord
 */
void provided_CustomerRecord_finalize(CustomerRecord * UNUSED( record))
{
  REGISTRY_USINGFUNCTION;
}

/** Read a record from a file
 * @param record a pointer to an initialized record on which to store data
 * @param file the file from which the data are read
 * @relates CustomerRecord
 */
void provided_CustomerRecord_read(CustomerRecord * record, FILE * file)
{
  REGISTRY_USINGFUNCTION;

  if (fread(record->name, CUSTOMERRECORD_NAME_SIZE, 1, file) != 1)
    fatalError("CustomerRecord_Read: fread");

  if (fread(record->address, CUSTOMERRECORD_ADDRESS_SIZE, 1, file) != 1)
    fatalError("CustomerRecord_Read: fread");

  if (fread(record->postalCode, CUSTOMERRECORD_POSTALCODE_SIZE, 1, file) != 1)
    fatalError("CustomerRecord_Read: fread");

  if (fread(record->town, CUSTOMERRECORD_TOWN_SIZE, 1, file) != 1)
    fatalError("CustomerRecord_Read: fread");
}

/** Write a record to a file
 * @param record a pointer to a record
 * @param file the file to which the data are written
 * @relates CustomerRecord
 */
void provided_CustomerRecord_write(CustomerRecord * record, FILE * file)
{
  REGISTRY_USINGFUNCTION;

  if (fwrite(record->name, CUSTOMERRECORD_NAME_SIZE, 1, file) != 1)
    fatalError("CustomerRecord_Write: fwrite");

  if (fwrite(record->address, CUSTOMERRECORD_ADDRESS_SIZE, 1, file) != 1)
    fatalError("CustomerRecord_Write: fwrite");

  if (fwrite(record->postalCode, CUSTOMERRECORD_POSTALCODE_SIZE, 1, file) != 1)
    fatalError("CustomerRecord_Write: fwrite");

  if (fwrite(record->town, CUSTOMERRECORD_TOWN_SIZE, 1, file) != 1)
    fatalError("CustomerRecord_Write: fwrite");
}
