#include "Tree.h"

using namespace SJH;
Tree::Tree()
{
}
Tree::~Tree()
{
}
bool Tree::insert(T t, Node *&node){
	if (node == NULL){
		node = new Node(t);
		return true;
	}
	if (t > node->data){
		return insert(t, node->R);
	}
	else
		return insert(t, node->L);
}
void Tree::sortvalues(Node *&node, std::vector<T> &arr){
	if (node == NULL)
		return;
	if (node->L)
		return sortvalues(node->L, arr);
	arr.push_back(node->data);
	if (node->R)
		return sortvalues(node->R, arr);
}
void Tree::sortvalues(std::vector<T> &arr, bool Max){
	sortvalues(Root, arr);
	if (!Max){
		T temp;
		for (int i = 0; i < len/2; i++)
		{
			temp = arr[i];
			arr[i] = arr[len-i];
			arr[len - i] = temp;
		}
	}
}
void Tree::remove(T t){
	if (Root){
		Node *Par = Root;
		Node *node = Root->Find(t,Par);
		if (node){
			if (node == Par){//根节点
				if (node->L == node->R){
					delete node;
					Root = NULL;
				}
				else if (node->L == NULL)
				{
					Par = node->R;
					node->R = NULL;
					delete node;
					Root = Par;
				}
				else if (node->R == NULL){
					Par = node->L;
					node->L = NULL;
					delete node;
					Root = Par;
				}
				else{
					Par = node->L;
					//递归右的左
					Node *RL = node->R;
					while (RL->L){
						RL = RL->L;
					}
					RL->L = Par;
					Par = node->R;
					node->L = node->R = NULL;
					delete node;
					Root = Par;
				}
			}
		}
	}
}