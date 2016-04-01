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

//                     O S C L _ M A P

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 *
 * @{
 */


/*! \file oscl_map.h
    \brief The file oscl_map.h defines the template class Oscl_Map which has a very similar API as the STL Map class (it basically provides a subset of the STL functionality).  Memory allocation is abstracted through the use of an allocator template parameter.
*/

#ifndef OSCL_MAP_H_INCLUDED
#define OSCL_MAP_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_TREE_H_INCLUDED
#include "oscl_tree.h"
#endif

#define OSCL_DISABLE_WARNING_TRUNCATE_DEBUG_MESSAGE
#include "osclconfig_compiler_warnings.h"

template <class T>
struct Oscl_Less
{
    bool operator()(const T& x, const T& y) const
    {
        return x < y ? true : false ;
    }
};

template <class V, class U>
struct Oscl_Select1st
{
    const U& operator()(const V& x) const
    {
        return x.first;
    }
};

/**
 * Oscl_Map Class.  A subset of STL::Map methods.
 * Oscl_Map is a sorted associative container that associates objects of type Key with objects of type T.
 * It is also a unique associative container, meaning that no two elements have the same key.  Oscl_Map
 * uses the key to speed lookup, insertion, and deletion of elements.  It is often superior to all other
 * containers when you need to lookup an element by key value.  Due to the underlying tree structure,
 * inserts and erases can be performed in logarithmic time, where a vector would take linear time in
 * some cases.
 */

template < class Key, class T, class Alloc, class Compare = Oscl_Less<Key> >
class Oscl_Map
{

    public:
        typedef Key key_type;
        typedef Compare key_compare;
        typedef Oscl_Pair<const Key, T> value_type;
        typedef Oscl_Map<Key, T, Alloc, Compare> self;

    private:
        typedef Oscl_Rb_Tree < key_type, value_type,
        Oscl_Select1st<value_type, key_type>,
        key_compare, Alloc > rep_type;
        rep_type t;  // red-black tree representing map

    public:
        typedef typename rep_type::pointer pointer;
        typedef typename rep_type::reference reference;
        typedef typename rep_type::const_reference const_reference;
        typedef typename rep_type::iterator iterator;
        typedef typename rep_type::const_iterator const_iterator;
        typedef typename rep_type::size_type size_type;

        class value_compare
        {
                friend class Oscl_Map<Key, T, Alloc, Compare>;
            protected :
                Compare comp;
                value_compare(Compare c) : comp(c) {}
            public:
                bool operator()(const value_type& x, const value_type& y) const
                {
                    return comp(x.first, y.first);
                }
        };

    public:

        /**
         * Creates an empty map using comp as the key compare object
         */
        Oscl_Map(const Compare& comp = Compare()) : t(comp) {}
//    Oscl_Map(const value_type* first, const value_type* last,
//        const Compare& comp = Compare()) : t(first, last, comp, false) {}

        /**
         * Oscl_Map copy constructor
         */
        Oscl_Map(const self& x) : t(x.t) {}
        /**
         * Oscl_Map assignment operator
         */
        self& operator=(const self& x)
        {
            t = x.t;
            return *this;
        }
        /**
         * Returns the key compare object used by the map
         */
        key_compare key_comp() const
        {
            return t.key_comp();
        }
        /**
         * Returns the value compare object used by the map
         */
        value_compare value_comp() const
        {
            return value_compare(t.key_comp());
        }
        /**
         * Returns an iterator pointing to the beginning of the map
         */
        iterator begin()
        {
            return t.begin();
        }
        /**
         * Returns a const iterator pointing to the beginning of the map
         */
        const_iterator begin() const
        {
            return t.begin();
        }
        /**
         * Returns an iterator pointing to the end of the map.
         */
        iterator end()
        {
            return t.end();
        }
        /**
         * Returns a const iterator pointing to the end of the map.
         */
        const_iterator end() const
        {
            return t.end();
        }
        /**
         * Returns true if map size is 0
         */
        bool empty() const
        {
            return t.empty();
        }
        /**
         * Returns the size of the map
         */
        size_type size() const
        {
            return t.size();
        }
        /**
         * Returns the maximum possible size of the map
         */
        size_type max_size() const
        {
            return t.max_size();
        }
        /**
         * Returns a reference to the object that is associated with a particular key.
         * If the map does not already contain such an object, operator[] inserts the default object value_type().
         */
        T& operator[](const key_type& k)
        {
            return (*((insert(value_type(k, T()))).first)).second;
        }
//    void swap(map<Key, T, Compare>& x) { t.swap(x.t); }


        typedef Oscl_Pair<iterator, bool> pair_iterator_bool;
        /**
         * Inserts x into the map
         */
        pair_iterator_bool insert(const value_type& x)
        {
            return t.insert_unique(x);
        }
        /**
         * Inserts x into the map using position as a hint as to where it should be inserted
         */
        iterator insert(iterator position, const value_type& x)
        {
            return t.insert_unique(position, x);
        }
        /**
         * Inserts the range [first,last) into the map
         */
        void insert(const value_type* first, const value_type* last)
        {
            t.insert_unique(first, last);
        }
        /**
         * Erases the element pointed to by position
         */
        void erase(iterator position)
        {
            t.erase(position);
        }
        /**
         * Erases the element with key x
         */
        size_type erase(const key_type& x)
        {
            return t.erase(x);
        }
        /**
         * Erases all elements in the range [first,last)
         */
        void erase(iterator first, iterator last)
        {
            t.erase(first, last);
        }
        /**
         * Erases all elements
         */
        void clear()
        {
            t.clear();
        }
        /**
         * Finds an element whose key is x
         */
        iterator find(const key_type& x)
        {
            return t.find(x);
        }
        /**
         * Finds an element whose key is x
         */
        const_iterator find(const key_type& x) const
        {
            return t.find(x);
        }
        /**
         * Returns the number of elements with key x.  For map this will either be 0 or 1.
         */
        size_type count(const key_type& x) const
        {
            return t.count(x);
        }
        /**
         * Finds the first element whose key is not less than x
         */
        iterator lower_bound(const key_type& x)
        {
            return t.lower_bound(x);
        }
        /**
         * Finds the first element whose key is not less than x
         */
        const_iterator lower_bound(const key_type& x) const
        {
            return t.lower_bound(x);
        }
        /**
         * Finds the first element whose key is not greater than x
         */
        iterator upper_bound(const key_type& x)
        {
            return t.upper_bound(x);
        }
        /**
         * Finds the first element whose key is not greater than x
         */
        const_iterator upper_bound(const key_type& x) const
        {
            return t.upper_bound(x);
        }
        typedef Oscl_Pair<iterator, iterator> pair_iterator_iterator;
        /**
         * Finds a range containing all elements whose key is x
         */
        pair_iterator_iterator equal_range(const key_type& x)
        {
            return t.equal_range(x);
        }
        typedef Oscl_Pair<const_iterator, const_iterator> pair_citerator_citerator;
        /**
         * Finds a range containing all elements whose key is x
         */
        pair_citerator_citerator equal_range(const key_type& x) const
        {
            return t.equal_range(x);
        }

    private:

};

//template <class Key, class T, class Compare>
//inline bool operator==(const map<Key, T, Compare>& x,
//                       const map<Key, T, Compare>& y) {
//    return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
//}

//template <class Key, class T, class Compare>
//inline bool operator<(const map<Key, T, Compare>& x,
//                      const map<Key, T, Compare>& y) {
//    return lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
//}


/*! @} */


#endif
