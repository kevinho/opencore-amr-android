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

//                     O S C L _ T A G T R E E

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 *
 * @{
 */


/*! \file oscl_tagtree.h
    \brief The file oscl_tagtree.h ...
*/

#ifndef OSCL_TAGTREE_H_INCLUDED
#define OSCL_TAGTREE_H_INCLUDED

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#ifndef OSCL_MAP_H_INCLUDED
#include "oscl_map.h"
#endif

#ifndef OSCL_VECTOR_H_INCLUDED
#include "oscl_vector.h"
#endif

#ifndef OSCL_STDSTRING_H_INCLUDED
#include "oscl_stdstring.h"
#endif

#define OSCL_DISABLE_WARNING_TRUNCATE_DEBUG_MESSAGE
#include "osclconfig_compiler_warnings.h"


struct Oscl_Tag_Base
{
    typedef char tag_base_unit;
    typedef tag_base_unit* tag_base_type;
    typedef uint32 size_type;

    bool operator()(const tag_base_type& x, const tag_base_type& y) const
    {
        return tag_cmp(x, y) < 0;
    }
    size_type tag_len(const tag_base_type& t) const
    {
        return (size_type)oscl_strlen(t);
    }
    tag_base_type tag_copy(tag_base_type& dest, const tag_base_type& src) const
    {
        return oscl_strncpy(dest, src, oscl_strlen(src) + 1);
    }
    int32 tag_cmp(const tag_base_type& x, const tag_base_type& y) const
    {
        return oscl_strncmp(x, y, oscl_strlen(x) + 1);
    }
    OSCL_IMPORT_REF tag_base_type tag_ancestor(tag_base_type& dest, const tag_base_type& src) const;
    OSCL_IMPORT_REF size_type tag_depth(const tag_base_type& t) const;
};

template <class Alloc>
struct Oscl_Tag : public Oscl_Tag_Base
{

    Oscl_Tag(const Oscl_Tag<Alloc>& t)
    {
        tag = tagAllocator.ALLOCATE(tag_len(t.tag) + 1);
        tag_copy(tag, t.tag);
    }

    Oscl_Tag(const tag_base_type& t)
    {
        tag = tagAllocator.ALLOCATE(tag_len(t) + 1);
        tag_copy(tag, t);
    }

    ~Oscl_Tag()
    {
        tagAllocator.deallocate(tag);
    }

    bool operator<(const Oscl_Tag<Alloc>& x) const
    {
        return (tag_cmp(tag, x.tag) < 0);
    }

    Oscl_TAlloc<tag_base_unit, Alloc> tagAllocator;
    tag_base_type tag;
};

/**
 * Oscl_TagTree Class.
 */
template <class T, class Alloc>
class Oscl_TagTree
{

    public:
        typedef Oscl_Tag<Alloc> tag_type;
        typedef typename tag_type::tag_base_type tag_base_type;


        struct Node
        {
            typedef Oscl_Vector<Node*, Alloc> children_type;
            Node() {}

            tag_type tag;
            T value;
            Node* parent;
            children_type children;

            void sort_children()
            {
                bool tryagain;
                if (children.empty()) return;
                do
                {
                    tryagain = 0;
                    for (typename children_type::iterator it = children.begin(); it != (children.end() - 1); it++)
                    {
                        typename children_type::iterator it2 = it + 1;
                        if ((*it2)->tag < (*it)->tag)
                        {
                            // swap em
                            Node* tmp = *it;
                            *it = *it2;
                            *it2 = tmp;
                            tryagain = 1;
                        }
                    }
                }
                while (tryagain);
            }

            typename tag_type::size_type depth()
            {
                return tag.tag_depth(tag.tag);
            }
        };

        typedef Oscl_Vector<Node*, Alloc> children_type;

        typedef Node node_type;
        typedef node_type* node_ptr;
        typedef Oscl_Map<const tag_base_type, node_ptr, Alloc , Oscl_Tag_Base> map_type;
        typedef typename map_type::size_type size_type;
        typedef typename map_type::value_type value_type;

        struct iterator
        {
            typedef node_type& reference;
            typedef node_type* pointer;
            typedef typename map_type::iterator mapiter;
            typedef iterator self;

            iterator() {}
            iterator(mapiter x)
            {
                mapit = x;
            }
            iterator(const iterator& it)
            {
                mapit = it.mapit;
            }

            reference operator*() const
            {
                return *((*mapit).second);
            }
            pointer operator->() const
            {
                return &(operator*());
            }

            bool operator==(const self& x)
            {
                return mapit == x.mapit;
            }

            bool operator!=(const self& x)
            {
                return mapit != x.mapit;
            }

            self& operator++()
            {
                mapit++;
                return *this;
            }

            self operator++(int)
            {
                self tmp = *this;
                ++*this;
                return tmp;
            }

            self& operator--()
            {
                mapit--;
                return *this;
            }

            self operator--(int)
            {
                self tmp = *this;
                --*this;
                return tmp;
            }

            mapiter mapit;
        };

        struct const_iterator
        {
            typedef const node_type& reference;
            typedef const node_type* pointer;
            typedef typename map_type::const_iterator mapiter;
            typedef const_iterator self;

            const_iterator() {}
            const_iterator(mapiter x)
            {
                mapit = x;
            }
            const_iterator(const const_iterator& it)
            {
                mapit = it.mapit;
            }

            reference operator*() const
            {
                return *((*mapit).second);
            }
            pointer operator->() const
            {
                return &(operator*());
            }

            bool operator==(const self& x)
            {
                return mapit == x.mapit;
            }

            bool operator!=(const self& x)
            {
                return mapit != x.mapit;
            }

            self& operator++()
            {
                mapit++;
                return *this;
            }

            self operator++(int)
            {
                self tmp = *this;
                ++*this;
                return tmp;
            }

            self& operator--()
            {
                mapit--;
                return *this;
            }

            self operator--(int)
            {
                self tmp = *this;
                --*this;
                return tmp;
            }

            mapiter mapit;
        };

    public:

        /**
         * Creates a tag tree with only a root node with tag ""
         */
        Oscl_TagTree(size_type max_depth = 0) : maxDepth(max_depth)
        {
            // insert the root node
            node_ptr node = create_node((char*)"", T());
            node->parent = NULL;
            typename map_type::value_type pair(node->tag.tag, node);
            nodeMap.insert(pair);
        }
        /**
         * Copy constructor
         */
        Oscl_TagTree(const Oscl_TagTree<T, Alloc>& x) : maxDepth(x.maxDepth)
        {
            for (const_iterator it = x.begin(); it != x.end(); it++)
            {
                insert(it->tag.tag, it->value);
            }
        }
        /**
         * Assignment operator
         */
        Oscl_TagTree<T, Alloc>& operator=(const Oscl_TagTree<T, Alloc>& x)
        {
            maxDepth = x.maxDepth;
            // clear the current tree
            clear();
            // insert nodes from assigned tree
            for (const_iterator it = x.begin(); it != x.end(); it++)
            {
                insert(it->tag.tag, it->value);
            }
            return *this;
        }
        /**
         * Destructor
         */
        ~Oscl_TagTree()
        {
            // destroy all nodes stored in the map
            for (iterator it = begin(); it != end(); it++)
            {
                destroy_node(&(*it));
            }
        }
        /**
         * Returns an iterator pointing to the first node in the tree.
         */
        iterator begin()
        {
            return iterator(nodeMap.begin());
        }
        /**
         * Returns an iterator pointing to the first node in the tree.
         */
        const_iterator begin() const
        {
            return const_iterator(nodeMap.begin());
        }
        /**
         * Returns an iterator pointing to the end of the tree.
         */
        iterator end()
        {
            return iterator(nodeMap.end());
        }
        /**
         * Returns a const iterator pointing to the end of the tree.
         */
        const_iterator end() const
        {
            return const_iterator(nodeMap.end());
        }
        /**
         * Returns true if tree size is 0
         */
        bool empty() const
        {
            return nodeMap.empty();
        }
        /**
         * Returns the number of nodes stored in the tree
         */
        size_type size() const
        {
            return nodeMap.size();
        }
        /**
         * Returns a reference to the object that is associated with a particular tag.
         * If the map does not already contain such an object, operator[] inserts the default object T().
         */
        T& operator[](const tag_base_type& t)
        {
            return (*((insert(t, T())).first)).value;
        }

        typedef Oscl_Pair<iterator, bool> pair_iterator_bool;
        /**
         * Inserts x into the tree and associates it with tag t.  If the tag already exists
         * x will not be inserted, and an iterator pointing to the existing node with tag t
         * is returned.
         *
         * @param t      tag to use
         * @param x      element to insert
         *
         * @return Returns a pair of parameters, iterator and bool.  The
         *         iterator points to the inserted node containing x.  If
         *         the tag t already existed, then the iterator points to
         *         the node associated with tag t.  The bool is true if x
         *         was inserted and false if it was not inserted due to an
         *         existing node with tag t.
         */
        pair_iterator_bool insert(const tag_base_type& t, const T& x)
        {

            tag_type currenttag(t);
            pair_iterator_bool result(end(), false);
            node_ptr child = NULL;
            size_type ii;
            size_type maxloops;

            // if it exceeds the max depth, then truncate it to the max depth size
            if (maxDepth > 0 && currenttag.tag_depth(currenttag.tag) > maxDepth)
            {
                maxloops = currenttag.tag_depth(currenttag.tag) - maxDepth;
                for (ii = 0; ii < maxloops; ii++)
                {
                    currenttag.tag_ancestor(currenttag.tag, currenttag.tag);
                }
            }

            maxloops = currenttag.tag_depth(currenttag.tag) + 1;
            for (ii = 0; ii < maxloops; ii++)
            {
                // check if tag already exists; if so then no need to continue creating nodes
                typename map_type::iterator mit = nodeMap.find(currenttag.tag);
                if (mit != nodeMap.end())
                {
                    // set child parent relationship
                    if (child)
                    {
                        child->parent = (*mit).second;
                        child->parent->children.push_back(child);
                    }
                    // if this is the first pass node, then set it for the return value
                    if (result.first == end()) result.first = iterator(mit);
                    break;
                }
                // otherwise create a new node, insert it into map, and set parent/child relationship
                else
                {
                    // insert the new node
                    // first pass sets the node to the given value, all others are default value
                    node_ptr node;
                    if (result.first == end())
                    {
                        node = create_node(currenttag.tag, x);
                    }
                    else
                    {
                        node = create_node(currenttag.tag, T());
                    }

                    typename map_type::value_type pair(node->tag.tag, node);
                    typename map_type::pair_iterator_bool mapresult = (nodeMap.insert(pair));

                    // if this is the first pass node to insert, save it for the return value
                    if (result.first == end())
                    {
                        result.first = iterator(mapresult.first);
                        result.second = mapresult.second;
                    }
                    // set child/parent relationship
                    if (child)
                    {
                        child->parent = (*(mapresult.first)).second;
                        child->parent->children.push_back(child);
                    }
                    child = node;
                }

                currenttag.tag_ancestor(currenttag.tag, currenttag.tag);
            }

            return result;
        }
        /**
         * Erases the element pointed to by the iterator.  If the
         * node has children, then the node will not be erased from
         * the tree.  It will be replaced with the default node
         * value.
         *
         * @param position Iterator pointing to the node to be erased
         */
        void erase(iterator position)
        {
            // if node has children, then replace it with default node value
            if (!(position->children.empty()))
            {
                position->value = T();
                return;
            }

            // destroy the node since only the pointer is stored
            destroy_node(&(*position));
            nodeMap.erase(position.mapit);
        }
        /**
         * Erases the node with tag x.  If the node has children,
         * then the node will not be erased from the tree.  It will
         * be replaced with the default node value
         *
         * @param x      Tag of node to erase
         *
         * @return Returns the number of nodes erased.  Since one-to-one
         *         mapping between nodes and tags, this will be either 0 or 1
         */
        size_type erase(const tag_base_type& x)
        {
            iterator it = find(x);
            if (it != end())
            {
                erase(it);
                return 1;
            }
            return 0;
        }
        /**
         * Erases the entire tag tree.
         */
        void clear()
        {
            // destroy all nodes stored in the map
            for (iterator it = begin(); it != end(); it++)
            {
                destroy_node(&(*it));
            }
            // clear the map
            nodeMap.clear();
        }
        /**
         * Finds an element whose key is x
         *
         * @return returns an iterator to the element with key x.  If no element is found, returns end()
         */
        iterator find(const tag_base_type& x)
        {
            return iterator(nodeMap.find(x));
        }
        /**
         * Finds an element whose key is x
         */
//Removed this version due to ADS 1.2 compile problem
//    const_iterator find(const tag_base_type& x) const { return const_iterator(nodeMap.find(x)); }
        /**
         * Returns the number of elements with key x.  This can only be 0 or 1..
         */
        size_type count(const tag_base_type& x) const
        {
            return nodeMap.count(x);
        }

    private:
        node_ptr create_node(const tag_base_type& t, const T& x)
        {
            node_ptr n = nodeAllocator.ALLOCATE(1);
            new(&n->tag) tag_type(t);
            new(&n->value) T(x);
            new(&n->children) Oscl_Vector<Node*, Alloc>();
            return n;
        }

        void destroy_node(node_ptr x)
        {
            x->tag.OSCL_TEMPLATED_DESTRUCTOR_CALL(tag_type, Oscl_Tag);
            x->value.T::~T();
            x->children.OSCL_TEMPLATED_DESTRUCTOR_CALL(children_type, Oscl_Vector);
            nodeAllocator.deallocate(x);
        }

        map_type nodeMap;
        Oscl_TAlloc<node_type, Alloc> nodeAllocator;
        size_type maxDepth;
};

/*! @} */


#endif

