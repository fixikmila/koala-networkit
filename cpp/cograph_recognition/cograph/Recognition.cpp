#include <cograph_recognition/CographRecognition.hpp>
#include <list>
#include <set>
#include <bits/stdc++.h>
#include <graph/GraphTools.hpp>
using namespace std;
namespace Koala {
    enum class Type{
        ZEROONE,
        VERTEX
    };
    enum class Marked{
        UNMARKED,
        MARKED,
        MARKED_AND_UNMARKED
    };
    class CoNode{
    private:
        Type type;
        int number;
        Marked marked;
        int md, d;
        //md is the current number of children, which have been both "marked" and "unmarked"
        //d is the current number of children

        CoNode* head_of_list_of_children;
        CoNode *next, *prev;//in list of children of its parent
        CoNode *parent;
        vector<CoNode>outEdges;//neighbours of cur vertex in G
    public:
        CoNode(Type type, int number): type(type), number(number),marked(Marked::UNMARKED),md(0),d(0),head_of_list_of_children(nullptr),
                                       next(nullptr),prev(nullptr), parent(nullptr){

        }
        int getnumber(){
            return number;
        }
        Type gettype(){
            return type;
        }
        void setnumber(int number){
            this -> number = number;
        }
        CoNode *get_head_of_list_of_children(){
            return head_of_list_of_children;
        }
        void set_head_of_list_of_children(CoNode * head_of_list_of_children){
            this -> head_of_list_of_children= head_of_list_of_children;
        }
        CoNode* getnext(){
            return next;
        }
        void setnext(CoNode *next){
            this -> next = next;
        }
        CoNode* getprev(){
            return prev;
        }
        void setprev(CoNode *prev){
            this -> prev = prev;
        }
        void addchild(CoNode *x){
            if(head_of_list_of_children == nullptr){
                head_of_list_of_children = x;
                x -> prev = x -> next = nullptr;
            } else{
                head_of_list_of_children -> prev = x;
                x -> next = head_of_list_of_children;
                x -> prev = nullptr;
                head_of_list_of_children = x;
            }
            x -> parent = this;
            d++;
        }
        void setoutEdges(vector<CoNode>outEdges){
            this->outEdges = outEdges;
        }
        vector<CoNode> getoutEdges(){
            return outEdges;
        }
        CoNode* getParent(){
            return parent;
        }
        CoNode* setParent(CoNode *parent){
            this->parent = parent;
        }
        void set_md(int md){
            this->md = md;
        }
        void inc_md(){
            this->md++;
        }
        int get_md(){
            return md;
        }
        void set_d(int d){
            this->d = d;
        }
        void inc_d(){
            this->d++;
        }
        int get_d(){
            return d;
        }
        Marked Marked_or_not(){
            return marked;
        }
        void unmark_for_new_iteration(){
            marked = Marked::UNMARKED;
            md = 0;
        }
        void mark(){
            marked = Marked::MARKED;}
        void unmark(){
            marked = Marked::MARKED_AND_UNMARKED;
        }
        vector<CoNode*> remove_were_marked(){
            auto u = head_of_list_of_children;
            vector<CoNode*>vec;
            while(u != nullptr){
                vec.push_back(u);
                d--;
                head_of_list_of_children = u -> getnext();
                if(head_of_list_of_children != nullptr)head_of_list_of_children->prev = nullptr;
                u = head_of_list_of_children;
            }
            return vec;
        }
    };
    class CoTree{
    private:
        CoNode* root;
    public:
        CoTree(CoNode* root):root(root){
        }
        CoNode* getRoot(){
            return root;
        }
        void setRoot(CoNode* R){
            root = R;
        }
    };
    CoTree* T;
    NetworKit::Graph G;
    int mark_count = 0;
    int mark_and_unmarked_count = 0;
    int mark_ever_count = 0;
    void unmark(queue<CoNode*> &marked_with_d_equal_to_md){
        CoNode* u = marked_with_d_equal_to_md.front();
        marked_with_d_equal_to_md.pop();
        u -> unmark();
        mark_count--;
        mark_and_unmarked_count++;
        u -> set_md(0);
        if(u != T -> getRoot()){
            auto w = u -> getParent();
            w -> inc_md();
            if(w -> get_md() == w -> get_d()){
                marked_with_d_equal_to_md.push(w);
                auto l = u -> getnext();
                if(u -> getprev() != nullptr){
                    (u -> getprev()) -> setnext(l);
                    u -> setprev(nullptr);
                    u -> setnext(w -> get_head_of_list_of_children());
                    w -> get_head_of_list_of_children() -> setprev (u);
                    w -> set_head_of_list_of_children(u);
                }
                //else u is head
            }
        }
    }
    void Mark( CoNode &x){
        queue<CoNode*> marked_with_d_equal_to_md;
        mark_count = 0;
        mark_and_unmarked_count = 0;
        mark_ever_count = 0;
        for(auto u : x.getoutEdges()){
            u.mark();
            mark_ever_count++;
            mark_count++;
            marked_with_d_equal_to_md.push(&u);
        }
        while(!marked_with_d_equal_to_md.empty()){
            unmark( marked_with_d_equal_to_md);
        }
        if(mark_count){
            if(T -> getRoot() -> get_d() == 1){
                T -> getRoot() -> mark();
            }
        }

    }

    void Reset_All_CoNodes(CoNode *x){
        x -> unmark_for_new_iteration();
        auto y = x -> get_head_of_list_of_children();
        while(y != nullptr){
            Reset_All_CoNodes(y);
            y = y -> getnext();
        }
    }
    CoNode Find_Lowest( ){

    }
    vector<CoNode *> get_were_marked(CoNode *u){
        auto x = u -> get_head_of_list_of_children();
        vector<CoNode *> a;
        while(x != nullptr && x->Marked_or_not() != Marked::UNMARKED){
            a.push_back(x);
            x = x -> getnext();
        }
        return a;
    }
    vector<CoNode *> get_were_not_marked(CoNode *u){
        auto x = u -> get_head_of_list_of_children();
        vector<CoNode *> a;
        while(x != nullptr && x->Marked_or_not() != Marked::UNMARKED){
            x = x -> getnext();
        }
        while(x != nullptr){
            a.push_back(x);
            x = x -> getnext();
        }
        return a;
    }
    void Insert_x_to_CoTree(CoNode *u, CoNode *x){
        vector<CoNode*>a;
        int u_number = u -> getnumber();
        if(u_number == 0){
            a = get_were_marked();
        } else{
            a = get_were_not_marked();
        }
        if(a.size() == 1){
            if(a[0].gettype() == Type::VERTEX){
                CoNode y(Type::ZEROONE, u_number ^ 1);
                u->remove_were_marked();
                u->addchild(&y);
                y.addchild(x);
                y.addchild(a[0]);
            } else{
                a[0] -> addchild(x);
            }
        } else{
            auto vec = u -> remove_were_marked();
            CoNode y(Type::ZEROONE, u_number);
            for(auto v : vec){
                y.addchild(v);
            }
            if(u_number == 0){
                if(u -> getprev() != nullptr)u -> getprev ->setnext(&y);
                if(u -> getnext() != nullptr)u -> getnext() -> setprev(&y);
                y.setParent(u -> parent);
                CoNode z(Type::ZEROONE, 0);
                y.addchild(&z);
                z.addchild(x);
                z.addchild(u);
            } else{
                CoNode z(Type::ZEROONE, 1);
                u ->addchild(&z);
                z.addchild(x);
                z.addchild(&y);
            }
        }
    }
    void Cograph__Recognition(NetworKit::Graph &graph){
        G = graph;
        vector<NetworKit::node>vertex;
        vector<CoNode>covertex;
        map<NetworKit::node, int> pos;
        int cnt = 0;
        for(auto i : G.nodeRange()){
            vertex.push_back(i);
            pos[i] = cnt++;
            CoNode C = CoNode(Type::VERTEX,i);
            covertex.push_back(C);
        }
        for(auto i : G.nodeRange()){
            vector<CoNode> vec;
            for(auto u : G.neighborRange(i)){
                vec.push_back(covertex[pos[u]]);
            }
            covertex[pos[i]].setoutEdges(vec);
        }
        CoNode R(Type::ZEROONE, 1);
        CoTree Tp(&R);
        T = &Tp;
        if(cnt == 0){
            return;
        }
        if(cnt == 1){
            R.addchild(&covertex[0]);
            return;
        }
        if(G.hasEdge(vertex[0], vertex[1])){
            R.addchild(&covertex[0]);
            R.addchild(&covertex[1]);
        } else{
            CoNode N(Type::ZEROONE, 0);
            R.addchild(&N);
            N.addchild(&covertex[0]);
            N.addchild(&covertex[1]);
        }
        for(int i = 2; i < cnt; i++){
            Reset_All_CoNodes(&R);
            Mark(covertex[i]);
            if(R.Marked_or_not() == Marked::MARKED_AND_UNMARKED){//all nodes of T were marked and unmarked <=> R is marked and unmarked
                R.addchild(&covertex[i]);
            } else if(mark_ever_count == 0){
                if(R.get_d() == 1){
                    (R.get_head_of_list_of_children()) -> addchild(&covertex[i]);
                } else{
                    CoNode R1(Type::ZEROONE, 1);
                    CoNode R2(Type::ZEROONE, 0);
                    R1.addchild(&R2);
                    R2.addchild(&R);
                    R2.addchild(&covertex[i]);
                }
            } else{
                CoNode u = Find_Lowest();
                Insert_x_to_CoTree(&u, &covertex[i]);
            }
        }
    }


}