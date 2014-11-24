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

//                     O S C L _ T R E E

// = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = =

/*! \addtogroup osclbase OSCL Base
 *
 * @{
 */


/*! \file oscl_tree.h
    \brief The file oscl_tree.h defines the template class Oscl_Rb_Tree which has a very similar API as the STL Tree class.  It is an implementation of a Red-Black Tree for use by the Oscl_Map class.  Memory allocation is abstracted through the use of an allocator template parameter.
*/

#ifndef OSCL_TREE_H_INCLUDED
#define OSCL_TREE_H_INCLUDED

#ifndef OSCL_DEFALLOC_H_INCLUDED
#include "oscl_defalloc.h"
#endif

#ifndef OSCL_BASE_H_INCLUDED
#include "oscl_base.h"
#endif

#define OSCL_DISABLE_WARNING_TRUNCATE_DEBUG_MESSAGE
#include "osclconfig_compiler_warnings.h"

template <class T1, class T2>
struct Oscl_Pair
{
    T1 first;
    T2 second;
    Oscl_Pair() : first(T1()), second(T2()) {}
    Oscl_Pair(const T1& a, const T2& b) : first(a), second(b) {}
};


struct Oscl_Rb_Tree_Node_Base
{
    typedef Oscl_Rb_Tree_Node_Base* base_link_type;
    typedef enum RedBl {red, black} color_type;

    color_type color;
    base_link_type parent;
    base_link_type left;
    base_link_type right;

    static base_link_type minimum(base_link_type x)
    {
        if (x)
        {
            while (x->left != 0) x = x->left;
        }
        return x;
    }
    static base_link_type maximum(base_link_type x)
    {
        if (x)
        {
            while (x->right != 0) x = x->right;
        }
        return x;
    }
};

template <class Value>
struct Oscl_Rb_Tree_Node : public Oscl_Rb_Tree_Node_Base
{
    typedef Value value_type;
    typedef Oscl_Rb_Tree_Node<Value>* link_type;
    value_type value;
};


template <class Value>
struct Oscl_Rb_Tree_Iterator
{
    typedef Value value_type;
    typedef value_type& reference;
    typedef value_type* pointer;
    typedef Oscl_Rb_Tree_Iterator<Value> iterator;
    typedef Oscl_Rb_Tree_Iterator<Value> self;
    typedef Oscl_Rb_Tree_Node_Base* base_link_type;
    typedef Oscl_Rb_Tree_Node<Value>* link_type;

    base_link_type node;

    Oscl_Rb_Tree_Iterator() {}
    Oscl_Rb_Tree_Iterator(link_type x)
    {
        node = x;
    }
    Oscl_Rb_Tree_Iterator(const iterator& it)
    {
        node = it.node;
    }

    reference operator*() const
    {
        return link_type(node)->value;
    }
    pointer operator->() const
    {
        return &(operator*());
    }

    bool operator==(const self& x)
    {
        return node == x.node;
    }

    bool operator!=(const self& x)
    {
        return node != x.node;
    }

    self& operator++()
    {
        if (node->right != 0)
        {
            node = node->right;
            while (node->left != 0)
            {
                node = node->left;
            }
        }
        else
        {
            base_link_type y = node->parent;
            while (node == y->right)
            {
                node = y;
                y = y->parent;
            }
            if (node->right != y) node = y;
        }
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
        if (node->color == Oscl_Rb_Tree_Node_Base::red && (node->parent)->parent == node)
        {
            node = node->right;   // return rightmost
        }
        else if (node->left != 0)
        {
            base_link_type y = node->left;
            while (y->right != 0)
                y = y->right;
            node = y;
        }
        else
        {
            base_link_type y = node->parent;
            while (node == y->left)
            {
                node = y;
                y = y->parent;
            }
            node = y;
        }
        return *this;
    }

    self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }
};


template <class Value>
struct Oscl_Rb_Tree_Const_Iterator
{
    typedef Value value_type;
    typedef const value_type& reference;
    typedef const value_type* pointer;
    typedef Oscl_Rb_Tree_Const_Iterator<Value> const_iterator;
    typedef Oscl_Rb_Tree_Const_Iterator<Value> self;
    typedef Oscl_Rb_Tree_Node_Base* base_link_type;
    typedef Oscl_Rb_Tree_Node<Value>* link_type;

    base_link_type node;

    Oscl_Rb_Tree_Const_Iterator() {}
    Oscl_Rb_Tree_Const_Iterator(link_type x)
    {
        node = x;
    }
    Oscl_Rb_Tree_Const_Iterator(const const_iterator& it)
    {
        node = it.node;
    }

    reference operator*() const
    {
        return link_type(node)->value;
    }
    pointer operator->() const
    {
        return &(operator*());
    }

    bool operator==(const self& x)
    {
        return node == x.node;
    }

    bool operator!=(const self& x)
    {
        return node != x.node;
    }

    self& operator++()
    {
        if (node->right != 0)
        {
            node = node->right;
            while (node->left != 0)
            {
                node = node->left;
            }
        }
        else
        {
            base_link_type y = node->parent;
            while (node == y->right)
            {
                node = y;
                y = y->parent;
            }
            if (node->right != y) node = y;
        }
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
        if (node->color == Oscl_Rb_Tree_Node_Base::red && (node->parent)->parent == node)
        {
            node = node->right;   // return rightmost
        }
        else if (node->left != 0)
        {
            base_link_type y = node->left;
            while (y->right != 0)
                y = y->right;
            node = y;
        }
        else
        {
            base_link_type y = node->parent;
            while (node == y->left)
            {
                node = y;
                y = y->parent;
            }
            node = y;
        }
        return *this;
    }

    self operator--(int)
    {
        self tmp = *this;
        --*this;
        return tmp;
    }
};


class Oscl_Rb_Tree_Base
{

    public:
        typedef Oscl_Rb_Tree_Node_Base::base_link_type base_link_type;

        OSCL_IMPORT_REF void rotate_left(base_link_type x, base_link_type& root);
        OSCL_IMPORT_REF void rotate_right(base_link_type x, base_link_type& root);
        OSCL_IMPORT_REF void rebalance(base_link_type x, base_link_type& root);
        OSCL_IMPORT_REF base_link_type rebalance_for_erase(base_link_type z,
                base_link_type& root,
                base_link_type& leftmost,
                base_link_type& rightmost);
};


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
class Oscl_Rb_Tree : public Oscl_Rb_Tree_Base
{

    public:
        typedef Key key_type;
        typedef Value value_type;
        typedef value_type* pointer;
        typedef const value_type* const_pointer;
        typedef value_type& reference;
        typedef const value_type& const_reference;
        typedef typename Oscl_Rb_Tree_Node<Value>::link_type link_type;
        typedef Oscl_Rb_Tree_Iterator<value_type> iterator;
        typedef Oscl_Rb_Tree_Const_Iterator<value_type> const_iterator;
        typedef uint32 size_type;
        typedef int32 difference_type;
    private:
        typedef typename Oscl_Rb_Tree_Node<Value>::color_type color_type;
        typedef Oscl_Rb_Tree_Node<Value> node_type;

    private:
        link_type header;
        size_type node_count;
        Compare key_compare;
        Oscl_TAlloc<node_type, Alloc> node_allocator;

    public:
        Oscl_Rb_Tree(const Compare& comp = Compare())
                : node_count(0), key_compare(comp)
        {
            header = get_node();
            header->color = Oscl_Rb_Tree_Node_Base::red;
            leftmost() = header;
            rightmost() = header;
            root() = 0;
        }

        Oscl_Rb_Tree(const Oscl_Rb_Tree<Key, Value, KeyOfValue, Compare, Alloc>& x)
                : node_count(0), key_compare(x.key_compare)
        {
            header = get_node();
            header->color = Oscl_Rb_Tree_Node_Base::red;
            if (x.root() == 0)
            {
                leftmost() = header;
                rightmost() = header;
                root() = 0;
            }
            else
            {
                root() = copy(x.root(), header);
                leftmost() = minimum(root());
                rightmost() = maximum(root());
            }
            node_count = x.node_count;
        }

        ~Oscl_Rb_Tree()
        {
            clear();
            release_node(header);
        }

        Oscl_Rb_Tree<Key, Value, KeyOfValue, Compare, Alloc>&
        operator=(const Oscl_Rb_Tree<Key, Value, KeyOfValue, Compare, Alloc>& x)
        {
            if (this != &x)
            {
                clear();
                node_count = 0;
                key_compare = x.key_compare;

                if (x.root() == 0)
                {
                    root() = 0;
                    leftmost() = header;
                    rightmost() = header;
                }
                else
                {
                    root() = copy(x.root(), header);
                    leftmost() = minimum(root());
                    rightmost() = maximum(root());
                    node_count = x.node_count;
                }
            }
            return *this;
        }

    public:
        iterator begin()
        {
            return leftmost();
        }
        const_iterator begin() const
        {
            return leftmost();
        }
        iterator end()
        {
            return header;
        }
        const_iterator end() const
        {
            return header;
        }
        bool empty() const
        {
            return node_count == 0;
        }
        size_type size() const
        {
            return node_count;
        }
        size_type max_size() const
        {
            return size_type(-1);
        }

        Oscl_Pair<iterator, bool> insert_unique(const value_type& v)
        {
            link_type y = header;
            link_type x = root();
            bool comp = true;
            while (x != 0)
            {
                y = x;
                comp = key_compare(KeyOfValue()(v), key(x));
                x = comp ? left(x) : right(x);
            }
            iterator j = iterator(y);
            if (comp)
            {
                if (j == begin())
                    return Oscl_Pair<iterator, bool>(insert(x, y, v), true);
                else
                    --j;
            }
            if (key_compare(key(j.node), KeyOfValue()(v)))
                return Oscl_Pair<iterator, bool>(insert(x, y, v), true);

            return Oscl_Pair<iterator, bool>(j, false);
        }

        iterator insert_unique(iterator position, const value_type& v)
        {
            if (position.node == header->left)   // begin()
            {
                if (size() > 0 && key_compare(KeyOfValue()(v), key(position.node)))
                    return insert((link_type)position.node, (link_type)position.node, v);
                // first argument just needs to be non-null
                else
                    return insert_unique(v).first;
            }
            else if (position.node == header)   // end()
            {
                if (key_compare(key(rightmost()), KeyOfValue()(v)))
                    return insert(0, rightmost(), v);
                else
                    return insert_unique(v).first;
            }
            else
            {
                iterator before = position;
                --before;
                if (key_compare(key(before.node), KeyOfValue()(v))
                        && key_compare(KeyOfValue()(v), key(position.node)))
                {
                    if (right(before.node) == 0)
                        return insert(0, (link_type)before.node, v);
                    else
                        return insert((link_type)position.node, (link_type)position.node, v);
                    // first argument just needs to be non-null
                }
                else
                    return insert_unique(v).first;
            }
        }

        void insert_unique(const_iterator first, const_iterator last)
        {
            for (; first != last; ++first)
                insert_unique(*first);
        }

        void insert_unique(const value_type* first, const value_type* last)
        {
            for (; first != last; ++first)
                insert_unique(*first);
        }

        void erase(iterator position)
        {
            link_type y = (link_type) rebalance_for_erase(position.node,
                          header->parent,
                          header->left,
                          header->right);

            destroy_node(y);
            --node_count;
        }

        size_type erase(const key_type& x)
        {
            Oscl_Pair<iterator, iterator> p = equal_range(x);
            size_type n = 0;
            distance(p.first, p.second, n);
            erase(p.first, p.second);
            return n;
        }

        void erase(iterator first, iterator last)
        {
            if (first == begin() && last == end())
                clear();
            else
                while (first != last) erase(first++);
        }

        void erase(const key_type* first, const key_type* last)
        {
            while (first != last) erase(*first++);
        }

        void clear()
        {
            if (node_count != 0)
            {
                erase_without_rebalance(root());
                leftmost() = header;
                root() = 0;
                rightmost() = header;
                node_count = 0;
            }
        }

        iterator find(const Key& k)
        {
            link_type y = header;        // Last node which is not less than k.
            link_type x = root();        // Current node.

            while (x != 0)
            {
                if (!key_compare(key(x), k))
                    y = x, x = left(x);
                else
                    x = right(x);
            }
            iterator j = iterator(y);
            return (j == end() || key_compare(k, key(j.node))) ? end() : j;
        }

        const_iterator find(const Key& k) const
        {
            link_type y = header; /* Last node which is not less than k. */
            link_type x = root(); /* Current node. */

            while (x != 0)
            {
                if (!key_compare(key(x), k))
                    y = x, x = left(x);
                else
                    x = right(x);
            }
            const_iterator j = const_iterator(y);
            return (j == end() || key_compare(k, key(j.node))) ? end() : j;
        }

        size_type count(const Key& k) const
        {
            Oscl_Pair<const_iterator, const_iterator> p = equal_range(k);
            size_type n = 0;
            distance(p.first, p.second, n);
            return n;
        }

        iterator lower_bound(const Key& k)
        {
            link_type y = header; /* Last node which is not less than k. */
            link_type x = root(); /* Current node. */

            while (x != 0)
            {
                if (!key_compare(key(x), k))
                {
                    y = x;
                    x = left(x);
                }
                else
                    x = right(x);
            }
            return iterator(y);
        }

        const_iterator lower_bound(const Key& k) const
        {
            link_type y = header; /* Last node which is not less than k. */
            link_type x = root(); /* Current node. */

            while (x != 0)
            {
                if (!key_compare(key(x), k))
                {
                    y = x;
                    x = left(x);
                }
                else
                    x = right(x);
            }
            return const_iterator(y);
        }

        iterator upper_bound(const Key& k)
        {
            link_type y = header; /* Last node which is greater than k. */
            link_type x = root(); /* Current node. */

            while (x != 0)
            {
                if (key_compare(k, key(x)))
                {
                    y = x;
                    x = left(x);
                }
                else
                    x = right(x);
            }
            return iterator(y);
        }

        const_iterator upper_bound(const Key& k) const
        {
            link_type y = header; /* Last node which is greater than k. */
            link_type x = root(); /* Current node. */

            while (x != 0)
            {
                if (key_compare(k, key(x)))
                {
                    y = x;
                    x = left(x);
                }
                else
                    x = right(x);
            }
            return const_iterator(y);
        }

        Oscl_Pair<iterator, iterator> equal_range(const Key& k)
        {
            return Oscl_Pair<iterator, iterator>(lower_bound(k), upper_bound(k));
        }

        Oscl_Pair<const_iterator, const_iterator> equal_range(const Key& k) const
        {
            return Oscl_Pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
        }

    private:
        // this helper function performs a downcast from base_link_type& to link_type&
        // under C99 (gcc 3.x) this breaks aliasing rules so we have to go via a void** instead
        inline static link_type& cast_to_link_type(base_link_type &node)
        {
            void** ptr = (void**) & node;
            link_type* base = (link_type*) ptr;
            return *base;
        }

        link_type& root() const
        {
            return cast_to_link_type(header->parent);
        }
        link_type& leftmost() const
        {
            return cast_to_link_type(header->left);
        }
        link_type& rightmost() const
        {
            return cast_to_link_type(header->right);
        }

        const Key& key(link_type x) const
        {
            return KeyOfValue()(value(x));
        }
        static reference value(link_type x)
        {
            return x->value;
        }
        static link_type& left(link_type x)
        {
            return cast_to_link_type(x->left);
        }
        static link_type& right(link_type x)
        {
            return cast_to_link_type(x->right);
        }
        static link_type& parent(link_type x)
        {
            return cast_to_link_type(x->parent);
        }

        const Key& key(base_link_type x) const
        {
            return KeyOfValue()(value(x));
        }
        static reference value(base_link_type x)
        {
            return ((link_type)x)->value;
        }
        static link_type& left(base_link_type x)
        {
            return cast_to_link_type(x->left);
        }
        static link_type& right(base_link_type x)
        {
            return cast_to_link_type(x->right);
        }
        static link_type& parent(base_link_type x)
        {
            return cast_to_link_type(x->parent);
        }

        static link_type minimum(link_type x)
        {
            return (link_type) Oscl_Rb_Tree_Node_Base::minimum(x);
        }
        static link_type maximum(link_type x)
        {
            return (link_type) Oscl_Rb_Tree_Node_Base::maximum(x);
        }

        iterator insert(link_type x, link_type y, const value_type& v)
        {
            link_type z;

            if (y == header || x != 0 || key_compare(KeyOfValue()(v), key(y)))
            {
                z = create_node(v);
                left(y) = z;                // also makes leftmost() = z when y == header
                if (y == header)
                {
                    root() = z;
                    rightmost() = z;
                }
                else if (y == leftmost())
                    leftmost() = z;           // maintain leftmost() pointing to min node
            }
            else
            {
                z = create_node(v);
                right(y) = z;
                if (y == rightmost())
                    rightmost() = z;          // maintain rightmost() pointing to max node
            }
            parent(z) = y;
            left(z) = 0;
            right(z) = 0;
            rebalance(z, header->parent);
            ++node_count;
            return iterator(z);

        }

        void erase_without_rebalance(link_type x)
        {
            while (x != 0)
            {
                erase_without_rebalance((link_type)x->right);
                link_type y = (link_type)x->left;
                destroy_node(x);
                x = y;
            }
        }

        link_type copy(link_type x, link_type p)
        {
            // structural copy.  x and p must be non-null.
            link_type top = clone_node(x);
            top->parent = p;

            if (x->right)
            {
                top->right = copy(right(x), top);
            }
            p = top;
            x = left(x);

            while (x != 0)
            {
                link_type y = clone_node(x);
                p->left = y;
                y->parent = p;
                if (x->right)
                {
                    y->right = copy(right(x), y);
                }
                p = y;
                x = left(x);
            }

            return top;
        }

        link_type get_node()
        {
            return node_allocator.ALLOCATE(1);
        }
        void release_node(link_type node)
        {
            node_allocator.deallocate(node);
        }

        link_type create_node(const value_type& v)
        {
            link_type x = get_node();
            new(&x->value) value_type(v);
            return x;
        }

        void destroy_node(link_type x)
        {
            x->value.~Value();
            release_node(x);
        }

        link_type clone_node(link_type x)
        {
            link_type tmp = create_node(x->value);
            tmp->color = x->color;
            tmp->left = 0;
            tmp->right = 0;
            return tmp;
        }

        void distance(const_iterator first, const_iterator last, size_type& n) const
        {
            while (first != last)
            {
                n++;
                first++;
            }
        }

        void distance(iterator first, iterator last, size_type& n) const
        {
            while (first != last)
            {
                n++;
                first++;
            }
        }
};


/*! @} */


#endif

