/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_H
#define SJTU_MAP_H

// only for std::less<T>
#include <cstddef>
#include <functional>
#include <iostream>

#include "exceptions.h"
#include "utility.h"


namespace sjtu {

template <class Key, class T, class Compare = std::less<Key> >
class map {
#define RED 0
#define BLACK 1
   public:
    typedef pair<const Key, T> value_type;

   private:
    struct Node {
        value_type data;
        Node *ch[2], *fa;
        bool col;
        int sz;  // sz = -1 => deleted
        Node(value_type data, Node *lc = nullptr, Node *rc = nullptr,
             Node *fa = nullptr, bool col = RED, int sz = 1)
            : data(data), fa(fa), col(col), sz(sz) {
            ch[0] = lc;
            ch[1] = rc;
        }
        bool gdir() const {
            if (fa == nullptr) return 0;
            return fa->ch[1] == this;
        }
    };
    int siz = 0;
    Node *rt = nullptr;
    Node *del_hd = nullptr;
    const Compare cmp;
    void delnode(Node *x) {
        if (x == nullptr) return;
        delnode(x->ch[0]);
        delnode(x->ch[1]);
        delete x;
    }
    int node_sz(Node *x) const {
        if (x == nullptr) return 0;
        return x->sz;
    }
    bool getcol(Node *x) const {
        if (x == nullptr) return BLACK;
        return x->col;
    }
    void rot(Node *p, bool dir) {
        // 0: left->rt
        // 1: right->rt
        Node *f = p->fa;
        Node *c = p->ch[dir];
        // assert(c);
        c->sz = p->sz;
        p->sz = node_sz(p->ch[!dir]) + node_sz(c->ch[!dir]) + 1;
        Node *t = c->ch[!dir];
        if (t) t->fa = p;
        p->ch[dir] = t;
        c->ch[!dir] = p;
        p->fa = c;
        c->fa = f;
        if (f)
            f->ch[p == f->ch[1]] = c;
        else
            rt = c;
        // return c;
    }
    void ins_simp(Node *p, Node *cur, bool dir) {
        if (!p) {
            rt = cur;
            return;
        }
        p->ch[dir] = cur;
        cur->fa = p;
        // if (p->fa) puts ("...");
        for (Node *nw = p; nw; nw = nw->fa) {
            // printf ("%d\n", (nw->data).first);
            nw->sz++;
        }
    }
    void ins_adjust(Node *p, Node *cur, bool dir) {
        cur->col = p ? RED : BLACK;
        ins_simp(p, cur, dir);
        // leaf_insert
        while (getcol(p = cur->fa) == RED) {
            bool p_dir = p->gdir();
            Node *gp = p->fa, *unc = gp->ch[!p_dir];
            // p != rt is guaranteed,otherwise col_p=BLACK,do not need
            // adjustment
            if (getcol(unc) == RED) {  // case1:uncle is RED
                p->col = unc->col = BLACK;
                gp->col = RED;
                cur = gp;
                continue;
            } else {
                if (cur->gdir() != p_dir) {
                    rot(p, !p_dir);
                    std::swap(p, cur);
                }
                // assume gp->p->cur is on a straight chain,otherwise
                // rotate(p),turn to other cases case2:uncle is BLACK
                p->col = BLACK;
                gp->col = RED;
                rot(gp, p_dir);
            }
        }
        rt->col = BLACK;
        // adjustment
    }
    void erase_simp(Node *cur) {
        auto p = cur->fa, son = cur->ch[0] ? cur->ch[0] : cur->ch[1];
        if (son) son->fa = p;
        if (!p) {
            rt = son;
            return;
        }
        p->ch[cur->gdir()] = son;
        for (Node *nw = p; nw; nw = nw->fa) nw->sz--;
    }
    void erase_adjust(Node *cur) {
        // printf ("*\n");
        bool dir = cur->gdir();
        erase_simp(cur);
        // printf ("*");
        Node *p = cur->fa;
        if (!p) {
            if (rt) rt->col = BLACK;
            return;
        } else {
            Node *son = p->ch[dir];
            if (son) {
                son->col = BLACK;
                return;
            }
        }
        // if (cur->col == RED) return;//cur is RED,do not need adjustment
        // delete a BLACK leaf
        while (p && getcol(cur) == BLACK) {
            Node *bro = p->ch[!dir];
            // assert(bro);//???
            // bro must exist,otherwise contradict with the rules of RBT
            if (getcol(bro) ==
                RED) {  // case1:brother is RED,turn to other cases
                bro->col = BLACK;
                p->col = RED;
                rot(p, !dir);
                bro = p->ch[!dir];
            }
            Node *c = bro->ch[dir], *d = bro->ch[!dir];
            if (getcol(c) == BLACK &&
                getcol(d) == BLACK) {  // case 2:bro,c,d=BLACK
                bro->col = RED;
                cur = p;
            } else {                       // case3,4
                if (getcol(d) == BLACK) {  // case 3:c=RED,d=BLACK
                    c->col = BLACK;
                    bro->col = RED;
                    rot(bro, dir);
                    bro = p->ch[!dir];
                    c = bro->ch[dir];
                    d = bro->ch[!dir];
                }
                // case4:bro=BLACK,d=RED
                bro->col = p->col;
                p->col = d->col = BLACK;
                rot(p, !dir);
                cur = rt;
            }
            p = cur->fa;
            if (!p) break;
            dir = cur->gdir();
        }
        cur->col = BLACK;
    }
    Node *gmost_l(Node *x) const {
        if (!x) return nullptr;
        Node *ret = x;
        while (ret->ch[0]) ret = ret->ch[0];
        return ret;
    }
    Node *gmost_r(Node *x) const {
        if (!x) return nullptr;
        Node *ret = x;
        while (ret->ch[1]) ret = ret->ch[1];
        return ret;
    }
    Node *lower_bound(const Key &key) const {
        Node *nw = rt, *ans = nullptr;
        while (nw) {
            if (!cmp((nw->data).first, key)) {
                ans = nw;
                nw = nw->ch[0];
            } else
                nw = nw->ch[1];
        }
        return ans;
    }
    Node *upper_bound(const Key &key) const {
        Node *nw = rt, *ans = nullptr;
        while (nw) {
            if (cmp(key, (nw->data).first)) {
                ans = nw;
                nw = nw->ch[0];
            } else
                nw = nw->ch[1];
        }
        return ans;
    }
    void erase(const Key &key) {
        Node *cur = lower_bound(key);
        if (cur == nullptr || cmp((cur->data).first, key) ||
            cmp(key, (cur->data).first))
            return;
        // data do not exist
        erase_node(cur);
    }
    void erase_node(Node *cur) {
        if (cur == nullptr) return;
        if (cur->ch[0] && cur->ch[1]) {
            Node *tmp = gmost_l(cur->ch[1]);
            Node *cur_fa = cur->fa, *cur_lc = cur->ch[0], *cur_rc = cur->ch[1];
            Node *tmp_fa = tmp->fa, *tmp_lc = tmp->ch[0], *tmp_rc = tmp->ch[1];
            bool cur_dir = cur->gdir(), tmp_dir = tmp->gdir();
            std::swap(cur->sz, tmp->sz);
            std::swap(cur->col, tmp->col);
            // printf ("?%d %d\n", (cur->data).first, (tmp->data).first);
            if (tmp == cur->ch[1]) {
                if (cur_fa) cur_fa->ch[cur_dir] = tmp;
                tmp->fa = cur_fa;
                tmp->ch[1] = cur;
                cur->fa = tmp;
                cur->ch[0] = tmp_lc;
                if (tmp_lc) tmp_lc->fa = cur;
                cur->ch[1] = tmp_rc;
                if (tmp_rc) tmp_rc->fa = cur;
                tmp->ch[0] = cur_lc;
                if (cur_lc) cur_lc->fa = tmp;
            } else {
                // printf ("*") ;
                if (cur_fa) cur_fa->ch[cur_dir] = tmp;
                tmp->fa = cur_fa;
                if (tmp_fa) tmp_fa->ch[tmp_dir] = cur;
                cur->fa = tmp_fa;
                tmp->ch[0] = cur_lc;
                if (cur_lc) cur_lc->fa = tmp;
                tmp->ch[1] = cur_rc;
                if (cur_rc) cur_rc->fa = tmp;
                cur->ch[0] = tmp_lc;
                if (tmp_lc) tmp_lc->fa = cur;
                cur->ch[1] = tmp_rc;
                if (tmp_rc) tmp_rc->fa = cur;
            }
            if (cur == rt) rt = tmp;
            // testprint(rt);
            // std::swap(tmp->data, cur->data);
            // cur = tmp;
        }
        siz--;
        erase_adjust(cur);
        delete cur;
    }
    void copy(Node *&x, Node *y, Node *fa = nullptr) {
        if (y == nullptr) {
            x = nullptr;
            return;
        }
        x = new Node(y->data);
        x->sz = y->sz;
        x->col = y->col;
        x->fa = fa;
        copy(x->ch[0], y->ch[0], x);
        copy(x->ch[1], y->ch[1], x);
    }
    Node *getpr(Node *x) const {
        if (!x) return nullptr;
        if (x->ch[0]) return gmost_r(x->ch[0]);
        Node *ret = x;
        while (ret && ret->fa && ret->gdir() == 0) ret = ret->fa;
        return ret ? ret->fa : nullptr;
    }
    Node *getnx(Node *x) const {
        if (!x) return nullptr;
        if (x->ch[1]) return gmost_l(x->ch[1]);
        Node *ret = x;
        while (ret && ret->fa && ret->gdir() == 1) ret = ret->fa;
        return ret ? ret->fa : nullptr;
    }

   public:
    /**
     * the internal type of data.
     * it should have a default constructor, a copy constructor.
     * You can use sjtu::map as value_type by typedef.
     */
    // typedef pair<const Key, T> value_type;
    /**
     * see BidirectionalIterator at CppReference for help.
     *
     * if there is anything wrong throw invalid_iterator.
     *     like it = map.begin(); --it;
     *       or it = map.end(); ++end();
     */
    class const_iterator;
    class iterator {
        friend class map;

       private:
        map *mp;
        Node *nd;
        /**
         * TODO add data members
         *   just add whatever you want.
         */
       public:
        iterator(map *mp = nullptr, Node *nd = nullptr) : mp(mp), nd(nd) {
            // TODO
        }
        iterator(const const_iterator &other) {
            // TODO
            mp = other.mp;
            nd = other.nd;
        }
        iterator(const iterator &other) {
            // TODO
            mp = other.mp;
            nd = other.nd;
        }
        iterator &operator=(const iterator &other) {
            if (this == &other) return *this;
            mp = other.mp;
            nd = other.nd;
            return *this;
        }
        iterator &operator=(const const_iterator &other) {
            mp = other.mp;
            nd = other.nd;
            return *this;
        }

        /**
         * TODO iter++
         */
        iterator operator++(int) {
            if (*this == mp->end()) throw invalid_iterator();
            iterator ret = *this;
            nd = mp->getnx(nd);
            return ret;
        }

        /**
         * TODO ++iter
         */
        iterator &operator++() {
            if (*this == mp->end()) throw invalid_iterator();
            nd = mp->getnx(nd);
            return *this;
        }

        /**
         * TODO iter--
         */
        iterator operator--(int) {
            if (*this == mp->begin()) throw invalid_iterator();
            iterator ret = *this;
            if (*this == mp->end()) {
                nd = mp->gmost_r(mp->rt);
            } else
                nd = mp->getpr(nd);
            return ret;
        }

        /**
         * TODO --iter
         */
        iterator &operator--() {
            if (*this == mp->begin()) throw invalid_iterator();
            if (*this == mp->end()) {
                nd = mp->gmost_r(mp->rt);
            } else
                nd = mp->getpr(nd);
            return *this;
        }

        /**
         * a operator to check whether two iterators are same (pointing to the
         * same memory).
         */
        value_type &operator*() const {
            if (nd == nullptr) throw invalid_iterator();
            return nd->data;
        }

        bool operator==(const iterator &rhs) const {
            return mp == rhs.mp && nd == rhs.nd;
        }

        bool operator==(const const_iterator &rhs) const {
            return mp == rhs.mp && nd == rhs.nd;
        }

        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return mp != rhs.mp || nd != rhs.nd;
        }

        bool operator!=(const const_iterator &rhs) const {
            return mp != rhs.mp || nd != rhs.nd;
        }

        /**
         * for the support of it->first.
         * See
         * <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/>
         * for help.
         */
        value_type *operator->() const noexcept {
            if (nd == nullptr) throw invalid_iterator();
            return &(nd->data);
        }
    };
    class const_iterator {
        // it should has similar member method as iterator.
        //  and it should be able to construct from an iterator.
        friend class map;

       private:
        // data members.
        const map *mp;
        Node *nd;

       public:
        const_iterator(const map *mp = nullptr, Node *nd = nullptr)
            : mp(mp), nd(nd) {
            // TODO
        }
        const_iterator(const iterator &other) {
            // TODO
            mp = other.mp;
            nd = other.nd;
        }
        const_iterator(const const_iterator &other) {
            // TODO
            mp = other.mp;
            nd = other.nd;
        }
        const_iterator &operator=(const iterator &other) {
            mp = other.mp;
            nd = other.nd;
            return *this;
        }
        const_iterator &operator=(const const_iterator &other) {
            if (this == &other) return *this;
            mp = other.mp;
            nd = other.nd;
            return *this;
        }
        // And other methods in iterator.
        // And other methods in iterator.
        // And other methods in iterator.
        const_iterator operator++(int) {
            // std::cerr << "++";
            if (*this == mp->cend()) {
                // std::cerr << "???";
                throw invalid_iterator();
            }
            const_iterator ret = *this;
            nd = mp->getnx(nd);
            return ret;
        }

        /**
         * TODO ++iter
         */
        const_iterator &operator++() {
            if (*this == mp->cend()) throw invalid_iterator();
            nd = mp->getnx(nd);
            return *this;
        }

        /**
         * TODO iter--
         */
        const_iterator operator--(int) {
            if (*this == mp->cbegin()) throw invalid_iterator();
            const_iterator ret = *this;
            if (*this == mp->cend()) {
                nd = mp->gmost_r(mp->rt);
            } else
                nd = mp->getpr(nd);
            return ret;
        }

        /**
         * TODO --iter
         */
        const_iterator &operator--() {
            if (*this == mp->cbegin()) throw invalid_iterator();
            if (*this == mp->cend()) {
                nd = mp->gmost_r(mp->rt);
            } else
                nd = mp->getpr(nd);
            return *this;
        }

        /**
         * a operator to check whether two iterators are same (pointing to the
         * same memory).
         */
        value_type &operator*() const {
            if (nd == nullptr) throw invalid_iterator();
            return nd->data;
        }

        bool operator==(const iterator &rhs) const {
            return mp == rhs.mp && nd == rhs.nd;
        }

        bool operator==(const const_iterator &rhs) const {
            return mp == rhs.mp && nd == rhs.nd;
        }

        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return mp != rhs.mp || nd != rhs.nd;
        }

        bool operator!=(const const_iterator &rhs) const {
            return mp != rhs.mp || nd != rhs.nd;
        }

        /**
         * for the support of it->first.
         * See
         * <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/>
         * for help.
         */
        value_type *operator->() const noexcept {
            // std::cerr << "->";
            if (nd == nullptr) {
                // std::cerr << "nd=nullptr";
                throw invalid_iterator();
            }
            return &(nd->data);
        }
    };

    /**
     * TODO two constructors
     */
    friend class iterator;
    friend class const_iterator;
    map() : cmp{} {
        rt = nullptr;
        siz = 0;
    }

    map(const map &other) : cmp{} {
        siz = other.siz;
        copy(rt, other.rt);
    }

    /**
     * TODO assignment operator
     */
    map &operator=(const map &other) {
        if (this == &other) return *this;
        // std::cerr << "try to copy";
        clear();
        siz = other.siz;
        copy(rt, other.rt);
        // std::cerr << "copy end";
        return *this;
    }

    /**
     * TODO Destructors
     */
    ~map() {
        delnode(rt);
    }

    /**
     * TODO
     * access specified element with bounds checking
     * Returns a reference to the mapped value of the element with key
     * equivalent to key. If no such element exists, an exception of type
     * `index_out_of_bound'
     */
    T &at(const Key &key) {
        Node *cur = lower_bound(key);
        if (cur == nullptr || cmp((cur->data).first, key) ||
            cmp(key, (cur->data).first)) {
            throw index_out_of_bound();
        }
        return (cur->data).second;
    }

    const T &at(const Key &key) const {
        Node *cur = lower_bound(key);
        if (cur == nullptr || cmp((cur->data).first, key) ||
            cmp(key, (cur->data).first)) {
            // std::cerr << "???";
            throw index_out_of_bound();
        }
        return (cur->data).second;
    }

    /**
     * TODO
     * access specified element
     * Returns a reference to the value that is mapped to a key equivalent to
     * key, performing an insertion if such key does not already exist.
     */
    T &operator[](const Key &key) {
        Node *cur = lower_bound(key);
        if (cur == nullptr || cmp((cur->data).first, key) ||
            cmp(key, (cur->data).first)) {
            cur = insert(value_type(key, T())).first.nd;
            return (cur->data).second;
        }
        return (cur->data).second;
    }

    /**
     * behave like at() throw index_out_of_bound if such key does not exist.
     */
    const T &operator[](const Key &key) const {
        Node *cur = lower_bound(key);
        if (cur == nullptr || cmp((cur->data).first, key) ||
            cmp(key, (cur->data).first)) {
            throw index_out_of_bound();
        }
        return (cur->data).second;
    }

    /**
     * return a iterator to the beginning
     */
    iterator begin() {
        return iterator(this, gmost_l(rt));
    }

    const_iterator cbegin() const {
        return const_iterator(this, gmost_l(rt));
    }

    /**
     * return a iterator to the end
     * in fact, it returns past-the-end.
     */
    iterator end() {
        return iterator(this, nullptr);
    }

    const_iterator cend() const {
        return const_iterator(this, nullptr);
    }

    /**
     * checks whether the container is empty
     * return true if empty, otherwise false.
     */
    bool empty() const {
        return siz == 0;
    }

    /**
     * returns the number of elements.
     */
    size_t size() const {
        return siz;
    }

    /**
     * clears the contents
     */
    void clear() {
        siz = 0;
        Node *tmp = rt;
        rt = nullptr;
        delnode(tmp);
    }

    /**
     * insert an element.
     * return a pair, the first of the pair is
     *   the iterator to the new element (or the element that prevented the
     * insertion), the second one is true if insert successfully, or false.
     */
    pair<iterator, bool> insert(const value_type &data) {
        Node *chk = lower_bound(data.first);
        if (chk && !cmp((chk->data).first, data.first) &&
            !cmp(data.first, (chk->data).first)) {
            return pair(iterator(this, chk), false);
        }
        Node *t = new Node(data);
        Node *nw = rt, *f = nullptr;
        bool dir = 0;
        while (nw) {
            f = nw;
            dir = cmp((nw->data).first, data.first);
            nw = nw->ch[dir];
        }
        siz++;
        ins_adjust(f, t, dir);
        return pair(iterator(this, t), true);
    }

    /**
     * erase the element at pos.
     *
     * throw if pos pointed to a bad element (pos == this->end() || pos points
     * an element out of this)
     */
    void erase(iterator pos) {
        if (pos.mp != this) throw invalid_iterator();
        if (pos == end()) throw invalid_iterator();
        // Node *chk =
        erase_node(pos.nd);
    }

    /**
     * Returns the number of elements with key
     *   that compares equivalent to the specified argument,
     *   which is either 1 or 0
     *     since this container does not allow duplicates.
     * The default method of check the equivalence is !(a < b || b > a)
     */
    size_t count(const Key &key) const {
        Node *ret = lower_bound(key);
        if (ret == nullptr) return 0;
        if (cmp((ret->data).first, key) || cmp(key, (ret->data).first))
            return 0;
        return 1;
    }

    /**
     * Finds an element with key equivalent to key.
     * key value of the element to search for.
     * Iterator to an element with key equivalent to key.
     *   If no such element is found, past-the-end (see end()) iterator is
     * returned.
     */
    iterator find(const Key &key) {
        Node *cur = lower_bound(key);
        if (cur == nullptr || cmp((cur->data).first, key) ||
            cmp(key, (cur->data).first)) {
            return end();
        }
        return iterator(this, cur);
    }

    const_iterator find(const Key &key) const {
        Node *cur = lower_bound(key);
        if (cur == nullptr || cmp((cur->data).first, key) ||
            cmp(key, (cur->data).first)) {
            return cend();
        }
        return const_iterator(this, cur);
    }
    Node *getrt() const {
        return rt;
    }
    //   void testprint(Node *x) const {
    // 		if (x == nullptr) return;
    // 		if (x->ch[0]) printf ("[");
    // 		testprint(x->ch[0]);
    // 		if (x->ch[0]) printf ("]");
    // 		std::cout << (x->data).second;
    // 		printf ("/");
    // 		if (x->ch[1]) printf ("[");
    // 		testprint(x->ch[1]);
    // 		if (x->ch[1]) printf ("]");

    // 	}
#undef RED
#undef BLACK
};

}  // namespace sjtu

#endif