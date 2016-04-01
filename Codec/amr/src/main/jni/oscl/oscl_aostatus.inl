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
/** \file oscl_aostatus.inl
    \brief Inline functions for oscl_aostatus.h
*/


OSCL_INLINE OsclAOStatus::OsclAOStatus()
{}
OSCL_INLINE OsclAOStatus::OsclAOStatus(int32 aVal)
        : iStatus(aVal)
{}
OSCL_INLINE int32 OsclAOStatus::operator=(int32 aVal)
{
    return(iStatus = aVal);
}
OSCL_INLINE int32 OsclAOStatus::operator==(int32 aVal) const
{
    return(iStatus == aVal);
}
OSCL_INLINE int32 OsclAOStatus::operator!=(int32 aVal) const
{
    return(iStatus != aVal);
}
OSCL_INLINE int32 OsclAOStatus::operator>=(int32 aVal) const
{
    return(iStatus >= aVal);
}
OSCL_INLINE int32 OsclAOStatus::operator<=(int32 aVal) const
{
    return(iStatus <= aVal);
}
OSCL_INLINE int32 OsclAOStatus::operator>(int32 aVal) const
{
    return(iStatus > aVal);
}
OSCL_INLINE int32 OsclAOStatus::operator<(int32 aVal) const
{
    return(iStatus < aVal);
}
OSCL_INLINE int32 OsclAOStatus::Value() const
{
    return(iStatus);
}




