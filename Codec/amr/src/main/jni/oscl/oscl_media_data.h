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

//               O S C L _ M E D I A _ D A T A

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclutil OSCL Util
 *
 * @{
 */


/** \file oscl_media_data.h
    \brief Defines a container class for media data made up of a collection of memory fragments.
*/

#ifndef OSCL_MEDIA_DATA_H
#define OSCL_MEDIA_DATA_H

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_MEM_BASIC_FUNCTIONS_H_INCLUDED
#include "oscl_mem_basic_functions.h"
#endif

#ifndef OSCL_MEDIA_STATUS_H_INCLUDED
#include "oscl_media_status.h"
#endif

typedef void (*BufferFreeFuncPtr)(void *);

typedef uint32 MediaTimestamp;

template <class T> class MemAllocator
{
    public:
        typedef T* pointer;
        //  virtual pointer allocate(size_t size, void *hint = 0) = 0;
        virtual pointer allocate(void * hint = 0, const int num_reserved_frags = 1) = 0;
        virtual void deallocate(pointer p) = 0;
        virtual ~MemAllocator() {}
};

class BufferState;
/* Buffer referee (how about the "whistle blower" ?*/
class BufferMgr
{
    public:
        virtual void BufferReleased(void* ptr, BufferState* state = NULL) = 0;
        virtual ~BufferMgr() {}
};


class BufferState
{
    public:
        BufferState(BufferFreeFuncPtr the_free_function, void *bufptr = 0) :
                refcount(0), ptr(bufptr), free_function(the_free_function), buf_mgr(0) {};

        BufferState(BufferMgr* the_buf_mgr = 0, void *bufptr = 0) :
                refcount(0), ptr(bufptr), free_function(0), buf_mgr(the_buf_mgr) {};

        void increment_refcnt()
        {
            ++refcount;
        };
        void decrement_refcnt()
        {
            if (!(--refcount))
            {
                if (buf_mgr) buf_mgr->BufferReleased(ptr, this);
                else if (free_function)
                {
                    free_function(ptr);
                    ptr = 0;
                }
            }
        };

        void bind(void *in_ptr, BufferFreeFuncPtr in_free_function)
        {
            refcount = 0;
            ptr = in_ptr, free_function = in_free_function;

        };
        void bind(void *in_ptr, BufferMgr* in_buf_mgr)
        {
            refcount = 0;
            ptr = in_ptr, buf_mgr = in_buf_mgr;
        };

        void *get_ptr()
        {
            return ptr;
        };
        int32 get_refcount()
        {
            return refcount;
        };

        BufferFreeFuncPtr get_free_function()
        {
            return free_function;
        };
        BufferMgr* get_buf_mgr()
        {
            return buf_mgr;
        };

        void reset()
        {
            refcount = 0;
            ptr = 0;
        };

    private:
        int32 refcount;
        void *ptr;
        BufferFreeFuncPtr free_function; // called to free the buffer
        BufferMgr* buf_mgr; // called to free the buffer
};


// BufferFragment is a special case of a memory
// fragment since it refers to fragments of a reference
// counted memory.  The name is used to make this explicit.
class BufferFragment : public OsclMemoryFragment {};


template <class ChainClass, uint32 max_frags> class BufFragGroup
{
    public:


        BufFragGroup(): next(0), num_fragments(0), length(0)
        {
            oscl_memset(fragments, 0, sizeof(fragments));
            oscl_memset(buffer_states, 0, sizeof(buffer_states));
        };

        virtual ~BufFragGroup() {}

        int32 GetMaxFrags() const
        {
            return max_frags;
        };
        int32 GetNumFrags() const
        {
            return num_fragments;
        };
        uint32 GetLength() const
        {
            return length;
        };

        BufferFragment * GetFragment(const int32 idx);

        BufferState * GetBufferState(const int32 idx);

        void AppendNext(ChainClass* next_ptr)
        {
            next = next_ptr;
        };
        ChainClass* GetNext() const
        {
            return next;
        };

        // make fragment array accessible to allow usage within socket
        // send functions.

    protected:


        // BufferFragment * GetFragment(const uint32 idx) const;



        virtual void Clear()
        {
            for (uint ii = 0 ; ii < num_fragments; ++ii)
            {
                if (buffer_states[ii])
                {
                    buffer_states[ii]->decrement_refcnt();
                }
            }
            num_fragments = length = 0;
            oscl_memset(buffer_states, 0, sizeof(buffer_states));
            oscl_memset(fragments, 0, sizeof(fragments));
        };

        BufFragStatusClass::status_t AddFragment(const BufferFragment& frag, BufferState* in_buffer_state,
                int32 location_offset = max_frags)
        {
            if (num_fragments > max_frags)
            {
                return BufFragStatusClass::TOO_MANY_FRAGS;
            }
            if (frag.ptr == NULL)
            {
                return BufFragStatusClass::EMPTY_FRAGMENT;
            }

            if (location_offset >= 0 && location_offset < (int32) num_fragments)
            {
                if (fragments[location_offset].ptr)
                {
                    // make room for the new fragment
                    oscl_memmove(fragments + location_offset + 1, fragments + location_offset,
                                 (num_fragments - location_offset)*sizeof(BufferFragment));
                    oscl_memmove(buffer_states + location_offset + 1, buffer_states + location_offset,
                                 (num_fragments - location_offset)*sizeof(BufferState*));
                    ++num_fragments; /* increment the number of fragments if a move
                          * is necessary.  If the location was empty, then
                          * has already been reserved and counted.
                          */
                }
                fragments[location_offset] = frag;
                buffer_states[location_offset] = in_buffer_state;
                if (in_buffer_state) in_buffer_state->increment_refcnt();
                length += frag.len;
                return BufFragStatusClass::BFG_SUCCESS;
            }

            // append the new fragment to the end
            fragments[num_fragments] = frag;
            buffer_states[num_fragments++] = in_buffer_state;
            length += frag.len;
            if (in_buffer_state) in_buffer_state->increment_refcnt();
            return BufFragStatusClass::BFG_SUCCESS;
        };

        BufferFragment fragments[max_frags];
        BufferState* buffer_states[max_frags];
        ChainClass* next;
        uint32 num_fragments;
        uint32 length;
};

template <class ChainClass, uint32 max_frags> inline BufferFragment *  BufFragGroup<ChainClass, max_frags> :: GetFragment(const int32 idx)
{
    if (idx < (int)num_fragments && idx >= 0)
    {
        return & fragments[idx];
    }
    else
    {
        return NULL;
    }
}

template <class ChainClass, uint32 max_frags> inline BufferState *  BufFragGroup<ChainClass, max_frags> :: GetBufferState(const int32 idx)
{
    if (idx < (int)num_fragments && idx >= 0)
    {
        return  buffer_states[idx];
    }
    else
    {
        return NULL;
    }
}


template <class ChainClass, uint32 max_frags, uint32 local_bufsize> class MediaData :
        public BufFragGroup<ChainClass, max_frags>
{

    public:
        MediaData(): timestamp(0), available_localbuf(local_bufsize), num_reserved_fragments(0) {};

        virtual ~MediaData() {}

        uint32 GetLocalBufsize() const
        {
            return local_bufsize;
        };
        MediaTimestamp GetTimestamp() const
        {
            return timestamp;
        };
        void SetTimestamp(MediaTimestamp in_timestamp)
        {
            timestamp = in_timestamp;
        };
        uint32 GetAvailableBufferSize() const
        {
            return available_localbuf;
        };



        // The input should be initialized so that the fragment length is the
        // request size.  The returned value will have a non-NULL pointer if the
        // request could be fulfilled.
        MediaStatusClass::status_t GetLocalFragment(BufferFragment& fragment)
        {
            if (fragment.len > available_localbuf)
            {
                fragment.ptr = NULL;
                fragment.len = available_localbuf;
                return MediaStatusClass::NOT_ENOUGH_SPACE;
            }

            fragment.ptr = localbuf + (local_bufsize - available_localbuf);
            available_localbuf -= fragment.len;
            return MediaStatusClass::BFG_SUCCESS;
        };

        virtual void Clear()
        {
            for (uint ii = 0 ; ii < this->num_fragments; ++ii)
            {
                if (this->buffer_states[ii])
                {
                    this->buffer_states[ii]->decrement_refcnt();
                }
            }
            this->num_fragments = 0;
            this->length = 0;
            oscl_memset(this->buffer_states, 0, sizeof(this->buffer_states));
            oscl_memset(this->fragments, 0, sizeof(this->fragments));
            this->available_localbuf = local_bufsize;
        };


        bool IsLocalData(const OsclMemoryFragment& frag) const
        {
            if (frag.ptr == NULL)
                return true;
            if ((uint8 *)frag.ptr >= localbuf)
            {
                if (((uint8 *)frag.ptr) + frag.len < localbuf + local_bufsize)
                {
                    return true;
                }
                return false;
            }
            return false;
        }

        int GetMediaSize() const
        {
            int num_reserved = 0;
            for (int ii = 0; ii < num_reserved_fragments; ii++)
                num_reserved += this->fragments[ii].len;

            return (this->length - num_reserved);
        }

        BufferFragment * GetMediaFragment(const uint32 idx)
        {
            uint32 tmp = idx + this->num_reserved_fragments;
            if (tmp > this->num_fragments)
            {
                return NULL;
            }
            else
            {
                return & this->fragments[tmp];
            }
        }

        uint32 GetNumMediaFrags(const uint32 idx) const
        {
            if (this->num_fragments >= this->num_reserved_fragments)
                return this->num_fragments - this->num_reserved_fragments;
            else return 0;
        }

    protected:
        // This function copies the fragment data to the local buffer
        MediaStatusClass::status_t AddLocalFragment(const BufferFragment& frag,
                int32 location_offset)
        {

            if (this->num_fragments > max_frags)
            {
                return MediaStatusClass::TOO_MANY_FRAGS;
            }

            if (frag.ptr == NULL || frag.len == 0)
            {
                return MediaStatusClass::EMPTY_FRAGMENT;
            }

            // see if there is enough room in the local buffer
            if (frag.len > available_localbuf)
            {
                return MediaStatusClass::NOT_ENOUGH_SPACE;
            }

            // get the fragment
            BufferFragment local_fragment;
            local_fragment.len = frag.len;
            local_fragment.ptr = localbuf + (local_bufsize - available_localbuf);
            available_localbuf -= frag.len;
            this->length += frag.len;

            oscl_memcpy(local_fragment.ptr, frag.ptr, frag.len*sizeof(uint8));

            if (location_offset >= 0 && location_offset < (int32) this->num_fragments)
            {
                if (this->fragments[location_offset].ptr)
                {
                    // make room for the new fragment
                    oscl_memmove(this->fragments + location_offset + 1, this->fragments + location_offset,
                                 (this->num_fragments - location_offset)*sizeof(BufferFragment));
                    oscl_memmove(this->buffer_states + location_offset + 1, this->buffer_states + location_offset,
                                 (this->num_fragments - location_offset)*sizeof(BufferState*));
                    ++this->num_fragments; /* increment the number of fragments if a move
                                * is necessary.  If the location was empty, then
                                * has already been reserved and counted.
                                */
                }
                this->fragments[location_offset] = local_fragment;
                this->buffer_states[location_offset] = NULL;
                return MediaStatusClass::BFG_SUCCESS;
            }

            // append the new fragment to the end
            this->fragments[this->num_fragments] = local_fragment;
            this->buffer_states[this->num_fragments++] = NULL;
            return MediaStatusClass::BFG_SUCCESS;
        };

        MediaTimestamp timestamp;
        uint8 localbuf[local_bufsize];
        uint32 available_localbuf;

        int num_reserved_fragments;
};

#endif

/*! @} */
