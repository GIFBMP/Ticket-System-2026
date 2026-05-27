#pragma once
#ifndef BPT_H
#define BPT_H
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include "vector.h"

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;
namespace sjtu {
    template<class T, int info_len>
    class MemoryRiver {
        int sizeofT = sizeof(T);
        const static int databegin = info_len * sizeof(int);
        fstream file;
        string file_name;
    public:
        MemoryRiver() = default;
        void initialise(std::string FN = "") {
            if (FN != "")file_name = FN;
            file.open(file_name, std::ios::in | std::ios::out);
            if (file.is_open()) return;
            file.open(file_name, std::ios::out);
            file.close();
            file.open(file_name, std::ios::in | std::ios::out);
            int tmp = 0;
            for (int i = 0; i < info_len; ++i) {
                file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
            }
        }
        void exit() {
            file.close();
        }
        void get_info(int &tmp, int n) {
            if (n > info_len) return;
            file.seekg((n - 1) * sizeof(int));
            file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        }
        void write_info(int tmp, int n) {
            if (n > info_len) return;
            file.seekp((n - 1) * sizeof(int));
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        }
        void write(T &t, int idx) {
            file.seekp((idx - 1) * sizeofT + databegin);
            file.write(reinterpret_cast<char *>(&t), sizeofT);
        }
        int push(T &t) {
            file.seekp(0, file.end);
            int index = ((int)file.tellp() - databegin) / sizeofT + 1;
            file.write(reinterpret_cast<char *>(&t), sizeofT);
            return index;
        }
        int size() {
            file.seekp(0, file.end);
            int index = ((int)file.tellp() - databegin) / sizeofT;
            return index;
        }
        void read(T &t, const int idx) {
            int pos = (idx - 1)* sizeofT + databegin;
            file.seekg(pos, std::ios::beg);
            file.read(reinterpret_cast<char *>(&t), sizeofT);
        }
    };    
    template<class Key, class Val> 
    class bpt {
        struct ValuePair {
            Key a; Val b;
            ValuePair() {
                a = Key(); b = Val();
            }
            ValuePair(Key a, Val b) : a(a), b(b) {}
            bool operator < (const ValuePair &t) const {
                return a == t.a ? b < t.b : a < t.a;
            }
            bool operator == (const ValuePair &t) const {
                return a == t.a && b == t.b;
            }
            bool operator <= (const ValuePair &t) const {
                return a == t.a ? b <= t.b : a <= t.a;
            }
        };
        static const int kMaxSize = 83;
        static const int kMinSize = (kMaxSize + 1) / 2;
        struct Node {
            ValuePair data[kMaxSize + 1];
            int ch[kMaxSize + 1];
            int fa, pr, nx, sz;
            bool isleaf;
            Node() {
                fa = 0; pr = 0; nx = 0; sz = 0;
                isleaf = true;
                memset (ch, 0, sizeof (ch));
                for (int i = 0; i < kMaxSize + 1; i++) data[i] = ValuePair();
            }
        };
        MemoryRiver<Node, 2> tr;
        int rt;
    public:
        bpt(string FN = "") {
            tr.initialise(FN);
            tr.get_info(rt, 1);
            //std::cerr << rt << '\n';
        }
        ~bpt() {
            tr.exit();   
        }
        int lower_bound(const Node &nd, const ValuePair &x) {
            int l = 0, r = nd.sz - 1;
            if (nd.data[r] < x) return r;
            while (l + 1 < r) {
                int mid = (l + r) >> 1;
                if (x <= nd.data[mid]) r = mid;
                else l = mid;
            }
            if (x <= nd.data[l]) return l;
            else return r;
        }
        int lower_bound_key(const Node &nd, const Key &x) {
            int l = 0, r = nd.sz - 1;
            if (nd.data[r].a < x) return r;
            while (l + 1 < r) {
                int mid = (l + r) >> 1;
                if (x <= nd.data[mid].a) r = mid;
                else l = mid;
            }
            if (x <= nd.data[l].a) return l;
            else return r;
        }
        void split(Node &x, int id) {
            Node newnode, fa;
            int cur_sz = x.sz / 2, new_sz = x.sz - cur_sz;
            newnode.sz = new_sz;
            newnode.isleaf = x.isleaf;
            newnode.fa = x.fa;
            newnode.pr = newnode.nx = 0;
            for (int i = 0; i < newnode.sz; i++) {
                newnode.ch[i] = 0;
            }
            /*
            1.modify new node
            2.modify leaf chain
            3.update cur and new_node
            3.modify father
            4.split father if necessary
            */
            for (int i = cur_sz; i < x.sz; i++) {
                newnode.data[i - cur_sz] = x.data[i];
                newnode.ch[i - cur_sz] = x.ch[i];
            }
            x.sz = cur_sz;
            int new_id = 0;
            if (!newnode.isleaf) {//not leaf:modify childs
                new_id = tr.push(newnode);
                for (int i = 0; i < newnode.sz; i++) {
                    Node tmp;
                    tr.read(tmp, newnode.ch[i]);
                    tmp.fa = new_id;
                    tr.write(tmp, newnode.ch[i]);
                }
            }
            else {//leaf:modify brothers
                newnode.pr = id;
                newnode.nx = x.nx;
                new_id = tr.push(newnode);
                if (x.nx) {
                    Node nxt;
                    tr.read(nxt, x.nx);
                    nxt.pr = new_id;
                    tr.write(nxt, x.nx);
                }
                x.nx = new_id;
            }
            if (x.fa) {
                tr.read(fa, x.fa);
                ValuePair tmp = newnode.data[newnode.sz - 1];
                int pos = lower_bound(fa, tmp);
                for (int i = fa.sz; i > pos; i--) {
                    fa.data[i] = fa.data[i - 1];
                    fa.ch[i] = fa.ch[i - 1];
                }
                fa.sz++;
                fa.data[pos] = x.data[x.sz - 1];
                fa.ch[pos] = id;
                fa.ch[pos + 1] = new_id;
                tr.write(fa, x.fa);
                tr.write(x, id);
                if (fa.sz > kMaxSize) {
                    split(fa, x.fa);
                }
            }
            else {//get a new root
                Node rt_block;
                rt_block.sz = 2;
                rt_block.isleaf = false;
                rt_block.data[0] = x.data[cur_sz - 1];
                rt_block.data[1] = newnode.data[new_sz - 1];
                rt_block.ch[0] = id;
                rt_block.ch[1] = new_id;
                rt_block.pr = rt_block.nx = rt_block.fa = 0;
                x.fa = newnode.fa = rt = tr.push(rt_block);
                tr.write(newnode, new_id);
                tr.write(x, id);
                tr.write_info(rt, 1);
            }
            //modify leaf chain

        }
        void insert(const Key &x, const Val &y) {
            ValuePair tmp(x, y);
            int nw = rt; Node block;
            if (!rt) {
                block.isleaf = true;
                block.data[0] = tmp;
                block.sz = 1;
                block.pr = block.nx = block.fa = 0;
                rt = tr.push(block);
                tr.write_info(rt, 1);
                return;
            }
            tr.read(block, rt);
            //nw:cur_node  block:cur_block  pos:child_pos
            while (!block.isleaf) {
                int pos = lower_bound(block, tmp);
                if (block.data[pos] < tmp) {
                    block.data[pos] = tmp;
                    tr.write(block, nw);
                }
                nw = block.ch[pos];
                tr.read(block, nw);
            }
            int pos = lower_bound(block, tmp);
            if (block.data[pos] == tmp) return;//data already exists
            //std::cerr << "insert:" << y << '\n';
            if (block.data[pos] < tmp) pos++;
            for (int i = block.sz; i > pos; i--)
                block.data[i] = block.data[i - 1];
            block.data[pos] = tmp;
            block.sz++;
            tr.write(block, nw);
            if (block.sz > kMaxSize) split(block, nw);
        }
        Val find(const Key &x) {
            if (!rt) return Val();
            int nw = rt;
            Node block;
            tr.read(block, nw);
            while (!block.isleaf) {
                int pos = lower_bound_key(block, x);
                if (block.data[pos].a < x) {
                    return Val();
                }
                nw = block.ch[pos];
                tr.read(block, nw);
            }
            int pos = lower_bound_key(block, x);
            if (!(block.data[pos].a == x)) {
                return Val();
            }
            return block.data[pos].b;
        }
        vector<Val> multifind(const Key &x) {
            vector<Val> ret;
            if (!rt) {
                return ret;
            }
            int nw = rt;
            Node block;
            tr.read(block, nw);
            while (!block.isleaf) {
                int pos = lower_bound_key(block, x);
                if (block.data[pos].a < x) {
                    return ret;
                }
                nw = block.ch[pos];
                tr.read(block, nw);
            }
            int pos = lower_bound_key(block, x);
            //std::cerr << block.data[pos].a.s << '\n';
            //std::cerr << x.s << ' ' << x.len << '\n';
            if (!(block.data[pos].a == x)) {
                return ret;
            }
            //std::cerr << "*";
            while (nw && block.data[block.sz - 1].a == x) {
                //std::cerr << "???";
                for (int i = pos; i < block.sz; i++) {
                    ret.push_back(block.data[i].b);
                    //std::cout << block.data[i].b << ' ';
                }
                pos = 0;
                nw = block.nx;
                //std::cerr << nw << '\n';
                if (!nw) break;
                tr.read(block, nw);
            }
            if (nw) {
                for (int i = pos; i < block.sz && block.data[i].a == x; i++)
                    ret.push_back(block.data[i].b);
            }
            //std::cout << '\n';
        }
        void del_adjust(Node &x, int id) {
            //std::cerr << "adjust:" << id << '\n';
            if (id == rt) {
                if (x.sz == 0) rt = 0;
                else if (x.sz == 1 && x.isleaf == false) {
                    rt = x.ch[0];
                    Node tmp;
                    tr.read(tmp, rt);
                    tmp.fa = 0;
                    tr.write(tmp, rt);
                }
                tr.write_info(rt, 1);
                return;
            }
            bool flag_borrow = false;
            //id != rt is guaranteed
            Node fa, left, right;
            tr.read(fa, x.fa);
            int lpos = lower_bound(fa, x.data[x.sz - 1]) - 1;
            int rpos = lpos + 2, nw_pos = lpos + 1;
            int left_id, right_id;
            //std::cout << lpos << ' ' << rpos << '\n';
            if (lpos >= 0) {
                left_id = fa.ch[lpos];
                tr.read(left, left_id);
                if (left.sz > kMinSize) {
                    flag_borrow = true;
                    ValuePair tmp = left.data[left.sz - 1];
                    left.sz--;
                    ValuePair new_mx = left.data[left.sz - 1];
                    for (int i = x.sz; i; i--) {
                        x.data[i] = x.data[i - 1];
                        x.ch[i] = x.ch[i - 1];
                    }
                    x.sz++;
                    x.data[0] = left.data[left.sz];
                    x.ch[0] = left.ch[left.sz];
                    if (x.isleaf == false) {
                        Node br;
                        tr.read(br, x.ch[0]);
                        br.fa = id;
                        tr.write(br, x.ch[0]);
                    }
                    fa.data[lpos] = new_mx;
                    tr.write(fa, x.fa);
                    tr.write(left, left_id);
                    tr.write(x, id);
                }
            }//try to borrow from left
            if (flag_borrow) return;
            if (rpos < fa.sz) {
                right_id = fa.ch[rpos];
                tr.read(right, right_id);
                if (right.sz > kMinSize) {
                    flag_borrow = true;
                    //puts ("right_borrow");
                    ValuePair tmp = right.data[0];
                    ValuePair new_mx = tmp;
                    int fir_id = right.ch[0];
                    for (int i = 0; i < right.sz - 1; i++) {
                        right.data[i] = right.data[i + 1];
                        right.ch[i] = right.ch[i + 1];
                    }
                    right.sz--; x.sz++;
                    x.data[x.sz - 1] = tmp;
                    x.ch[x.sz - 1] = fir_id;
                    if (right.isleaf == false) {
                        Node br;
                        tr.read(br, fir_id);
                        br.fa = id;
                        tr.write(br, fir_id);
                    }
                    //nw_pos = rpos - 1
                    fa.data[nw_pos] = new_mx;
                    tr.write(fa, x.fa);
                    tr.write(right, right_id);
                    tr.write(x, id);
                }

            }//try to borrow from right
            if (flag_borrow) return;
            //borrow failed, try to merge and go upwards
            if (lpos >= 0) {
                left_id = fa.ch[lpos];
                int cur_sz = x.sz, left_sz = left.sz;
                for (int i = left_sz; i < left_sz + cur_sz; i++) {
                    left.ch[i] = x.ch[i - left_sz];
                    left.data[i] = x.data[i - left_sz];
                }
                left.sz += cur_sz;
                if (left.isleaf == false) {
                    for (int i = left_sz; i < left_sz + cur_sz; i++) {
                        Node tmp;
                        tr.read(tmp, left.ch[i]);
                        tmp.fa = left_id;
                        tr.write(tmp, left.ch[i]);
                    }
                }
                ValuePair new_mx = left.data[left.sz - 1];
                for (int i = nw_pos; i < fa.sz - 1; i++) {
                    fa.ch[i] = fa.ch[i + 1];
                    fa.data[i] = fa.data[i + 1];
                }
                fa.sz--;
                fa.data[lpos] = new_mx;
                left.nx = x.nx;
                if (x.isleaf && x.nx) {
                    Node tmp;
                    tr.read(tmp, x.nx);
                    tmp.pr = left_id;
                    tr.write(tmp, x.nx);
                }
                tr.write(fa, left.fa);
                tr.write(left, left_id);
                if (fa.sz < kMinSize) del_adjust(fa, left.fa);
                return;
            }//try to merge with left
            else if (rpos < fa.sz) {
                //puts ("right_merge");
                right_id = fa.ch[rpos];
                int cur_sz = x.sz, right_sz = right.sz;
                for (int i = cur_sz; i < cur_sz + right_sz; i++) {
                    x.ch[i] = right.ch[i - cur_sz];
                    x.data[i] = right.data[i - cur_sz];
                }
                x.sz += right_sz;
                if (x.isleaf == false) {
                    for (int i = cur_sz; i < cur_sz + right_sz; i++) {
                        Node tmp;
                        tr.read(tmp, x.ch[i]);
                        tmp.fa = id;
                        tr.write(tmp, x.ch[i]);
                    }
                }
                ValuePair new_mx = x.data[x.sz - 1];
                for (int i = rpos; i < fa.sz - 1; i++) {
                    fa.ch[i] = fa.ch[i + 1];
                    fa.data[i] = fa.data[i + 1];
                }
                fa.sz--;
                fa.data[nw_pos] = new_mx;
                x.nx = right.nx;
                if (right.isleaf && right.nx) {
                    Node tmp;
                    tr.read(tmp, right.nx);
                    tmp.pr = id;
                    tr.write(tmp, right.nx);
                }
                tr.write(fa, x.fa);
                tr.write(x, id);
                if (fa.sz < kMinSize) del_adjust(fa, x.fa);
                return;
            }//try to merge with right
        }
        void del(const Key &x, const Val &y) {
            ValuePair tmp(x, y);
            if (!rt) return;
            int nw = rt;
            Node block;
            tr.read(block, nw);
            while (!block.isleaf) {
                int pos = lower_bound(block, tmp);
                if (block.data[pos] < tmp) return;
                nw = block.ch[pos];
                tr.read(block, nw);
            }
            int pos = lower_bound(block, tmp);
            if (!(block.data[pos].a == x && block.data[pos].b == y)) return;
            for (int i = pos; i < block.sz - 1; i++)
                block.data[i] = block.data[i + 1];
            block.sz--;
            tr.write(block, nw);
            ValuePair new_mx = block.data[block.sz - 1];
            if (new_mx < tmp) {//maximum is changed
                Node tmp_fa;
                int id = block.fa;
                while (id) {
                    tr.read(tmp_fa, id);
                    pos = lower_bound(tmp_fa, tmp);
                    if (tmp_fa.data[pos] == tmp) {
                        tmp_fa.data[pos] = new_mx;
                        tr.write(tmp_fa, id);
                    }
                    else break;
                    id = tmp_fa.fa;
                }
            }
            if (block.sz < kMinSize)
                del_adjust(block, nw);
        }
        int getrt() {return rt;}
        bool empty() {return tr.size() == 0;}
    };
}
#endif