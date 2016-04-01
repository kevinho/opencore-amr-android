/* ------------------------------------------------------------------
 * Copyright (C) 1998-2009 PacketVideo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied.
 * See the License for the specific language governing permissions
 * and limitations under the License.
 * -------------------------------------------------------------------
 */
// -*- c++ -*-
// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

//                     O S C L _ S T D S T R I N G

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 *
 * @{
 */


/**
 *  @file oscl_stdstring.h
 *  @brief This file provides standard string operations such as strlen,
 *  strncpy, etc. ANSI defines undefined behavior when the destination
 *  pointer is null for operations such as strncpy, strncat, etc. But,
 *  we chose to define one. In such cases, we return the destination
 *  as null.
 */


#ifndef OSCL_STDSTRING_H_INCLUDED
#define OSCL_STDSTRING_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif


/**
 * Gets the length of a string
 *
 * @param str    NULL terminated string.
 *
 * @return Returns the number of characters in string, excluding the
 *         terminal NULL.
 */
OSCL_IMPORT_REF uint32 oscl_strlen(const char* str);

/**
 * Gets the length of a wide char string
 *
 * @param str    NULL terminated string.
 *
 * @return Returns the number of characters in string, excluding the
 *         terminal NULL.
 */
OSCL_IMPORT_REF uint32 oscl_strlen(const oscl_wchar* str);

/**
 * Copies the chars of one string to another.
 *
 * Copies the initial count characters of src to dest and
 * returns dest. If count is less than or equal to the
 * length of src, a null character is not appended automatically
 * to the copied string. If count is greater than the length
 * of src, the destination string is padded with null characters
 * up to length count. The behavior of strncpy is undefined
 * if the source and destination strings overlap.
 *
 * @param dest   Destination string
 * @param src    NULL terminated source string
 * @param count  Number of chars to copy
 *
 * @return Returns dest.
 */
OSCL_IMPORT_REF char* oscl_strncpy(char* dest, const char* src, uint32 count);

/**
 * Copies the chars of one string to another.
 *
 * Copies the initial count characters of src to dest and
 * returns dest. If count is less than or equal to the
 * length of src, a null character is not appended automatically
 * to the copied string. If count is greater than the length
 * of src, the destination string is padded with null characters
 * up to length count. The behavior of strncpy is undefined
 * if the source and destination strings overlap.
 *
 * @param dest   Destination string
 * @param src    NULL terminated source string
 * @param count  Number of chars to copy
 *
 * @return Returns dest.
 */
OSCL_IMPORT_REF oscl_wchar* oscl_strncpy(oscl_wchar* dest, const oscl_wchar* src, uint32 count);

/**
 * Lexicographically compares two NULL terminated strings,
 * str1 and str2, and returns a value indicating the relationship
 * between them.
 *
 * @param str1   String to compare
 * @param str2   String to compare
 *
 * @return Negative if str1 < str2
 *         Positive if str1 > str2
 *         Zero if equal
 */
OSCL_IMPORT_REF int32 oscl_strcmp(const char* str1, const char* str2);

/**
 * Lexicographically compares two NULL terminated strings,
 * str1 and str2, and returns a value indicating the relationship
 * between them.
 *
 * @param str1   String to compare
 * @param str2   String to compare
 *
 * @return Negative if str1 < str2
 *         Positive if str1 > str2
 *         Zero if equal
 */
OSCL_IMPORT_REF int32 oscl_strcmp(const oscl_wchar* str1, const oscl_wchar* str2);


/**
 * Lexicographically compares, at most, the first count characters
 * in str1 and str2 and returns a value indicating the relationship
 * between the substrings.
 *
 * @param str1   String to compare
 * @param str2   String to compare
 * @param count  Number of characters to compare
 *
 * @return Negative if str1 < str2
 *         Positive if str1 > str2
 *         Zero if equal
 */
OSCL_IMPORT_REF int32 oscl_strncmp(const char* str1, const char* str2, uint32 count);

/**
 * Lexicographically compares, at most, the first count characters
 * in str1 and str2 and returns a value indicating the relationship
 * between the substrings.
 *
 * @param str1   String to compare
 * @param str2   String to compare
 * @param count  Number of characters to compare
 *
 * @return Negative if str1 < str2
 *         Positive if str1 > str2
 *         Zero if equal
 */
OSCL_IMPORT_REF int32 oscl_strncmp(const oscl_wchar* str1, const oscl_wchar* str2, uint32 count);

/**
 * Appends up to count characters from string src to string
 * dest, and then appends a terminating null character. The
 * initial character of src overwrites the null character at
 * the end of dest. Subsequent characters in src are appended
 * to dest until either the end of src is reached or count
 * characters have been copied. If copying takes place between
 * objects that overlap, the behavior is undefined.
 *
 * @param dest   null terminated destination string
 * @param src    source string
 * @param count  number of characters to append.
 *
 * @return dest
 */
OSCL_IMPORT_REF char* oscl_strncat(char* dest, const char* src, uint32 count);

/**
 * Appends up to count characters from string src to string
 * dest, and then appends a terminating null character. The
 * initial character of src overwrites the null character at
 * the end of dest. Subsequent characters in src are appended
 * to dest until either the end of src is reached or count
 * characters have been copied. If copying takes place between
 * objects that overlap, the behavior is undefined.
 *
 * @param dest   null terminated destination string
 * @param src    source string
 * @param count  number of characters to append.
 *
 * @return dest
 */
OSCL_IMPORT_REF oscl_wchar* oscl_strncat(oscl_wchar* dest, const oscl_wchar* src, uint32 count);

/**
 * Finds the first occurrence of c in string, or it
 * returns NULL if c is not found. The null-terminating
 * character is included in the search.
 *
 * @param str    null terminated source string
 * @param c      character to search for
 *
 * @return
 */
OSCL_IMPORT_REF const char* oscl_strchr(const char *str, int32 c);
OSCL_IMPORT_REF char* oscl_strchr(char *str, int32 c);

/**
 * Finds the first occurrence of c in string, or it
 * returns NULL if c is not found. The null-terminating
 * character is included in the search.
 *
 * @param str    null terminated source string
 * @param c      character to search for
 *
 * @return
 */
OSCL_IMPORT_REF const oscl_wchar* oscl_strchr(const oscl_wchar *str, int32 c);
OSCL_IMPORT_REF oscl_wchar* oscl_strchr(oscl_wchar *str, int32 c);

/**
 * Finds the last occurrence of c in string, or it
 * returns NULL if c is not found. The null-terminating
 * character is included in the search.
 *
 * @param str    null terminated source string
 * @param c      character to search for
 *
 * @return
 */
OSCL_IMPORT_REF const char* oscl_strrchr(const char *str, int32 c);
OSCL_IMPORT_REF char* oscl_strrchr(char *str, int32 c);
OSCL_IMPORT_REF const oscl_wchar* oscl_strrchr(const oscl_wchar *str, int32 c);
OSCL_IMPORT_REF oscl_wchar* oscl_strrchr(oscl_wchar *str, int32 c);

/**
 * Sets the characters of a string to a specified character
 *
 * @param dest   buffer to modify
 * @param val    character to set
 * @param count  number of chars to set
 *
 * @return the value of dest
 */
OSCL_IMPORT_REF char* oscl_strset(char* dest, char val, uint32 count);

/**
 * Sets the characters of a string to a specified character
 *
 * @param dest   buffer to modify
 * @param val    character to set
 * @param count  number of chars to set
 *
 * @return the value of dest
 */
OSCL_IMPORT_REF oscl_wchar* oscl_strset(oscl_wchar* dest, oscl_wchar val, uint32 count);


/**
 * Case in-sensitive string comparision.
 *
 * @param str1  string to compare
 * @param str2  string to compare
 *
 *
 * @return Negative if str1 < str2
 *         Positive if str1 > str2
 *         Zero if equal
 */
OSCL_IMPORT_REF int32 oscl_CIstrcmp(const char *str1, const char *str2);

/**
 * Case in-sensitive string comparision.
 *
 * @param str1  string to compare
 * @param str2  string to compare
 *
 *
 * @return Negative if str1 < str2
 *         Positive if str1 > str2
 *         Zero if equal
 */
OSCL_IMPORT_REF int32 oscl_CIstrcmp(const oscl_wchar *str1, const oscl_wchar *str2);

/**
 * Lexicographically compares(case in-sensitive), at most, the first
 * count characters in str1 and str2 and returns a value indicating
 * the relationship between the substrings.
 *
 * @param str1  string to compare
 * @param str2  string to compare
 * @param count  Number of characters to compare
 *
 *
 * @return Negative if str1 < str2
 *         Positive if str1 > str2
 *         Zero if equal
 */
OSCL_IMPORT_REF int32 oscl_CIstrncmp(const char *str1, const char *str2, uint32 count);

/**
 * Lexicographically compares(case in-sensitive), at most, the first
 * count characters in str1 and str2 and returns a value indicating
 * the relationship between the substrings.
 *
 * @param str1  string to compare
 * @param str2  string to compare
 * @param count  Number of characters to compare
 *
 *
 * @return Negative if str1 < str2
 *         Positive if str1 > str2
 *         Zero if equal
 */
OSCL_IMPORT_REF int32 oscl_CIstrncmp(const oscl_wchar *str1, const oscl_wchar *str2, uint32 count);

/**
 * convert upper case ASCII character to lower case.
 * behaviour of this function for non-ASCII characters is not defined.
 *
 * @param car    upper case character.
 *
 * @return    lower case character.
 */
OSCL_IMPORT_REF char oscl_tolower(const char car);


/**
 * convert upper case ASCII character to lower case.
 * behaviour of this function for non-ASCII characters is not defined.
 *
 * @param car    upper case character.
 *
 * @return    lower case character.
 */
OSCL_IMPORT_REF oscl_wchar oscl_tolower(const oscl_wchar car);


/**
 * check if supplied parameter is an alphabet (ASCII only).
 *
 * @param car
 *
 * @return    1 if car is an alphabet
 *            0 if car is not an alphabet.
 */
OSCL_IMPORT_REF bool oscl_isLetter(const char car);


/**
 * find the occurrence of sub-string in a string.
 *
 * @param str1   string.
 * @param str2   sub-string
 *
 * @return    pointer to the begining of sub-string.
 */
OSCL_IMPORT_REF const char* oscl_strstr(const char* str1, const char* str2);
OSCL_IMPORT_REF char* oscl_strstr(char* str1, const char* str2);


/**
 * find the occurrence of sub-string in a string.
 *
 * @param str1   string.
 * @param str2   sub-string
 *
 * @return    pointer to the begining of sub-string.
 */
OSCL_IMPORT_REF const oscl_wchar* oscl_strstr(const oscl_wchar* str1, const oscl_wchar* str2);
OSCL_IMPORT_REF oscl_wchar* oscl_strstr(oscl_wchar* str1, const oscl_wchar* str2);


/**
 * Appends string src to string dest, and then appends a
 * terminating null character. The initial character of src
 * overwrites the null character at the end of dest.
 * Subsequent characters in src are appended to dest until
 * the end of src is reached. If copying takes place between
 * objects that overlap, the behavior is undefined.
 *
 * @param dest   null terminated destination string
 * @param src    source string
 *
 *
 * @return dest
 */
OSCL_IMPORT_REF char* oscl_strcat(char* dest, const char* src);


/**
 * Appends up to count characters from string src to string
 * dest, and then appends a terminating null character. The
 * initial character of src overwrites the null character at
 * the end of dest. Subsequent characters in src are appended
 * to dest until either the end of src is reached or count
 * characters have been copied. If copying takes place between
 * objects that overlap, the behavior is undefined.
 *
 * @param dest   null terminated destination string
 * @param src    source string
 * @param count  number of characters to append.
 *
 * @return dest
 */
OSCL_IMPORT_REF oscl_wchar* oscl_strcat(oscl_wchar* dest, const oscl_wchar* src);


/*! @} */


#endif
