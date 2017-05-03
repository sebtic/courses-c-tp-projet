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

#include <Dictionary.h>

/** Create an empty dictionary on the heap
 * @return a new dictionary
 * @warning the dictionary should be destroyed using Dictionary_destroy()
 */
Dictionary * provided_Dictionary_create(void)
{
  Dictionary * dictionary;

  REGISTRY_USINGFUNCTION;

  dictionary = (Dictionary*) malloc(sizeof(Dictionary));
  if (dictionary == NULL)
    fatalError("Dictionary_Create");
  dictionary->count = 0;
  dictionary->entries = NULL;
  return dictionary;
}

/** Destroy a dictionary
 * @param dictionary the dictionary
 */
void provided_Dictionary_destroy(Dictionary * dictionary)
{
  int i;

  REGISTRY_USINGFUNCTION;

  for(i = 0; i < dictionary->count; ++i)
  {
    free(dictionary->entries[i].name);
    if (dictionary->entries[i].type == STRING_ENTRY)
      free(dictionary->entries[i].value.stringValue);
  }
  free(dictionary->entries);
  free(dictionary);
}

/** Get a pointer on the entry associated with the given entry name
 * @param dictionary the dictionary
 * @param name the name of theentry
 * @return a pointer on the entry or NULL if the entry was not found
 */
DictionaryEntry * provided_Dictionary_getEntry(Dictionary * dictionary, const char * name)
{
  int i;

  REGISTRY_USINGFUNCTION;

  for(i = 0; i < dictionary->count; ++i)
    if (icaseCompareString(dictionary->entries[i].name, name) == 0)
      return &dictionary->entries[i];
  return NULL;
}

/** Define or change a dictionary entry as a string
 * @param dictionary the dictionary
 * @param name the entry name
 * @param value the value
 */
void provided_Dictionary_setStringEntry(Dictionary * dictionary, const char * name, const char * value)
{
  DictionaryEntry * entry;

  REGISTRY_USINGFUNCTION;

  entry = Dictionary_getEntry(dictionary, name);
  if (entry == NULL)
  {
    dictionary->entries = (DictionaryEntry*) realloc(dictionary->entries, (size_t) (dictionary->count + 1) * sizeof(DictionaryEntry));
    entry = &dictionary->entries[dictionary->count];
    entry->name = duplicateString(name);
    entry->type = UNDEFINED_ENTRY;
    dictionary->count++;
  }
  if (entry->type == STRING_ENTRY)
    free(entry->value.stringValue);
  entry->type = STRING_ENTRY;
  entry->value.stringValue = duplicateString(value);
}

/** Define or change a dictionary entry as a number
 * @param dictionary the dictionary
 * @param name the entry name
 * @param value the value
 */
void provided_Dictionary_setNumberEntry(Dictionary * dictionary, const char * name, double value)
{
  DictionaryEntry * entry;

  REGISTRY_USINGFUNCTION;

  entry = Dictionary_getEntry(dictionary, name);
  if (entry == NULL)
  {
    dictionary->entries = (DictionaryEntry*) realloc(dictionary->entries, (size_t) (dictionary->count + 1) * sizeof(DictionaryEntry));
    entry = &dictionary->entries[dictionary->count];
    entry->name = duplicateString(name);
    entry->type = UNDEFINED_ENTRY;
    dictionary->count++;
  }
  if (entry->type == STRING_ENTRY)
    free(entry->value.stringValue);
  entry->type = NUMBER_ENTRY;
  entry->value.numberValue = value;
}

/** Create a new string on the heap which is the result of the formatting of format according to the dictionary content
 * @param dictionary the dictionary
 * @param format the string to format
 * @return a new string created on the heap
 * @warning the user is responsible for freeing the returned string
 */
char * provided_Dictionary_format(Dictionary * dictionary, const char * format)
{
  char * result, *temp;
  const char * start, *end;
  char * name, *parameters;
  DictionaryEntry * entry;

  REGISTRY_USINGFUNCTION;

  start = indexOfChar(format, '%');
  if (start == NULL)
  {
    result = duplicateString(format);
  }
  else
  {
    result = subString(format, start);

    while (start != NULL)
    {
      end = indexOfChar(start + 1, '%');
      if (end == NULL)
      {
        fprintf(stderr, "Fermeture de balise manquante\n");
        end = start + stringLength(start);
      }

      if (start + 1 == end)
      {
        temp = concatenateString(result, "%");
        free(result);
        result = temp;
      }
      else
      {
        const char * lbrace = indexOfChar(start + 1, '{');
        if (lbrace == NULL)
        {
          name = subString(start + 1, end);
          parameters = duplicateString("");
        }
        else
          if (lbrace > end)
          {
            name = subString(start + 1, end);
            parameters = duplicateString("");
          }
          else
          {
            const char * rbrace = indexOfChar(lbrace + 1, '}');
            if (rbrace == NULL || rbrace > end)
            {
              fprintf(stderr, "Accolade fermante manquante dans une balise\n");
              rbrace = end - 1;
            }
            name = subString(start + 1, lbrace);
            parameters = subString(lbrace + 1, rbrace);
          }

        entry = Dictionary_getEntry(dictionary, name);
        if (entry == NULL)
        {
          fprintf(stderr, "Variable %s non définie donc ignorée\n", name);
        }
        else
        {
          char * value;
          if (entry->type == STRING_ENTRY)
          {
            const char * caseIndex = indexOfString(parameters, "case=");
            const char * minIndex = indexOfString(parameters, "min=");
            const char * maxIndex = indexOfString(parameters, "max=");

            value = duplicateString(entry->value.stringValue);
            if (caseIndex != NULL)
            {
              if (toLowerChar(*(caseIndex + 5)) == 'u')
                makeUpperCaseString(value);
              else
                makeLowerCaseString(value);
            }

            if (minIndex != NULL)
            {
              size_t minLength = (size_t) atoi(minIndex + 4);
              size_t len = stringLength(value);
              if (len < minLength)
              {
                size_t i;
                temp = (char*) malloc(minLength + 1);
                for(i = 0; i < len; ++i)
                  temp[i] = value[i];
                for(i = len; i < minLength; ++i)
                  temp[i] = ' ';
                temp[minLength] = '\0';
                free(value);
                value = temp;
              }
            }

            if (maxIndex != NULL)
            {
              size_t maxLength = (size_t) atoi(maxIndex + 4);
              size_t len = stringLength(value);
              if (len > maxLength)
                value[maxLength] = '\0';
            }
          }
          else
          {
            const char * precisionIndex = indexOfString(parameters, "precision=");
            const char * minIndex = indexOfString(parameters, "min=");
            int precision, min;
            char buf[1024];

            if (precisionIndex != NULL)
              precision = atoi(precisionIndex + 10);
            else
              precision = -1;

            if (minIndex != NULL)
              min = atoi(minIndex + 4);
            else
              min = 0;

            if (precision < 0)
              snprintf(buf, 1024, "%*f", min, entry->value.numberValue);
            else
              snprintf(buf, 1024, "%*.*f", min, precision, entry->value.numberValue);
            value = duplicateString(buf);
          }

          temp = concatenateString(result, value);
          free(result);
          free(value);
          result = temp;
        }
        free(name);
        free(parameters);
      }
      start = indexOfChar(end + 1, '%');

      if (start != NULL)
      {
        char * sub = subString(end + 1, start);
        temp = concatenateString(result, sub);
        free(sub);
        free(result);
        result = temp;
      }
    }

    temp = concatenateString(result, end + 1);
    free(result);
    result = temp;
  }

  return result;
}
