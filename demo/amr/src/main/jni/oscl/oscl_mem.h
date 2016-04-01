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

//                  O S C L _ M E M

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclmemory OSCL Memory
 *
 * @{
 */


/*! \file oscl_mem.h
    \brief This file contains basic memory definitions for common use across platforms.

    This is the main entry point header file for the OSCL memory library.  It should be
    the only one users directly include.  Basic memory copy, compare, and move functions
    are defined here as well as the allocation functions.
*/

#ifndef OSCL_MEM_H_INCLUDED
#define OSCL_MEM_H_INCLUDED

#ifndef OSCLCONFIG_MEMORY_H_INCLUDED
#include "osclconfig_memory.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_TYPES_H_INCLUDE
#include "oscl_types.h"
#endif

#ifndef OSCL_ASSERT_H_INCLUDED
#include "oscl_assert.h"
#endif

#ifndef OSCL_MEM_BASIC_FUNCTIONS_H
#include "oscl_mem_basic_functions.h"
#endif

#ifndef OSCL_LOCK_BASE_H_INCLUDED
#include "oscl_lock_base.h"
#endif

#define OSCL_DISABLE_WARNING_TRUNCATE_DEBUG_MESSAGE
#include "osclconfig_compiler_warnings.h"

#ifndef OSCL_MEM_INST_H_INCLUDED
#include "oscl_mem_inst.h"
#endif

#ifndef OSCL_HEAPBASE_H_INCLUDED
#include "oscl_heapbase.h"
#endif

//Default for OSCL_HAS_GLOBAL_NEW_DELETE in case it is *not* defined
//in the osclconfig_memory.h
#ifndef OSCL_HAS_GLOBAL_NEW_DELETE
#ifdef NDEBUG
//Release Mode - No definition for global new and delete.
#define OSCL_HAS_GLOBAL_NEW_DELETE 0
#else
//Debug Mode - Define global new and delete.
#define OSCL_HAS_GLOBAL_NEW_DELETE 1
#endif //NDEBUG
#endif //OSCL_HAS_GLOBAL_NEW_DELETE

class OsclMem
{
    public:
        /** Per-thread initialization of Oscl Memory
        **   @param lock: A lock class for use with multi-threaded applications.
        **      The lock is needed in use cases where memory may be allocated
        **      in one thread and freed in another.  In this case, there must
        **      be a single lock object, and its pointer must be passed to
        **      the OsclMem::Init call in each thread.
        **      If no lock is provided, the memory manager will not be thread-safe.
        ** @exception: Leaves on error
        */
        OSCL_IMPORT_REF static void Init();

        /** Per-thread cleanup of Oscl Memory
        ** @exception: Leaves on error;
        */
        OSCL_IMPORT_REF static void Cleanup();

};

/*
** Choose whether to use per-thread or singleton registry for auditing
*/
#include "oscl_base.h"

/*
** Audit control block
*/
#if (OSCL_BYPASS_MEMMGT)
//empty class for compilation only
class OsclAuditCB
{
    public:
};
#else
class OsclMemStatsNode;
class OsclMemAudit;
class OsclAuditCB
{
    public:
        const OsclMemStatsNode* pStatsNode;
        OsclMemAudit *pAudit;

        OsclAuditCB() :
                pStatsNode(NULL),
                pAudit(NULL)
        {}

        OsclAuditCB(const OsclMemStatsNode* myStatsNode,
                    OsclMemAudit *ptr)
                :
                pStatsNode(myStatsNode),
                pAudit(ptr)
        {
        }
};
#endif//OSCL_BYPASS_MEMMGT

/**
 * Get memory-aligned size of an object.
 *
 * @param size size of object
 *
 * @returns memory-aligned size
 */
OSCL_COND_IMPORT_REF uint oscl_mem_aligned_size(uint size);

/**
 * Initialize an OsclAuditCB object.
 * Sets the stats node pointer to null, and sets the
 * audit pointer to the global audit object.
 *
 * @param auditCB memory management audit object
 */
OSCL_IMPORT_REF void OsclMemInit(OsclAuditCB & auditCB);

/**
 * Cleans up the base class of a partially-constructed
 * derived class.  This macro will call the destructor
 * if necessary, based on the error-handling implementation.
 *
 * @param T: name of the base class.
 */
#define OSCL_CLEANUP_BASE_CLASS(T) _OSCL_CLEANUP_BASE_CLASS(T)

/** *******************************************************
 * Macros for new/delete with a given allocator/deallocator.
 */

/**
 * Creates an object of type T using the given allocator to
 * acquire the memory needed.
 *
 * @param T_allocator allocator for objects of type T, must be
 *    an Oscl_TAlloc<T, Allocator>, where Allocator is an Oscl_DefAlloc
 * @param T           type of object to create
 * @param params      object initialization parameters
 *
 * @return pointer to created object
 *
 * @exception none, unless thrown by the given allocator
 */
#if(OSCL_BYPASS_MEMMGT)
#define OSCL_ALLOC_NEW(T_allocator, T, params) new(T_allocator.allocate(1)) T params
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_ALLOC_NEW(T_allocator, T, params) new(T_allocator.allocate_fl(1,__FILE__,__LINE__)) T params
#else
#define OSCL_ALLOC_NEW(T_allocator, T, params) new(T_allocator.allocate(1)) T params
#endif

/**
 * Creates an object of type T using the given allocator to
 * acquire the memory needed.
 * This macro is similar to OSCL_ALLOC_NEW except that it
 * handles constructors that leave.
 * If the constructor leaves, the destructor will be called,
 * and allocated memory will be freed before allowing the
 * leave to propagate to the next level.
 *
 * @param T_ptr    variable to hold return value-- pointer to
 *                 new object of type T.
 * @param T_allocator allocator for objects of type T, must be
 *    an Oscl_TAlloc<T, Allocator>, where Allocator is an
 *    Oscl_DefAlloc
 * @param T        type of object to create
 * @param params   object initialization parameters
 *
 * @return pointer to created object
 *
 * @exception none, unless thrown by the given allocator
 */
#if(OSCL_BYPASS_MEMMGT)
#define OSCL_TRAP_ALLOC_NEW(T_ptr,T_allocator,T,params) _OSCL_TRAP_NEW(T_allocator.allocate(1),T_allocator.deallocate,T_ptr,T,params)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_TRAP_ALLOC_NEW(T_ptr,T_allocator,T,params) _OSCL_TRAP_NEW(T_allocator.allocate_fl(1,__FILE__,__LINE__),T_allocator.deallocate,T_ptr,T,params)
#else
#define OSCL_TRAP_ALLOC_NEW(T_ptr,T_allocator,T,params) _OSCL_TRAP_NEW(T_allocator.allocate(1),T_allocator.deallocate,T_ptr,T,params)
#endif

/**
 * Deletes the object of type T using the given allocator
 *
 * @param T_allocator allocator for objects of type T
 * @param T           type of object to delete
 * @param ptr         pointer to previously created object
 *
 * @exception none, unless thrown by the given allocator
 */
#define OSCL_ALLOC_DELETE(ptr, T_allocator, T) \
  {\
  ptr->~T();\
  T_allocator.deallocate(ptr);\
  }


/** *******************************************************
 * Macros for malloc/free with memory management.
 */

//These are for internal use but need to be visible since they're used
//in macros.
#if(!OSCL_BYPASS_MEMMGT)
OSCL_IMPORT_REF void* _oscl_audit_malloc(size_t , OsclAuditCB & , const char * f = NULL, const int l = 0);
OSCL_IMPORT_REF void* _oscl_audit_calloc(size_t , size_t, OsclAuditCB & , const char * f = NULL, const int l = 0);
OSCL_IMPORT_REF void* _oscl_audit_realloc(void*, size_t , OsclAuditCB & , const char * f = NULL, const int l = 0);
OSCL_IMPORT_REF void* _oscl_audit_new(size_t , OsclAuditCB & , const char * f = NULL, const int l = 0) ;
OSCL_IMPORT_REF void* _oscl_default_audit_malloc(size_t , const char * f = NULL, const int l = 0);
OSCL_IMPORT_REF void* _oscl_default_audit_calloc(size_t , size_t, const char * f = NULL, const int l = 0);
OSCL_IMPORT_REF void* _oscl_default_audit_realloc(void*, size_t , const char * f = NULL, const int l = 0);
OSCL_IMPORT_REF void* _oscl_default_audit_new(size_t , const char * f = NULL, const int l = 0) ;
OSCL_IMPORT_REF void _oscl_audit_free(void *);
#else
OSCL_IMPORT_REF void* _oscl_default_new(size_t nBytes);
#endif//OSCL_BYPASS_MEMMGT

#if (OSCL_HAS_GLOBAL_NEW_DELETE)
//Global New operator overloaded to check native new operators called

#if(!OSCL_BYPASS_MEMMGT)
inline void * operator new(size_t aSize, const char *aFile, int aLine)
{
#if(PVMEM_INST_LEVEL>0)
    //in case NULL is passed in, record this file & line #
    if (!aFile)
        return _oscl_default_audit_new(aSize, __FILE__, __LINE__);
#endif
    return _oscl_default_audit_new(aSize, aFile, aLine);
};
#endif

inline void * operator new(size_t aSize)
{
#if(!OSCL_BYPASS_MEMMGT)
#if(PVMEM_INST_LEVEL>0)
    return _oscl_default_audit_new(aSize, __FILE__, __LINE__);
#else
    return _oscl_default_audit_new(aSize);
#endif
#else
    return _oscl_default_new(aSize);
#endif
};

inline void operator delete(void *aPtr)
{
#if(!OSCL_BYPASS_MEMMGT)
    _oscl_audit_free(aPtr);
#else
    _oscl_free(aPtr);
#endif
};

#if(!OSCL_BYPASS_MEMMGT)
inline void * operator new[](size_t aSize, const char *aFile, int aLine)
{
#if(PVMEM_INST_LEVEL>0)
    //in case NULL is passed in, record this file & line #
    if (!aFile)
        return _oscl_default_audit_new(aSize, __FILE__, __LINE__);
#endif
    return _oscl_default_audit_new(aSize, aFile, aLine);
};
#endif

inline void * operator new[](size_t aSize)
{
#if(!OSCL_BYPASS_MEMMGT)
#if(PVMEM_INST_LEVEL>0)
    return _oscl_default_audit_new(aSize, __FILE__, __LINE__);
#else
    return _oscl_default_audit_new(aSize);
#endif
#else
    return _oscl_default_new(aSize);
#endif
};

inline void operator delete[](void *aPtr)
{
#if(!OSCL_BYPASS_MEMMGT)
    _oscl_audit_free(aPtr);
#else
    _oscl_free(aPtr);
#endif
};
#endif //OSCL_HAS_GLOBAL_NEW_DELETE

/**
 * Allocates a memory block using the memory management's
 * global audit object.
 *
 * @param count  number of bytes to allocate
 *
 * @return a void pointer to the allocated space, or NULL if there is insufficient
 *         memory available.
 *
 * @exception none
 */
#if(OSCL_BYPASS_MEMMGT)
#define OSCL_MALLOC(count) _oscl_malloc(count)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_MALLOC(count) _oscl_default_audit_malloc(count,__FILE__,__LINE__)
#else
#define OSCL_MALLOC(count) _oscl_default_audit_malloc(count)
#endif

/*
** The public oscl_malloc call has been deprecated.
** PV code should call OSCL_MALLOC.
** This macro is defined for back-compatibility.
*/
#define oscl_malloc(a) OSCL_MALLOC(a)

/**
 * Another back-compatibility definition.
 */
#define OSCL_DEFAULT_MALLOC(x) OSCL_MALLOC(x)

/**
* Allocates a memory block using the given audit object.
*
* @param auditCB input memory management audit object
* @param count   number of bytes to allocate
*
* @return a void pointer to the allocated space, or NULL if there is insufficient
*         memory available.
*
* @exception none
*/
#if(OSCL_BYPASS_MEMMGT)
#define OSCL_AUDIT_MALLOC(auditCB, count) _oscl_malloc(count)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_AUDIT_MALLOC(auditCB, count) _oscl_audit_malloc(count, auditCB, __FILE__, __LINE__)
#else
#define OSCL_AUDIT_MALLOC(auditCB, count) _oscl_audit_malloc(count, auditCB)
#endif

/**
 * Allocates a memory block using the memory management's
 * global audit object.  The block is initialized to zero.
 *
 * @param num  number of elements
 * @param size  number of bytes to allocate for each element
 *
 * @return a void pointer to the allocated space, or NULL if there is insufficient
 *         memory available.
 *
 * @exception none
 */
#if(OSCL_BYPASS_MEMMGT)
#define OSCL_CALLOC(num,size) _oscl_calloc(num,size)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_CALLOC(num,size) _oscl_default_audit_calloc(num,size,__FILE__,__LINE__)
#else
#define OSCL_CALLOC(num,size) _oscl_default_audit_calloc(num,size)
#endif

/*
** The public oscl_calloc call has been deprecated.
** PV code should call OSCL_CALLOC.
** This macro is defined for back-compatibility.
*/
#define oscl_calloc(a,b) OSCL_CALLOC(a,b)

/**
* Allocates a memory block using the specified
* audit object.  The block is initialized to zero.
*
* @param auditCB input memory management audit object
* @param num  number of elements
* @param size  number of bytes to allocate for each element
*
* @return a void pointer to the allocated space, or NULL if there is insufficient
*         memory available.
*
* @exception none
*/
#if(OSCL_BYPASS_MEMMGT)
#define OSCL_AUDIT_CALLOC(auditCB, num,size) _oscl_calloc(num,size)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_AUDIT_CALLOC(auditCB, num,size) _oscl_audit_calloc(num,size, auditCB, __FILE__, __LINE__)
#else
#define OSCL_AUDIT_CALLOC(auditCB, num,size) _oscl_audit_calloc(num,size, auditCB)
#endif

/**
 * Re-Allocates a memory block using the memory management's
 * global audit object.
 *
 * @param ptr  original memory block
 * @param new_size  New size of the block
 *
 * @return a void pointer to the allocated space, or NULL if there is insufficient
 *         memory available.
 *
 * @exception none
 */
#if(OSCL_BYPASS_MEMMGT)
#define OSCL_REALLOC(ptr,new_size) _oscl_realloc(ptr,new_size)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_REALLOC(ptr,new_size) _oscl_default_audit_realloc(ptr,new_size,__FILE__,__LINE__)
#else
#define OSCL_REALLOC(ptr,new_size) _oscl_default_audit_realloc(ptr,new_size)
#endif

/*
** The public oscl_realloc call has been deprecated.
** PV code should call OSCL_REALLOC.  This macro is
** defined for back-compatibility.
*/
#define oscl_realloc(a,b) OSCL_REALLOC(a,b)

/**
* Re-Allocates a memory block using the specified
* audit object.
*
* @param auditCB input memory management audit object
* @param ptr  original memory block
* @param new_size  New size of the block
*
* @return a void pointer to the allocated space, or NULL if there is insufficient
*         memory available.
*
* @exception none
*/
#if(OSCL_BYPASS_MEMMGT)
#define OSCL_AUDIT_REALLOC(auditCB, ptr,new_size) _oscl_realloc(ptr,new_size)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_AUDIT_REALLOC(auditCB, ptr,new_size) _oscl_audit_realloc(ptr,new_size, auditCB, __FILE__, __LINE__)
#else
#define OSCL_AUDIT_REALLOC(auditCB, ptr,new_size) _oscl_audit_realloc(ptr,new_size, auditCB)
#endif

/**
 * Deallocates or frees a memory block.
 *
 * @param ptr    pointer to previously allocated memory block using the given audit object
 */
#if(OSCL_BYPASS_MEMMGT)
#define OSCL_FREE(ptr) _oscl_free(ptr)
#else
#define OSCL_FREE(ptr) _oscl_audit_free(ptr)
#endif

/*
** The public oscl_free call has been deprecated.
** PV code should call OSCL_FREE.
** This macro is defined for back-compatibility.
*/
#define oscl_free(x) OSCL_FREE(x)

/**
 * Another back-compatibility definition.
 */
#define OSCL_DEFAULT_FREE(x) OSCL_FREE(x)

/** *******************************************************
 * Macros for new/delete with memory management.
 */

/**
 * Oscl "new" operator.  This uses the global memory
 * audit object.
 *
 * @param T       data type for 'new' operation
 * @param params  object initialization parameters
 *
 * @return pointer to the newly created object of type T
 *
 * @exception may leave with code = bad alloc
 *
 *
 */
#if(OSCL_BYPASS_MEMMGT)
#define OSCL_NEW( T, params) new T params
#elif!(OSCL_HAS_GLOBAL_NEW_DELETE)
#define OSCL_NEW( T, params) new T params
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_NEW( T, params) new(__FILE__,__LINE__) T params
#else
#define OSCL_NEW( T, params) new T params
#endif

/********************************************************
 * Macro for placement new
 *
 * @param ptr          pointer to an object
 *
 * @param constructor  constructor of the class for the object
 *
********************************************************/
#define OSCL_PLACEMENT_NEW(ptr, constructor) new(ptr) constructor

/**
 * Oscl "new" operator.  This uses the global memory
 * audit object.  This operator is similar to OSCL_NEW
 * except that it will handle constructors that leave.
 * If the constructor leaves, the destructor will be called,
 * and allocated memory will be freed before allowing the
 * leave to propagate to the next level.
 *
 * @param T_ptr   variable to hold return value-- pointer to
 *                new object of type T.
 * @param T       data type for 'new' operation
 * @param params  object initialization parameters
 *
 * @return pointer to the newly created object of type T
 *
 * @exception may leave with code = bad alloc
 *
 *
 */
#if(OSCL_BYPASS_MEMMGT)
#define OSCL_TRAP_NEW(T_ptr,T,params) _OSCL_TRAP_NEW(_oscl_default_new(sizeof(T)),_oscl_free,T_ptr,T,params)
#elif!(OSCL_HAS_GLOBAL_NEW_DELETE)
#define OSCL_TRAP_NEW(T_ptr,T,params) _OSCL_TRAP_NEW(_oscl_default_audit_new(sizeof(T)),_oscl_audit_free,T_ptr,T,params)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_TRAP_NEW(T_ptr,T,params) _OSCL_TRAP_NEW(_oscl_default_audit_new(sizeof(T),__FILE__,__LINE__),_oscl_audit_free,T_ptr,T,params)
#else
#define OSCL_TRAP_NEW(T_ptr,T,params) _OSCL_TRAP_NEW(_oscl_default_audit_new(sizeof(T)),_oscl_audit_free,T_ptr,T,params)
#endif


/**
 * Oscl "new" operator.  This uses the specified memory
 * audit object.
 *
 * @param auditCB input memory management audit object
 * @param T       data type for 'new' operation
 * @param params  object initialization parameters
 *
 * @return pointer to the newly created object of type T
 *
 * @exception may leave with code = bad alloc
 *
 */
#if(OSCL_BYPASS_MEMMGT)
#define OSCL_AUDIT_NEW(auditCB, T, params) new(_oscl_default_new(sizeof(T))) T params
#elif!(OSCL_HAS_GLOBAL_NEW_DELETE)
#define OSCL_AUDIT_NEW(auditCB, T, params) new(_oscl_audit_new(sizeof(T),auditCB)) T params
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_AUDIT_NEW(auditCB, T, params) new(_oscl_audit_new(sizeof(T),auditCB,__FILE__,__LINE__)) T params
#else
#define OSCL_AUDIT_NEW(auditCB, T, params) new(_oscl_audit_new(sizeof(T),auditCB)) T params
#endif

/**
 * Oscl "new" operator.  This uses the specified memory
 * audit object.  This macro is similar to OSCL_AUDIT_NEW
 * except that it will handle constructors that leave.
 * If the constructor leaves, the destructor will be called,
 * and allocated memory will be freed before allowing the
 * leave to propagate to the next level.
 *
 * @param T_ptr   variable to hold return value-- pointer to
 *                new object of type T.
 * @param auditCB input memory management audit object
 * @param T       data type for 'new' operation
 * @param params  object initialization parameters
 *
 * @return pointer to the newly created object of type T
 *
 * @exception may leave with code = bad alloc
 *
 */
#if(OSCL_BYPASS_MEMMGT)
#define OSCL_TRAP_AUDIT_NEW(T_ptr,auditCB,T,params) _OSCL_TRAP_NEW(_oscl_default_new(sizeof(T)),_oscl_free,T_ptr,T,params)
#elif!(OSCL_HAS_GLOBAL_NEW_DELETE)
#define OSCL_TRAP_AUDIT_NEW(T_ptr,auditCB,T,params) _OSCL_TRAP_NEW(_oscl_audit_new(sizeof(T),auditCB),_oscl_audit_free,T_ptr,T,params)
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_TRAP_AUDIT_NEW(T_ptr,auditCB,T,params) _OSCL_TRAP_NEW(_oscl_audit_new(sizeof(T),auditCB,__FILE__,__LINE__),_oscl_audit_free,T_ptr,T,params)
#else
#define OSCL_TRAP_AUDIT_NEW(T_ptr,auditCB,T,params) _OSCL_TRAP_NEW(_oscl_audit_new(sizeof(T),auditCB),_oscl_audit_free,T_ptr,T,params)
#endif

/**
 * Oscl "delete" operator.
 *
 * @param ptr pointer to memory block previously allocated with OSCL_NEW
 *
 * @return void
 */
#define OSCL_DELETE(ptr) {\
    if(ptr){delete(ptr);}\
}


/** *******************************************************
 * Macros for array new/delete with memory management.
 * These only work for simple array types and cannot
 * be used for class types with constructor/destructors.
 *
 * Note: some compilers do not support placement array
 * new operator, so these macros don't use it.
 */

/**
 * Oscl array "new" operator.  This uses the input memory
 * audit object.
 *
 * @param auditCB input memory management audit object
 * @param T       data type for 'new' operation
 * @param count   number of elements to create
 *
 * @return pointer to the newly created object array of type T
 *
 * @exception may leave with code = bad alloc
 *
 *
 */
#if(OSCL_BYPASS_MEMMGT)
#define OSCL_AUDIT_ARRAY_NEW(auditCB, T, count) new(_oscl_default_new(sizeof(T)*(count))) T
#elif!(OSCL_HAS_GLOBAL_NEW_DELETE)
#define OSCL_AUDIT_ARRAY_NEW(auditCB, T, count) new(_oscl_audit_new(sizeof(T)*(count),auditCB)) T
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_AUDIT_ARRAY_NEW(auditCB, T, count) new(_oscl_audit_new(sizeof(T)*(count),auditCB,__FILE__,__LINE__)) T
#else
#define OSCL_AUDIT_ARRAY_NEW(auditCB, T, count) new(_oscl_audit_new(sizeof(T)*(count),auditCB)) T
#endif

/**
 * Oscl array "new" operator.  This uses the global memory
 * audit object.
 *
 * @param T       data type for 'new' operation
 * @param count   number of elements to create
 *
 * @return pointer to the newly created object array of type T
 *
 * @exception may leave with code = bad alloc
 *
 *
 */
#if(OSCL_BYPASS_MEMMGT)
#define OSCL_ARRAY_NEW(T, count) new T[count]
#elif!(OSCL_HAS_GLOBAL_NEW_DELETE)
#define OSCL_ARRAY_NEW(T, count) new T[count]
#elif(PVMEM_INST_LEVEL>0)
#define OSCL_ARRAY_NEW(T, count) new(__FILE__,__LINE__) T[count]
#else
#define OSCL_ARRAY_NEW(T, count) new T[count]
#endif

/**
 * Oscl array delete operator..
 *
 * @param ptr pointer to memory block previously allocated with OSCL_ARRAY_NEW
 *
 * @return void
 */
#define OSCL_ARRAY_DELETE(ptr) delete [] ptr


/**
* Previously this was in oscl_mem_imp.h
*/

#ifndef OSCL_DEFALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif

#ifndef OSCL_REFCOUNTER_H_INCLUDED
#include "oscl_refcounter.h"
#endif

#ifndef OSCL_MEM_BASIC_FUNCTIONS_H_INCLUDED
#include "oscl_mem_basic_functions.h"
#endif

#ifndef OSCL_ERROR_H_INCLUDED
#include "oscl_error.h"
#endif

#ifndef OSCL_EXCEPTION_H_INCLUDED
#include "oscl_exception.h"
#endif

#define OSCL_DISABLE_WARNING_TRUNCATE_DEBUG_MESSAGE
#include "osclconfig_compiler_warnings.h"

// This macro is defined is osclconfig_compiler_warnings.h
// This GCC #pragma turns off compiler warning for the rest of this header file
// This needs to be done because with the GCC 4.1 toolchain, many compiler warnings
// are generated because Oscl_MemAllocator and Oscl_MemBasicAllocator have virtual functions, but
// no virtual destructor.
// An attempt has been made to add the virtual destructors, however, it resulted
// in run time crashes indicative of double freeing of memory.
// This is a temporary fix, until the crashes are resolved.
//
#ifdef OSCL_DISABLE_GCC_WARNING_SYSTEM_HEADER
#pragma GCC system_header
#endif

/** \class OsclMemAllocator
** A simple allocator class.  Configurable as to whether
** this goes through the memory manager or not.
**
*/
class OsclMemAllocator : public Oscl_DefAlloc
{
    public:
        /** This API throws an exception when malloc returns NULL.
          * n must be greater than 0.
          *
          * @return pointer (or Leave with OsclErrNoMemory )
          *
          */
        OsclAny* allocate(const uint32 n)
        {
#if(OSCL_BYPASS_MEMMGT)
            OsclAny* p = _oscl_malloc(n);
            if (!p)
                OsclError::LeaveIfNull(p);
#if OSCL_MEM_FILL_WITH_PATTERN
            oscl_memset(p, 0x55, n);
#endif
            return (p);
#elif (PVMEM_INST_LEVEL>0)
            //this is really a usage error-- caller should provide file & line.
            //set a debug breakpoint here...
            return allocate_fl(n, __FILE__, __LINE__);
#else
            return allocate_fl(n, NULL, 0);
#endif
        }

#if(!OSCL_BYPASS_MEMMGT)
        OsclAny* allocate_fl(const uint32 n, const char * file_name, const int line_num)
        {
            OsclAny* p = _oscl_default_audit_malloc(n, file_name, line_num);
            if (!p)
                OsclError::LeaveIfNull(p);
#if OSCL_MEM_FILL_WITH_PATTERN
            oscl_memset(p, 0x55, n);
#endif
            return (p);
        }
#endif

        void deallocate(OsclAny* p)
        {
            if (p)
                OSCL_FREE(p);
        }
};


/** \class OsclMemBasicAllocator
** A simple allocator class that does not use the memory management.
**
** Note: this allocator is for internal use by Oscl only.  Higher
** level code should use OsclMemAllocator.
**
*/
class OsclMemBasicAllocator : public Oscl_DefAlloc
{
    public:
        /** This API throws an exception when malloc returns NULL.
          * n must be greater than 0.
          *
          * @return pointer (or Leave with OsclErrNoMemory )
          *
          */
        OsclAny* allocate(const uint32 n)
        {
            OsclAny* p = _oscl_malloc(n);
            OsclError::LeaveIfNull(p);
#if OSCL_MEM_FILL_WITH_PATTERN
            oscl_memset(p, 0x55, n);
#endif
            return (p);
        }

        void deallocate(OsclAny* p)
        {
            if (p)
                _oscl_free(p);
        }
};

/** \class OsclMemAllocDestructDealloc
** An OsclAllocDestructDealloc class that uses
** OsclMemAllocator.
*/
template <class T> class OsclMemAllocDestructDealloc : public OsclAllocDestructDealloc
{
    public:
#if !(OSCL_BYPASS_MEMMGT)
        OsclAny* allocate_fl(const uint32 size, const char * file_name, const int line_num)
        {
            return alloc.allocate_fl(size, file_name, line_num);
        }
#endif
        OsclAny* allocate(const uint32 size)
        {
#if(OSCL_BYPASS_MEMMGT)
            return alloc.allocate(size);
#elif(PVMEM_INST_LEVEL>0)
            //this is really a usage error-- caller should provide file & line.
            //set a debug breakpoint here...
            return allocate_fl(size, __FILE__, __LINE__);
#else
            return allocate_fl(size, NULL, 0);
#endif
        }
        void deallocate(OsclAny* p)
        {
            alloc.deallocate(p);
        }
        void destruct_and_dealloc(OsclAny* p)
        {
            T* ptr = reinterpret_cast<T*>(p);
            ptr->~T();
            deallocate(p);
            OSCL_UNUSED_ARG(ptr); // removes warning on some compilers
        }
    private:
        OsclMemAllocator alloc;
};

/** \class OsclMemBasicAllocDestructDealloc
** An OsclAllocDestructDealloc class that uses
** OsclMemBasicAllocator.
*/
template <class T> class OsclMemBasicAllocDestructDealloc : public OsclAllocDestructDealloc
{
    public:
        OsclAny* allocate(const uint32 size)
        {
#if(OSCL_BYPASS_MEMMGT)
            return alloc.allocate(size);
#else
            return alloc.allocate_fl(size, NULL, 0);
#endif
        }
        void deallocate(OsclAny* p)
        {
            alloc.deallocate(p);
        }
        void destruct_and_dealloc(OsclAny* p)
        {
            T* ptr = reinterpret_cast<T*>(p);
            ptr->~T();
            deallocate(p);
            OSCL_UNUSED_ARG(ptr); // removes warning on some compilers
        }
    private:
        OsclMemBasicAllocator alloc;
};

/**
 * This class is used to get a pointer to the global audit object.
 */

class OsclMemAudit;
class OsclMemGlobalAuditObject
{
    public:
        typedef OsclMemAudit audit_type;
        /**
         * returns the global audit object.  For use
         * in macros only-- not a public API.
         */
        OSCL_IMPORT_REF static audit_type* getGlobalMemAuditObject();

    private:
        /**
         * creates the global audit object
         */
        static void createGlobalMemAuditObject();

        /**
         * deletes the global audit object
         */
        static void deleteGlobalMemAuditObject();

        friend class OsclMem;
};

/**
* HeapBase is the base class for all classes that allocates memory.
*
* HeapBase has overloaded new and delete operators.
*
* Derived from _OsclHeapBase providing CBase* alike pointer and virtual destructor for cleanupstack
* to Push and Pop for cleanup when leave occurs.
*
* HeapBase has a virtual destructor which calls the destructor of all the derived classes.
*/

class HeapBase : public _OsclHeapBase
{
    public:
#if (OSCL_HAS_HEAP_BASE_SUPPORT)

#if(!OSCL_BYPASS_MEMMGT)
        static void* operator new(size_t aSize, const char *aFile = NULL, const int aLine = 0)
        {
#if(PVMEM_INST_LEVEL>0)
            //in case NULL is passed in, record this file & line #
            if (!aFile)
                return _oscl_default_audit_new(aSize, __FILE__, __LINE__);
#endif
            return _oscl_default_audit_new(aSize, aFile, aLine);
        }
#else
        static void* operator new(size_t aSize)
        {
            return _oscl_default_new(aSize);
        }
#endif

        static void* operator new[](size_t aSize)
        {
#if(!OSCL_BYPASS_MEMMGT)
            return _oscl_default_audit_new(aSize);
#else
            return _oscl_default_new(aSize);
#endif
        }

        static void* operator new[](size_t aSize, const char *aFile = NULL, const int aLine = 0)
        {
#if(!OSCL_BYPASS_MEMMGT)
#if(PVMEM_INST_LEVEL>0)
            //in case NULL is passed in, record this file & line #
            if (!aFile)
                return _oscl_default_audit_new(aSize, __FILE__, __LINE__);
#endif
            return _oscl_default_audit_new(aSize, aFile, aLine);
#else
            OSCL_UNUSED_ARG(aFile);
            OSCL_UNUSED_ARG(aLine);
            return _oscl_default_new(aSize);
#endif
        }

        static void* operator new(size_t aSize, void *aPtr)
        {
            return aPtr;
        }

        static void operator delete(void* aPtr)
        {
#if(!OSCL_BYPASS_MEMMGT)
            _oscl_audit_free(aPtr);
#else
            _oscl_free(aPtr);
#endif
        }

        static void operator delete[](void* aPtr)
        {
#if(!OSCL_BYPASS_MEMMGT)
            _oscl_audit_free(aPtr);
#else
            _oscl_free(aPtr);
#endif
        }
#endif //OSCL_HAS_HEAP_BASE_SUPPORT
        HeapBase() {};
        virtual ~HeapBase() {};
};

/** Internal-use macro to catch leaves
*in constructors.  If the constructor leaves,
*this will free the memory
*before allowing the leave to propagate to the next
*level.  It is the constructor's responsibility to
*cleanup any memory in the partially constructed
*object before leaving.  This cleanup may include
*cleaning up the base class using the OSCL_CLEANUP_BASE_CLASS
*macro.
*
* @param exp: expression to allocate memory.
* @param Tptr:variable to hold result.
* @param T: type
* @param params: constructor arg list
* @param freeFunc: delete or free function.
*/
#define _OSCL_TRAP_NEW(exp,freeFunc,T_ptr,T,params)\
{\
    int32 __err;\
    OsclAny*__ptr=exp;\
    OSCL_TRY(__err,T_ptr=new(__ptr) T params;);\
    if(__err){\
        freeFunc(__ptr);\
        T_ptr=NULL;\
        OsclError::Leave(__err);\
    }\
}

/**
 * This macro is used to cleanup the
 * base class in a derived-class constructor
 * just before a leave occurs.
 *
 * @param T: base class name.
 */
#ifdef PVERROR_IMP_CPP_EXCEPTIONS
//when using C++ exceptions, base class cleanup is automatic
#define _OSCL_CLEANUP_BASE_CLASS(T)
#else
//otherwise the destructor needs to be called explicitly.
#define _OSCL_CLEANUP_BASE_CLASS(T) this->T::~T()
#endif


/*! @} */


#if (!OSCL_DISABLE_INLINES)
#include "oscl_mem.inl"
#endif

#endif // OSCL_MEM_H_INCLUDED



/*! @} */

