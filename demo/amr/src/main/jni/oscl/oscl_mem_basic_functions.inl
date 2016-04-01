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
#ifndef OSCL_MEM_BASIC_FUNCTIONS_INL
#define OSCL_MEM_BASIC_FUNCTIONS_INL

#ifndef OSCLCONFIG_MEMORY_H_INCLUDED
#include "osclconfig_memory.h"
#endif

#ifndef OSCL_MEM_BASIC_FUNCTIONS_H
#include "oscl_mem_basic_functions.h"
#endif

// Basic memory manipulation functions
#define OSCL_DISABLE_WARNING_TRUNCATE_DEBUG_MESSAGE
#include "osclconfig_compiler_warnings.h"

OSCL_INLINE OSCL_COND_EXPORT_REF void* _oscl_malloc(int32 size)
{
    return malloc(size);
}

OSCL_INLINE OSCL_COND_EXPORT_REF void* _oscl_realloc(void* src, int32 size)
{
    return realloc(src, size);
}

OSCL_INLINE OSCL_COND_EXPORT_REF void* _oscl_calloc(int32 size1, int32 size2)
{
    return calloc(size1, size2);
}

OSCL_INLINE OSCL_COND_EXPORT_REF void _oscl_free(void* src)
{
    free(src);
}

OSCL_INLINE OSCL_COND_EXPORT_REF void* oscl_memcpy(void* dest, const void* src, uint32 count)
{
    return memcpy(dest, src, count);
}

OSCL_INLINE OSCL_COND_EXPORT_REF void* oscl_memmove(void* dest, const void* src, uint32 count)
{
    return memmove(dest, src, count);
}

OSCL_INLINE OSCL_COND_EXPORT_REF void* oscl_memmove32(void* dest, const void* src, uint32 count)
{
    return memmove(dest, src, count);
}

OSCL_INLINE OSCL_COND_EXPORT_REF void* oscl_memset(void* dest, uint8 val, uint32 count)
{
    return memset(dest, val, count);
}

OSCL_INLINE OSCL_COND_EXPORT_REF int oscl_memcmp(const void* dest, const void* src, uint32 count)
{
    return memcmp(dest, src, count);
}

#endif /*OSCL_MEM_BASIC_FUNCTIONS_INL*/

