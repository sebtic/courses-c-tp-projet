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

#include <MyString.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

/** Create a copy on the heap of part of a string. The new string contains the characters pointed from start (inclusive) to end (exclusive).
 * @note The new string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 * @param start a pointer to the first character of the new string
 * @param end a pointer to the next character of the new string (the pointed character is not part of the new string)
 * @return the new string
 *
 * @code
 * char * str = "abcdef";
 * char * s1 = subString(str,str);
 * char * s2 = subString(str,str+strlen(str));
 * char * s3 = subString(str+1,str+2);
 *
 * We get:
 *    s1: "";
 *    s2: "abcdef";
 *    s3: "b";
 * @endcode
 */
char * (*subString)(const char * start, const char * end);

/** Create a new string on the heap which contents is the result of the insertion in src of insertLength characters from  toBeInserted at position insertPosition.
 * @note The new string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 * @param src the string in which the insertion is done
 * @param insertPosition the position where the insertion is done
 * @param toBeInserted a pointer on the first character to insert
 * @param insertLength the number of character to insert (insertLength must be lower or equal to strlen(toBeInserted))
 * @return the new string
 */
char * (*insertString)(const char * src, int insertPosition, const char * toBeInserted, int insertLength);

char * provided_dangerousStrCat(const char * UNUSED(dest), const char * UNUSED(src))
{
  fatalError("La fonction strcat est dangereuse. Utilisez une autre fonction.");
  return NULL;
}
char * provided_dangerousStrNCat(const char * UNUSED(dest), const char * UNUSED(src), size_t UNUSED(srcMaxLength))
{
  fatalError("La fonction strncat est dangereuse. Utilisez une autre fonction.");
  return NULL;
}

/** Like the tolower() function. It converts the letter c to lower case, if possible.
 * @param c the letter to convert
 * @return the lower case letter associated to c if c is a letter, or c otherwise
 */
char provided_toLowerChar(char c)
{
  REGISTRY_USINGFUNCTION;

  if ('A' <= c && c <= 'Z')
    return (char) (c - 'A' + 'a');
  else
    return c;
}

/** Like the toupper() function. It converts the letter c to upper case, if possible.
 * @param c the letter to convert
 * @return the upper case letter associated to c if c is a letter, or c otherwise
 */
char provided_toUpperChar(char c)
{
  REGISTRY_USINGFUNCTION;

  if ('a' <= c && c <= 'z')
    return (char) (c - 'a' + 'A');
  else
    return c;
}

/** Like the strcmp() function. It compares the two strings str1 and str2.
 * It returns an integer less than, equal to, or greater than zero if str1 is found, respectively, to be less than, to match, or be greater
 * than str2.
 * @param str1 the first string
 * @param str2 the second string
 * @return an integer less than, equal to, or greater than zero if str1 is found, respectively, to be less than, to match, or be greater
 * than str2
 */
int provided_compareString(const char * str1, const char * str2)
{
  REGISTRY_USINGFUNCTION;

  while (*str1 != '\0' && *str2 != '\0' && *str1 == *str2)
  {
    str1++;
    str2++;
  }
  if (*str1 == '\0' && *str2 == '\0')
    return 0;
  else
  {
    return *str1 - *str2;
  }
}

/** Like the strcasecmp() function. It compares the two strings str1 and str2, ignoring the case of the characters.
 * It returns an integer less than, equal to, or greater than zero if str1 is found, respectively, to be less than, to match, or be greater
 * than str2.
 * @param str1 the first string
 * @param str2 the second string
 * @return an integer less than, equal to, or greater than zero if str1 is found, respectively, to be less than, to match, or be greater
 * than str2
 */
int provided_icaseCompareString(const char * str1, const char * str2)
{
  REGISTRY_USINGFUNCTION;

  while (*str1 != '\0' && *str2 != '\0' && toLowerChar(*str1) == toLowerChar(*str2))
  {
    str1++;
    str2++;
  }
  if (*str1 == '\0' && *str2 == '\0')
    return 0;
  else
  {
    return toLowerChar(*str1) - toLowerChar(*str2);
  }
}

/** Like the strlen() function. It calculates the length of the string str, not including the terminating '\\0' character.
 * @param str the string
 * @return the number of characters in str.
 */
size_t provided_stringLength(const char * str)
{
  size_t count = 0;

  REGISTRY_USINGFUNCTION;

  while (*str != '\0')
  {
    count++;
    str++;
  }
  return count;
}

void provided_dangerousStrCpy(char * UNUSED(dest), const char * UNUSED(src))
{
  fatalError("La fonction strcpy est dangereuse. Utilisez une autre fonction.");
}

void provided_dangerousStrNCpy(char * UNUSED(dest), const char * UNUSED(src), size_t UNUSED(destsize))
{
  fatalError("La fonction strncpy est dangereuse. Utilisez une autre fonction.");
}

/** Copy the string pointed to by src, including the terminating null byte ('\\0'), to the buffer pointed to by dest.
 * @warning The strings may not overlap, and the destination string dest must be large enough to receive the copy.
 *
 * @param dest the destination string
 * @param src the source string
 */
void provided_copyString(char * dest, const char * src)
{
  REGISTRY_USINGFUNCTION;

  fprintf(stderr, "You should avoid strcpy and copyString. Use strncpy or copyStringWithLength instead\n");
  copyStringWithLength(dest, src, INT_MAX);
}

/** Copy the first destSize characters of the string pointed to by src, including the terminating null byte ('\\0'), to the buffer pointed to by dest.
 * @warning The strings may not overlap, and the destination string dest must be large enough to receive the copy.
 * @warning If src is longer than destSize characters, the string is truncated and the terminating null byte ('\\0') is added.
 *
 * @param dest the destination string
 * @param src the source string
 * @param destSize the maximal number of characters to copy
 */
void provided_copyStringWithLength(char * dest, const char * src, size_t destsize)
{
  size_t len;
  size_t i;

  if (destsize == 0)
    fatalError("destSize must not be null");

  REGISTRY_USINGFUNCTION;

  len = stringLength(src);

  if (len + 1 > destsize)
    len = destsize - 1;
  for(i = 0; i < len; ++i)
    dest[i] = src[i];
  dest[len] = '\0';
}

/** Like the strdup() function. It creates a copy of the string on the heap.
 * @note The copy is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 * @param str the string to duplicate
 */
char * provided_duplicateString(const char * str)
{
  char * result;
  char * p;

  REGISTRY_USINGFUNCTION;

  result = malloc((size_t) stringLength(str) + 1);
  p = result;

  while (*str != '\0')
    *p++ = *str++;
  *p = *str;
  return result;
}

/** Test if the string str begins by the string start, ignoring the case of the characters.
 * @param start the string which should be at the beginning of str
 * @param str the string to test
 * @return a non null number (true) if the str begins with the string start, false otherwise
 */
int provided_icaseStartWith(const char * start, const char * str)
{
  REGISTRY_USINGFUNCTION;

  while (*start != '\0' && *str != '\0' && toLowerChar(*start) == toLowerChar(*str))
  {
    start++;
    str++;
  }
  return (*start == '\0');
}

/** Test if the string str ends by the string start, ignoring the case of the characters.
 * @param end the string which should be at the end of str
 * @param str the string to test
 * @return a non null number (true) if the str ends with the string end, false otherwise
 */
int provided_icaseEndWith(const char * end, const char * str)
{
  size_t lenStr;
  size_t lenEnd;

  REGISTRY_USINGFUNCTION;

  lenStr = stringLength(str);
  lenEnd = stringLength(end);

  if (lenStr < lenEnd)
    return 0;
  str += lenStr - lenEnd;
  while (*end != '\0' && *str != '\0' && toLowerChar(*end) == toLowerChar(*str))
  {
    end++;
    str++;
  }
  return (*end == '\0');
}

/** Create a new string on the heap which is the result of the concatenation of the two strings.
 * @note The new string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 * @param str1 the first string
 * @param str2 the second string
 * @return the new string
 */
char * provided_concatenateString(const char * src1, const char * src2)
{
  char * result;
  char * p;

  REGISTRY_USINGFUNCTION;

  result = (char*) malloc(stringLength(src1) + stringLength(src2) + 1);
  p = result;

  while (*src1 != '\0')
  {
    *p = *src1;
    p++;
    src1++;
  }

  while (*src2 != '\0')
  {
    *p = *src2;
    p++;
    src2++;
  }
  *p = '\0';
  return result;
}

/** Like the index() function. It returns a pointer to the first occurrence of the character c in the string str.
 * @param str the string to search in
 * @param c the character to find
 * @return a pointer to the first occurrence of the character c in the string str if c is in str, NULL otherwise
 */
const char * provided_indexOfChar(const char *s, char c)
{
  REGISTRY_USINGFUNCTION;
  while (*s != '\0' && *s != c)
    s++;
  if (*s == c)
    return (char*) s;
  else
    return NULL;
}

/** Create a copy on the heap of part of a string. The new string contains the characters pointed from start (inclusive) to end (exclusive).
 * @note The new string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 * @param start a pointer to the first character of the new string
 * @param end a pointer to the next character of the new string (the pointed character is not part of the new string)
 * @return the new string
 *
 * @code
 * char * str = "abcdef";
 * char * s1 = subString(str,str);
 * char * s2 = subString(str,str+strlen(str));
 * char * s3 = subString(str+1,str+2);
 *
 * We get:
 *    s1: "";
 *    s2: "abcdef";
 *    s3: "b";
 * @endcode
 */
char * provided_subString(const char * start, const char * end)
{
  char * result;
  char * p;

  REGISTRY_USINGFUNCTION;

  result = (char*) malloc((size_t) (end - start + 1));
  p = result;

  while (*start != *end)
  {
    *p = *start;
    start++;
    p++;
  }
  *p = '\0';
  return result;
}

/** Like the strstr() function. It returns a pointer to the first occurrence of the string aiguille in the string meule_de_foin.
 * @param meule_de_foin the string to search in
 * @param aiguille the string to find
 * @return a pointer to the first occurrence of the string aiguille in the string meule_de_foin if aiguille is in meule_de_foin, NULL otherwise
 */
const char * provided_indexOfString(const char *meule_de_foin, const char *aiguille)
{
  REGISTRY_USINGFUNCTION;

  while (*meule_de_foin != '\0')
  {
    char * pmeule = (char*) meule_de_foin;
    char * paiguille = (char*) aiguille;
    while (*pmeule != '\0' && *paiguille != '\0' && *pmeule == *paiguille)
    {
      pmeule++;
      paiguille++;
    }
    if (*paiguille == '\0')
    {
      return (char*) meule_de_foin;
    }
    meule_de_foin++;
  }
  return NULL;
}

/** Convert a string to upper case.
 * @param str the string to convert
 */
void provided_makeUpperCaseString(char * str)
{
  REGISTRY_USINGFUNCTION;

  while (*str != '\0')
  {
    *str = toUpperChar(*str);
    str++;
  }
}

/** Convert a string to lower case.
 * @param str the string to convert
 */
void provided_makeLowerCaseString(char * str)
{
  REGISTRY_USINGFUNCTION;

  while (*str != '\0')
  {
    *str = toLowerChar(*str);
    str++;
  }
}

/** Create a new string on the heap which contents is the result of the insertion in src of insertLength characters from  toBeInserted at position insertPosition.
 * @note The new string is allocated using malloc().
 * @warning the user is responsible for freeing the memory allocated for the new string
 * @param src the string in which the insertion is done
 * @param insertPosition the position where the insertion is done
 * @param toBeInserted a pointer on the first character to insert
 * @param insertLength the number of character to insert (insertLength must be lower or equal to strlen(toBeInserted))
 * @return the new string
 */
char * provided_insertString(const char * src, int insertPosition, const char * toBeInserted, int insertLength)
{
  char * result;
  int i;

  REGISTRY_USINGFUNCTION;

  result = (char*) malloc(stringLength(src) + (size_t) insertLength + 1);

  for(i = 0; i < insertPosition; ++i)
    result[i] = src[i];
  for(i = 0; i < insertLength; ++i)
    result[insertPosition + i] = toBeInserted[i];
  for(i = insertPosition; src[i] != '\0'; ++i)
    result[i + insertLength] = src[i];
  result[i + insertLength] = '\0';
  return result;
}
