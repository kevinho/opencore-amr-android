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

//                 O S C L _ B I N _ S T R E A M

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*
** Constants
*/

const static int16 NO_BITS_IN_BYTE = 8;
const static uint8 BYTE_MASK = 0xff;

#include "oscl_mem_basic_functions.h"

OSCL_INLINE bool OsclBinStream::good()
{
    return state == GOOD_STATE;
}

OSCL_INLINE bool OsclBinStream::eof()
{
    return state == EOF_STATE;
}

OSCL_INLINE bool OsclBinStream::fail()
{
    return state == FAIL_STATE;
}

OSCL_INLINE void OsclBinStream::Attach(void * buffer, uint32 l_length)
{
    fragsLeft = 0;
    pBasePosition = (uint8 *)buffer;
    pPosition = pBasePosition;
    length = l_length;
    state = GOOD_STATE;
    specialFragBuffer.ptr = buffer;
    specialFragBuffer.len = l_length;
    numFrags = 1;
    firstFragPtr = &specialFragBuffer;
}

OSCL_INLINE void OsclBinStream::Attach(const uint32 numFragments, const OsclMemoryFragment * fragPtr)
{
    pBasePosition = (uint8 *)fragPtr->ptr;
    pPosition = pBasePosition;
    numFrags = numFragments;
    firstFragPtr  = fragPtr;
    length = fragPtr->len;
    if (numFragments > 1)
    {
        fragsLeft = numFragments - 1;
        nextFragPtr = fragPtr + 1;
    }
    else
    {
        fragsLeft = 0;
    }
    state = GOOD_STATE;
}

OSCL_INLINE uint32 OsclBinStream::tellg()
{
    uint32 pos = 0;
    for (int ii = 0; ii < numFrags - fragsLeft - 1; ii++)
    {
        pos += firstFragPtr[ii].len;
    }
    pos += PositionInBlock();

    return pos;
}

OSCL_INLINE void OsclBinStream::Seek(uint32 absPosition)
{
    uint32 pos = 0;
    int fragIndex;

    for (fragIndex = 0;
            fragIndex < numFrags && absPosition >= pos + firstFragPtr[fragIndex].len;
            fragIndex++)
    {
        pos += firstFragPtr[fragIndex].len;
    }

    if (fragIndex >= numFrags)
    {
        fragsLeft = 0;
        pBasePosition = (uint8 *)firstFragPtr[numFrags-1].ptr;
        length = firstFragPtr[numFrags-1].len;
        pPosition = pBasePosition + length;
        if (absPosition == pos)
        {
            state = EOF_STATE;
        }
        else
        {
            state = FAIL_STATE;
        }
        return;
    }

    // otherwise there is more data
    nextFragPtr = &firstFragPtr[fragIndex + 1];
    fragsLeft = numFrags - fragIndex - 1;
    pBasePosition = (uint8 *)firstFragPtr[fragIndex].ptr;
    length = firstFragPtr[fragIndex].len;
    uint32 reqBytes = absPosition - pos;
    if (reqBytes <= length)
    {
        pPosition = pBasePosition + reqBytes;
    }
    else
    {
        pPosition = pBasePosition + length;
        state = FAIL_STATE;
    }
}

OSCL_INLINE uint32 OsclBinStream::PositionInBlock()
{
    return pPosition - pBasePosition;
}

OSCL_INLINE void OsclBinStream::seekFromCurrentPosition(int32 offset)
{
    Seek(tellg() + offset);
}

OSCL_INLINE bool OsclBinStream::ReserveSpace(uint32 size)
{
    if (fail())
    {
        return false;
    }
    uint32 newSize = PositionInBlock() + size;
    if (newSize > length)
    {
        state = FAIL_STATE;
        return false;
    }
    if (newSize == length)
    {
        state = EOF_STATE;
    }
    return true;
}

OSCL_INLINE bool OsclBinStream::HaveRoomInCurrentBlock(uint32 size)
{
    uint32 pos = PositionInBlock() + size;
    if (pos < length)
    {
        return true;
    }
    if (pos == length && fragsLeft == 0)
    {
        state = EOF_STATE;
    }
    return (pos <= length);
}

/*
** Class OsclBinIStream
**      This class implements the basic stream functions for an input stream.
*/
OSCL_INLINE uint8 OsclBinIStream::Read_uint8()
{
    if (HaveRoomInCurrentBlock(sizeof(uint8)))
    {
        return (*pPosition++);
    }
    if (fragsLeft)
    {
        pBasePosition = (uint8 *)nextFragPtr->ptr;
        pPosition = pBasePosition;
        length = nextFragPtr->len;
        fragsLeft--;
        nextFragPtr++;
        return (*pPosition++);
    }
    state = FAIL_STATE;
    return 0;
}

OSCL_INLINE OsclBinIStream & OsclBinIStream::get(
    int8 * data,    /* Pointer to the place to store the bytes read */
    int32 size    /* Number of bytes to read */
)
{
    if (HaveRoomInCurrentBlock(size))
    {
        oscl_memcpy(data, pPosition, size);
        pPosition += size;
    }
    else
    {
        uint32 pos = PositionInBlock();
        uint32 bytesToCopy = length - pos;
        oscl_memcpy(data, pPosition, bytesToCopy);
        data += bytesToCopy;
        uint32 bytesLeft = size - bytesToCopy;
        while (bytesLeft > 0 && fragsLeft)
        {
            pBasePosition = (uint8 *)nextFragPtr->ptr;
            pPosition = pBasePosition;
            length = nextFragPtr->len;
            fragsLeft--;
            nextFragPtr++;

            if (bytesLeft <= length)
            {
                bytesToCopy = bytesLeft;
            }
            else
            {
                bytesToCopy = length;
            }

            oscl_memcpy(data, pPosition, bytesToCopy);
            data += bytesToCopy;
            pPosition += bytesToCopy;
            bytesLeft -= bytesToCopy;
        }
    }
    return *this;
}

/*
** Class OsclBinIStreamLittleEndian
**      This class implements a binary input stream using little endian byte ordering
*/
OSCL_INLINE OsclBinIStreamLittleEndian & OsclBinIStreamLittleEndian::operator>>(int8 & data)
{
    data = int8(Read_uint8());
    return *this;
}

OSCL_INLINE OsclBinIStreamLittleEndian & OsclBinIStreamLittleEndian::operator>>(uint8 & data)
{
    data = Read_uint8();
    return *this;
}

OSCL_INLINE OsclBinIStreamLittleEndian & OsclBinIStreamLittleEndian::operator>>(int16 & data)
{
    data = int16(Read_uint16());

    return *this;
}

OSCL_INLINE OsclBinIStreamLittleEndian & OsclBinIStreamLittleEndian::operator>>(uint16 & data)
{
    data = Read_uint16();

    return *this;
}

OSCL_INLINE OsclBinIStreamLittleEndian & OsclBinIStreamLittleEndian::operator>>(int32 & data)
{
    data = int32(Read_uint32());
    return *this;
}

OSCL_INLINE OsclBinIStreamLittleEndian & OsclBinIStreamLittleEndian::operator>>(uint32 & data)
{
    data = Read_uint32();
    return *this;
}

OSCL_INLINE uint16 OsclBinIStreamLittleEndian::Read_uint16()
{
    if (HaveRoomInCurrentBlock(sizeof(uint16)))
    {
#if defined(BYTE_ORDER_LITTLE_ENDIAN) && !defined(INTEGERS_WORD_ALIGNED)
        uint16 *ptr = (uint16 *)pPosition;
        pPosition += sizeof(uint16);
        return *ptr;
#else
        uint16 byteB = *pPosition++;
        uint16 byteA = *pPosition++;

        return ((byteA << NO_BITS_IN_BYTE) | byteB);
#endif
    }
    else
    {
        uint16 byteB = Read_uint8();
        uint16 byteA = Read_uint8();
        return ((byteA << NO_BITS_IN_BYTE) | byteB);
    }
}

OSCL_INLINE uint32 OsclBinIStreamLittleEndian::Read_uint32()
{
    if (HaveRoomInCurrentBlock(sizeof(uint32)))
    {
#if defined(BYTE_ORDER_LITTLE_ENDIAN) && !defined(INTEGERS_WORD_ALIGNED)
        uint32 *ptr = (uint32 *)pPosition;
        pPosition += sizeof(uint32);
        return *ptr;
#else
        uint32 result;
        uint8 byteD = *pPosition++;
        uint8 byteC = *pPosition++;
        uint8 byteB = *pPosition++;
        uint8 byteA = *pPosition++;

        result = byteA;
        result = (result << NO_BITS_IN_BYTE) | byteB;
        result = (result << NO_BITS_IN_BYTE) | byteC;
        result = (result << NO_BITS_IN_BYTE) | byteD;

        return result;
#endif
    }
    else
    {
        uint32 result;
        uint8 byteD = Read_uint8();
        uint8 byteC = Read_uint8();
        uint8 byteB = Read_uint8();
        uint8 byteA = Read_uint8();

        result = byteA;
        result = (result << NO_BITS_IN_BYTE) | byteB;
        result = (result << NO_BITS_IN_BYTE) | byteC;
        result = (result << NO_BITS_IN_BYTE) | byteD;

        return result;
    }
}



/*
** Class OsclBinIStreamBigEndian
**      This class implements a binary input stream using big endian byte ordering
*/
OSCL_INLINE void OsclBinIStreamBigEndian::Read(int8 & data)
{
    data = int8(Read_uint8());
}

OSCL_INLINE void OsclBinIStreamBigEndian::Read(uint8 & data)
{
    data = Read_uint8();
}

OSCL_INLINE void OsclBinIStreamBigEndian::Read(int16 & data)
{
    data = int16(Read_uint16());
}

OSCL_INLINE void OsclBinIStreamBigEndian::Read(uint16 & data)
{
    data = Read_uint16();
}

OSCL_INLINE void OsclBinIStreamBigEndian::Read(int32 & data)
{
    data = int32(Read_uint32());
}

OSCL_INLINE void OsclBinIStreamBigEndian::Read(uint32 & data)
{
    data = Read_uint32();
}

OSCL_INLINE OsclBinIStreamBigEndian & OsclBinIStreamBigEndian::operator>>(int8 & data)
{
    data = int8(Read_uint8());
    return *this;
}

OSCL_INLINE OsclBinIStream & OsclBinIStreamBigEndian::operator>>(uint8 & data)
{
    data = Read_uint8();
    return *this;
}

OSCL_INLINE OsclBinIStreamBigEndian & OsclBinIStreamBigEndian::operator>>(int16 & data)
{
    data = int16(Read_uint16());
    return *this;
}

OSCL_INLINE OsclBinIStreamBigEndian & OsclBinIStreamBigEndian::operator>>(uint16 & data)
{
    data = Read_uint16();
    return *this;
}

OSCL_INLINE OsclBinIStreamBigEndian & OsclBinIStreamBigEndian::operator>>(int32 & data)
{
    data = int32(Read_uint32());
    return *this;
}

OSCL_INLINE OsclBinIStreamBigEndian & OsclBinIStreamBigEndian::operator>>(uint32 & data)
{
    data = Read_uint32();
    return *this;
}

OSCL_INLINE uint16 OsclBinIStreamBigEndian::Read_uint16()
{
    if (HaveRoomInCurrentBlock(sizeof(uint16)))
    {
#if defined(BYTE_ORDER_BIG_ENDIAN) && !defined(INTEGERS_WORD_ALIGNED)
        uint16 *ptr = (uint16 *)pPosition;
        pPosition += sizeof(uint16);
        return *ptr;
#else
        uint16 byteA = *pPosition++;
        uint16 byteB = *pPosition++;

        return ((byteA << NO_BITS_IN_BYTE) | byteB);
#endif
    }
    else
    {
        uint16 byteA = Read_uint8();
        uint16 byteB = Read_uint8();

        return ((byteA << NO_BITS_IN_BYTE) | byteB);
    }
}

OSCL_INLINE uint32 OsclBinIStreamBigEndian::Read_uint32()
{
    if (HaveRoomInCurrentBlock(sizeof(uint32)))
    {
#if defined(BYTE_ORDER_BIG_ENDIAN) && !defined(INTEGERS_WORD_ALIGNED)
        uint32 *ptr = (uint32 *)pPosition;
        pPosition += sizeof(uint32);
        return *ptr;
#else
        uint32 result;

        result = *pPosition++;
        result = (result << NO_BITS_IN_BYTE) | *pPosition++;
        result = (result << NO_BITS_IN_BYTE) | *pPosition++;
        result = (result << NO_BITS_IN_BYTE) | *pPosition++;

        return result;
#endif
    }
    else
    {
        uint32 result;

        result = Read_uint8();
        result = (result << NO_BITS_IN_BYTE) | Read_uint8();
        result = (result << NO_BITS_IN_BYTE) | Read_uint8();
        result = (result << NO_BITS_IN_BYTE) | Read_uint8();

        return result;
    }
}


/*
** Class OsclBinOStream
**      This class implements the basic stream functions for an output stream.
*/

OSCL_INLINE OsclBinOStream & OsclBinOStream::write(
    const int8 * data,      /* data to store */
    int32 size      /* length of data to store */
)
{
    if (ReserveSpace(size))
    {
        oscl_memcpy(pPosition, data, size);
        pPosition += size;
    }
    return *this;
}

/*
** Class OsclBinOStreamLittleEndian
**      This class implements a binary output stream using little endian byte ordering
*/
OSCL_INLINE OsclBinOStreamLittleEndian & OsclBinOStreamLittleEndian::operator<<(const int8 & data)
{
    if (ReserveSpace(sizeof(data)))
    {
        *((int8 *)pPosition) = data;
        pPosition++;
    }
    return *this;
}

OSCL_INLINE OsclBinOStreamLittleEndian & OsclBinOStreamLittleEndian::operator<<(const uint8 & data)
{
    if (ReserveSpace(sizeof(data)))
    {
        *pPosition++ = data;
    }
    return *this;
}

OSCL_INLINE OsclBinOStreamLittleEndian & OsclBinOStreamLittleEndian::operator<<(const int16 & data)
{
    if (ReserveSpace(sizeof(data)))
    {
        WriteUnsignedShort((uint16)data);
    }

    return *this;
}

OSCL_INLINE OsclBinOStreamLittleEndian & OsclBinOStreamLittleEndian::operator<<(const uint16 & data)
{
    if (ReserveSpace(sizeof(data)))
    {
        WriteUnsignedShort(data);
    }
    return *this;
}

OSCL_INLINE OsclBinOStreamLittleEndian & OsclBinOStreamLittleEndian::operator<<(const int32 & data)
{
    if (ReserveSpace(sizeof(data)))
    {
        WriteUnsignedLong(uint32(data));
    }

    return *this;
}

OSCL_INLINE OsclBinOStreamLittleEndian & OsclBinOStreamLittleEndian::operator<<(const uint32 & data)
{
    if (ReserveSpace(sizeof(data)))
    {
        WriteUnsignedLong(data);
    }

    return *this;
}

OSCL_INLINE void OsclBinOStreamLittleEndian::WriteUnsignedShort(const uint16 data)
{
#if defined(BYTE_ORDER_LITTLE_ENDIAN) && !defined(INTEGERS_WORD_ALIGNED)
    uint16 *ptr = (uint16 *)pPosition;
    pPosition += sizeof(uint16);
    *ptr = data;
#else
    uint8 byteB = (uint8)data;
    uint8 byteA = data >> NO_BITS_IN_BYTE;

    *pPosition++ = byteB;
    *pPosition++ = byteA;
#endif
}

OSCL_INLINE void OsclBinOStreamLittleEndian::WriteUnsignedLong(const uint32 data)
{
#if defined(BYTE_ORDER_LITTLE_ENDIAN) && !defined(INTEGERS_WORD_ALIGNED)
    uint32 *ptr = (uint32 *)pPosition;
    pPosition += sizeof(uint32);
    *ptr = data;
#else
    uint32 temp = data;
    *pPosition++ = (uint8)temp;
    temp >>= NO_BITS_IN_BYTE;
    *pPosition++ = (uint8)temp;
    temp >>= NO_BITS_IN_BYTE;
    *pPosition++ = (uint8)temp;
    temp >>= NO_BITS_IN_BYTE;
    *pPosition++ = (uint8)temp;
#endif
}


OSCL_INLINE OsclBinOStreamBigEndian & OsclBinOStreamBigEndian::operator<<(const int8 & data)
{
    if (ReserveSpace(sizeof(data)))
    {
        *((int8 *)pPosition) = data;
        pPosition++;
    }

    return *this;
}

OSCL_INLINE OsclBinOStreamBigEndian & OsclBinOStreamBigEndian::operator<<(const uint8 & data)
{
    if (ReserveSpace(sizeof(data)))
    {
        *pPosition++ = data;
    }
    return *this;
}

OSCL_INLINE OsclBinOStreamBigEndian & OsclBinOStreamBigEndian::operator<<(const int16 & data)
{
    if (ReserveSpace(sizeof(data)))
    {
        WriteUnsignedShort((uint16)data);
    }
    return *this;
}

OSCL_INLINE OsclBinOStreamBigEndian & OsclBinOStreamBigEndian::operator<<(const uint16 & data)
{
    if (ReserveSpace(sizeof(data)))
    {
        WriteUnsignedShort(data);
    }
    return *this;
}

OSCL_INLINE OsclBinOStreamBigEndian & OsclBinOStreamBigEndian::operator<<(const int32 & data)
{
    if (ReserveSpace(sizeof(data)))
    {
        WriteUnsignedLong(uint32(data));
    }
    return *this;
}

OSCL_INLINE OsclBinOStreamBigEndian & OsclBinOStreamBigEndian::operator<<(const uint32 & data)
{
    if (ReserveSpace(sizeof(data)))
    {
        WriteUnsignedLong(data);
    }

    return *this;
}

OSCL_INLINE void OsclBinOStreamBigEndian::WriteUnsignedShort(const uint16 data)
{
#if defined(BYTE_ORDER_BIG_ENDIAN) && !defined(INTEGERS_WORD_ALIGNED)
    uint16 *ptr = (uint16 *)pPosition;
    pPosition += sizeof(uint16);
    *ptr = data;
#else
    uint8 byteB = (uint8)data;
    uint8 byteA = data >> NO_BITS_IN_BYTE;

    *pPosition++ = byteA;
    *pPosition++ = byteB;
#endif
}

OSCL_INLINE void OsclBinOStreamBigEndian::WriteUnsignedLong(const uint32 data)
{
#if defined(BYTE_ORDER_BIG_ENDIAN) && !defined(INTEGERS_WORD_ALIGNED)
    uint32 *ptr = (uint32 *)pPosition;
    pPosition += sizeof(uint32);
    *ptr = data;
#else
    uint32 temp = data;
    uint8 byteD = (uint8)temp;
    temp >>= NO_BITS_IN_BYTE;
    uint8 byteC = (uint8)temp;
    temp >>= NO_BITS_IN_BYTE;
    uint8 byteB = (uint8)temp;
    temp >>= NO_BITS_IN_BYTE;
    uint8 byteA = (uint8)temp;

    *pPosition++ = byteA;
    *pPosition++ = byteB;
    *pPosition++ = byteC;
    *pPosition++ = byteD;
#endif
}
