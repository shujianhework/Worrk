#pragma once
#include <vector>
namespace SJH{
#define T int
	class Tree
	{
	private:
		struct Node{
			T data;
			Node *R;
			Node *L;
			Node(T data) :data(data),R(NULL),L(NULL){}
			Node *Find(T d,Node *&Par){
				if (data == d)
					return this;
				Par = this;
				if (d > data){
					return R == NULL ? NULL : R->Find(d,Par);
				}else
					return L == NULL ? NULL : L->Find(d,Par);
			}
			~Node(){
				if (L)
					delete L;
				if (R)
					delete R;
				L = R = NULL;
			}
		};
	private:
		Node* Root;
	private:
		void sortvalues(Node *&node, std::vector<T> &arr);
		bool insert(T t, Node *&node);
	public:
		int len;
	public:
		Tree();
		~Tree();
		inline bool insert(T t){return insert(t, Root);}
		inline void sortvalues(std::vector<T> &arr, bool Max = false);
		inline void remove(T t);
	};
}
