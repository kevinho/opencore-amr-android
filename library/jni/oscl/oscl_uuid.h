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
/**
 *  @file oscl_uuid.h
 *  @brief This file defines the OSCL UUID structure used for unique
 *  identifiers as well as the short (32-bit) identifiers OsclUid32.
 */

#ifndef OSCL_UUID_H_INCLUDED
#define OSCL_UUID_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_MEM_BASIC_FUNCTIONS_H
#include "oscl_mem_basic_functions.h"
#endif

#ifndef OSCL_STRING_UTILS_H
#include "oscl_string_utils.h"
#endif

#ifndef OSCL_STDSTRING_H_INCLUDED
#include "oscl_stdstring.h"
#endif
// __cplusplus

#define EMPTY_UUID PVUuid(0,0,0,0,0,0,0,0,0,0,0)

typedef uint32 OsclUid32;
const char PV_CHAR_CLOSE_BRACKET = ')';
const char PV_CHAR_COMMA = ',';
/**
 * OSCL UUID structure used for unique identification of modules and interfaces.
 */
struct OsclUuid
{
#define BYTES_IN_UUID_ARRAY 8

    OsclUuid()
    {
        oscl_memset(this, 0, sizeof(OsclUuid));
    }

    OsclUuid(uint32 l, uint16 w1, uint16 w2, uint8 b1, uint8 b2, uint8 b3,
             uint8 b4, uint8 b5, uint8 b6, uint8 b7, uint8 b8)
    {
        data1 = l;
        data2 = w1;
        data3 = w2;
        data4[0] = b1;
        data4[1] = b2;
        data4[2] = b3;
        data4[3] = b4;
        data4[4] = b5;
        data4[5] = b6;
        data4[6] = b7;
        data4[7] = b8;
    }

    //The OSCL UUID structure takes in a string parameter
    //Expected string input for the OsclString should be like:
    //(0xa054569c,0x24c5,0x452e,0x99,0x77,0x87,0x4b,0xca,0x79,0xd3,0xaf)

    OsclUuid(const char* aUuidString)
    {
        //Initialize all data members to 0 to begin with
        data1 = data2 = data3 = 0;

        for (int ii = 0; ii < 8; ++ii)
        {
            data4[ii] = 0;
        }

        if (!aUuidString)
        {
            return;
        }
        int uuidStrLen = oscl_strlen(aUuidString);

        if (uuidStrLen != 0)
        {
            const char* sptr = NULL, *eptr = NULL;
            int commaval = 0;
            sptr = aUuidString;
            ++sptr; //Increment over the starting parantheses '('
            eptr = sptr;
            for (int i = 0; i < uuidStrLen - 1 ; ++i)
            {
                if ((*eptr != PV_CHAR_COMMA) && (*eptr != PV_CHAR_CLOSE_BRACKET)) //Increment the pointer unless you get to the ","
                {                               //The comma signifies the beginning of the new OsclUuid parameter
                    ++eptr;
                }
                else
                {
                    sptr = sptr + 2;//Move over the 0x characters in the beginning of the hex value;
                    ++commaval;
                    switch (commaval)
                    {
                        case 1:
                        {
                            PV_atoi(sptr , 'x', eptr - sptr, data1);
                            break;
                        }
                        case 2:
                        {
                            uint32 tempdata2 = 0;
                            PV_atoi(sptr , 'x', (eptr - sptr), tempdata2);
                            data2 = (uint16)tempdata2;
                            break;
                        }
                        case 3:
                        {
                            uint32 tempdata3 = 0;
                            PV_atoi(sptr , 'x', (eptr - sptr), tempdata3);
                            data3 = (uint16)tempdata3;
                            break;
                        }
                        case 4:
                        {
                            uint32 tempdata4_0 = 0;
                            PV_atoi(sptr , 'x', (eptr - sptr), tempdata4_0);
                            data4[0] = (uint8)tempdata4_0;
                            break;
                        }
                        case 5:
                        {
                            uint32 tempdata4_1 = 0;
                            PV_atoi(sptr , 'x', (eptr - sptr), tempdata4_1);
                            data4[1] = (uint8)tempdata4_1;
                            break;
                        }
                        case 6:
                        {
                            uint32 tempdata4_2 = 0;
                            PV_atoi(sptr , 'x', (eptr - sptr), tempdata4_2);
                            data4[2] = (uint8)tempdata4_2;
                            break;
                        }
                        case 7:
                        {
                            uint32 tempdata4_3 = 0;
                            PV_atoi(sptr , 'x', (eptr - sptr), tempdata4_3);
                            data4[3] = (uint8)tempdata4_3;
                            break;
                        }
                        case 8:
                        {
                            uint32 tempdata4_4 = 0;
                            PV_atoi(sptr , 'x', (eptr - sptr), tempdata4_4);
                            data4[4] = (uint8)tempdata4_4;
                            break;
                        }
                        case 9:
                        {
                            uint32 tempdata4_5 = 0;
                            PV_atoi(sptr , 'x', (eptr - sptr), tempdata4_5);
                            data4[5] = (uint8)tempdata4_5;
                            break;
                        }
                        case 10:
                        {
                            uint32 tempdata4_6 = 0;
                            PV_atoi(sptr , 'x', (eptr - sptr), tempdata4_6);
                            data4[6] = (uint8)tempdata4_6;
                            break;
                        }
                        case 11:
                        {
                            uint32 tempdata4_7 = 0;
                            PV_atoi(sptr, 'x', (eptr - sptr), tempdata4_7);
                            data4[7] = (uint8)tempdata4_7;
                            break;
                        }
                    }
                    if (*eptr == PV_CHAR_CLOSE_BRACKET) //Break from the loop on finding
                    {
                        break;
                    }
                    ++eptr;
                    sptr = eptr;
                }
            }
        }
    }

    OsclUuid(const OsclUuid &uuid)
    {
        oscl_memcpy(this, &uuid, sizeof(OsclUuid));
    }

    OsclUuid &operator=(const OsclUuid& src)
    {
        oscl_memcpy(this, &src, sizeof(OsclUuid));
        return *this;
    }

    bool operator==(const OsclUuid& src) const
    {
        if (data1 != src.data1 || data2 != src.data2 || data3 != src.data3)
        {
            return false;
        }

        for (int ii = 0; ii < 8; ++ii)
        {
            if (data4[ii] != src.data4[ii])
            {
                return false;
            }
        }

        return true;
    }

    bool operator!=(const OsclUuid &src) const
    {
        return !(*this == src);
    }

    uint32  data1;
    uint16  data2;
    uint16  data3;
    uint8   data4[BYTES_IN_UUID_ARRAY];
};

#endif

