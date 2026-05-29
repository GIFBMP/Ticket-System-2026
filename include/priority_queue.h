#ifndef SJTU_PRIORITY_QUEUE_H
#define SJTU_PRIORITY_QUEUE_H

#include <cmath>       // maybe you need it
#include <cstddef>     // for size_t
#include <functional>  // for std::less
//#include <iostream>

#include "exceptions.h"

namespace sjtu {

template <class T, class Compare = std::less<T>>
class priority_queue {
private:
    struct Node {
        Node *ch, *nx;
        int deg;
        T data;
        Node () {ch = nx = nullptr;}
        Node (T tmp, int deg = 0) : data(tmp) , deg(deg) {
            ch = nx = nullptr;
        }
    };
    Node *root;
    Node *mx_val;
    int sz;
    Compare cmp;
    void copy(Node *&x, Node *y) {
        if (y == nullptr) {
            x = nullptr;
            return;
        }
        x = new Node(y->data, y->deg);
        copy(x->ch, y->ch);
        copy(x->nx, y->nx);
    }
    void del (Node *x) {
        if (x == nullptr) return;
        if (x->ch) del (x->ch);
        if (x->nx) del (x->nx);
        delete x;
    }
    void del_chain (Node *x) {
        if (x == nullptr) return;
        x->ch = nullptr;
        if (x->nx) del_chain (x->nx);
        delete x;
    }
    Node* copy_root_list(Node* x) {
        if (x == nullptr) return nullptr;
        Node* n = new Node(x->data, x->deg);
        n->ch = x->ch; 
        n->nx = copy_root_list(x->nx);
        return n;
    }
    Node *combine (Node *x, Node *y) {
        if (x == nullptr) return y;
        if (y == nullptr) return x;
        Node *rt = nullptr;
        Node **nw = &rt;
        while (x && y) {
            if (x->deg < y->deg) {
                (*nw) = x;
                x = x->nx;
            }
            else {
                (*nw) = y;
                y = y->nx;
            }
            nw = &((*nw)->nx);
        }
        if (x) (*nw) = x;
        else (*nw) = y;
        return rt;
    }
    void lnk (Node *x, Node *y) { // x->y
        x->nx = y->ch;
        y->ch = x;
        (y->deg)++;
    }
    Node *trymerge(Node *x, Node *y) {
        Node *rt = nullptr, *pr_tmp = nullptr, *tmp = nullptr, *nx_tmp = nullptr;
        rt = combine(x, y);
        if (rt == nullptr) return nullptr;
        //testprint(rt);
        //puts ("");
        pr_tmp = nullptr;
        tmp = rt;
        nx_tmp = tmp->nx;
        while (nx_tmp != nullptr) {
            if (tmp->deg != nx_tmp->deg || (nx_tmp->nx != nullptr && nx_tmp->deg == nx_tmp->nx->deg)) {
                pr_tmp = tmp;
                tmp = nx_tmp;
            } 
            else if (cmp(tmp->data, nx_tmp->data)) {
                if (pr_tmp == nullptr)
                    rt = nx_tmp;
                else
                    pr_tmp->nx = nx_tmp;
                lnk(tmp, nx_tmp);
                tmp = nx_tmp;
            } 
            else {
                //printf ("%d %d\n", tmp->data, nx_tmp->data);
                tmp->nx = nx_tmp->nx;
                //if (tmp->nx == nullptr) printf ("+");
                lnk(nx_tmp, tmp);
            }
            nx_tmp = tmp->nx;
        }
        return rt;
    }
    void updmx () {
        if (root == nullptr) return;
        mx_val = root;
        for (Node *tmp = root->nx; tmp != nullptr; tmp = tmp->nx) {
            if (cmp(mx_val->data, tmp->data)) {
                mx_val = tmp;
            }
        }
    }
    Node *rev(Node *x) {
        Node *prev = nullptr;
        Node *curr = x;
        Node *next = nullptr;
        while (curr != nullptr) {
            next = curr->nx;
            curr->nx = prev;
            prev = curr;
            curr = next;
        }
        return prev;
    }
public:
    // void testprint (Node *x) {
    //     if (x == nullptr) return;
    //     printf ("%d:", x->data.x);
    //     if (x->ch) printf ("ch-%d ", x->ch->data.x);
    //     if (x->nx) printf ("nx-%d ", x->nx->data.x);
    //     printf ("\n");
    //     testprint(x->ch);
    //     //printf ("}\n");
    //     testprint(x->nx);
    // }
    priority_queue() {
        root = nullptr;
        sz = 0;
    }
    priority_queue(const priority_queue &x) {
        sz = x.sz; 
        if (x.root) mx_val = x.mx_val;
        copy(root, x.root);
    }
    ~priority_queue() {
        del(root);
    }

    priority_queue& operator = (const priority_queue &x) {
        if (this == &x) return *this;
        sz = x.sz; 
        Node *tmp = root;
        del(tmp);
        if (x.root) mx_val = x.mx_val;
        root = nullptr;
        copy(root, x.root);
        return *this;
    }

    /**
     * @brief Push one element into the queue.
     * @note Its time complexity shall be O(log n).
     */
    void push(const T &dat) {
        priority_queue x;
        x.root = new Node(dat);
        x.sz = 1;
        x.mx_val = x.root;
        Node* old_root = root;
        int old_sz = sz;
        Node* old_mx = mx_val;
        Node* t1 = copy_root_list(root);
        Node* t2 = copy_root_list(x.root);
        Node *c[25];
        int tp = 0;
        for (Node *x = t1; x; x = x->nx) c[++tp] = x;
        for (Node *x = t2; x; x = x->nx) c[++tp] = x;
        try {
            Node* new_root = trymerge(t1, t2);
            root = new_root;
            updmx();
            del_chain(old_root);
            sz += x.sz;
            
            x.clear();
            //x.root = nullptr;
        }
        catch (...) {
            //printf ("???");
            //std::cerr << "???";
            //del_chain(t1);
            //del_chain(t2);
            for (int i = 1; i <= tp; i++) delete c[i];
            root = old_root;
            sz = old_sz;
            mx_val = old_mx;
            throw;
        }
    }

    /**
     * @return A const reference of the top element in the queue.
     * @throws container_is_empty when the top element does not exist.
     * @note Its time complexity shall be O(1).
     */
    const T& top() const {
        if (!sz) throw container_is_empty();
        return mx_val->data;
    }

    /**
     * @brief remove the top element in the queue.
     * @throws container_is_empty when the top element does not exist.
     * @note Its time complexity shall be O(n).
     */
     void pop() {
        if (!sz) throw container_is_empty();
        Node *max_node = root;
        Node *max_prev = nullptr;
        Node *curr = root;
        Node *prev = nullptr;
        Node *tmp = root;
        int old_sz = sz;
        Node *old_mx = mx_val;
        try {
            while (curr != nullptr) {
                if (cmp(max_node->data, curr->data)) {
                    max_node = curr;
                    max_prev = prev;
                }
                prev = curr;
                curr = curr->nx;
            }

            if (max_prev == nullptr)
                root = max_node->nx;
            else
                max_prev->nx = max_node->nx;
            root = trymerge(root, rev(max_node->ch));
            delete max_node;
            sz--;
            updmx();
        }
        catch (...) {
            root = tmp;
            sz = old_sz;
            mx_val = old_mx;
            throw;
        }
        
    }

    /**
     * @return number of elements in the queue.
     */
    size_t size() const {
        return sz;
    }

    /**
     * @return whether there is any element in the queue.
     */
    bool empty() const {
        return sz == 0;
    }

    /**
     * @brief Clear all elements in the queue.
     * @note Its time complexity shall be O(n).
     */
    void clear() {
        Node *tmp = root;
        root = nullptr; sz = 0;
        del(tmp);
    }

    /**
     * @brief Merge element sets of two queues.
     * @note Its time complexity shall be O(log n).
     */
    void merge(priority_queue &x) {
        if (x.empty() || this == &x) return;
        Node* old_root = root;
        Node *old_xroot = x.root;
        int old_sz = sz;
        Node* old_mx = mx_val;
        Node* t1 = copy_root_list(root);
        Node* t2 = copy_root_list(x.root);
        Node *c[25];
        int tp = 0;
        for (Node *x = t1; x; x = x->nx) c[++tp] = x;
        for (Node *x = t2; x; x = x->nx) c[++tp] = x;
        try {
            Node* new_root = trymerge(t1, t2);
            root = new_root;
            del_chain(old_root);
            sz += x.sz;
            updmx();
            x.root = nullptr;
            x.sz = 0;
            x.mx_val = nullptr;
            del_chain(old_xroot);
        }
        catch (...) {
            for (int i = 1; i <= tp; i++) delete c[i];
            root = old_root;
            sz = old_sz;
            mx_val = old_mx;
            throw;
        }
    }
};

}  // namespace sjtu

#endif