#include <iostream>
#include <assert.h>
#include <algorithm> 
#include <stack>
#include <utility>  

/*

#define _CRTDBG_MAP_ALLOC 
#include <stdlib.h> 
#include <crtdbg.h>

*/

//Узел обычного дерева поиска
template <typename KeyType>
struct CNode {
	CNode* LeftChild;
	CNode* RightChild;
	CNode* Parent;
	KeyType Key;
	CNode() : LeftChild(nullptr), RightChild(nullptr), Parent(0) {}
};
//Дерево поиска
template <typename KeyType>
class CMyTree {
public:
	CMyTree() : root(nullptr) {}
	~CMyTree();
	bool Add(const KeyType& key);
	CNode<KeyType>* get_root();
	void DestroyTree(CNode<KeyType> *root);
	int Height();
private:
	int height(CNode<KeyType>* current);
	CNode<KeyType>* root;
	bool add(CNode<KeyType>& current, const KeyType& key);
};

template <typename KeyType>
CMyTree<KeyType>::~CMyTree() {
	DestroyTree(root);
}

template <typename KeyType>
CNode<KeyType>* CMyTree<KeyType>::get_root() {
	return root;
}

template <typename KeyType>
void CMyTree<KeyType>::DestroyTree(CNode<KeyType> *root) {
	if (root != nullptr) {
		DestroyTree(root->LeftChild);
		DestroyTree(root->RightChild);
		delete root;
	}
}

template <typename KeyType>
bool CMyTree<KeyType>::Add(const KeyType& key) {
	if (root == nullptr) {
		root = new CNode<KeyType>;
		root->Key = key;
		return true;
	}
	return add(*root, key);
}

template <typename KeyType>
bool CMyTree<KeyType>::add(CNode<KeyType>& current, const KeyType& key) {
	if (current.Key == key) {
		return false;
	}
	if (current.Key > key) {
		if (current.LeftChild == nullptr) {
			current.LeftChild = new CNode<KeyType>;
			current.LeftChild->Key = key;
			return true;
		}
		return add(*current.LeftChild, key);
	}
	else {
		if (current.RightChild == nullptr) {
			current.RightChild = new CNode<KeyType>;
			current.RightChild->Key = key;
			return true;
		}
		return add(*current.RightChild, key);
	}
}

template <typename KeyType>
int CMyTree<KeyType>::Height() {
	return height(root);
}

template <typename KeyType>
int CMyTree<KeyType>::height(CNode<KeyType>* current) {
	if (current == nullptr) {
		return 0;
	}
	else {
		return std::max(height(current->LeftChild), height(current->RightChild)) + 1;
	}
}

//Декартово дерево
template<typename KeyType>
class CTreap {
public:
	CTreap() : root(nullptr) {}
	~CTreap();
	bool IsEmpty() const { return root == nullptr; }
	//true, если реально добавили новый узел
	bool Add(const KeyType& key);
	//Добавление вторым способом
	void Insert(const KeyType& key, int priority);
	bool Has(const KeyType& key) const;
	//true, если удалили
	bool Delete(const KeyType& key);
	int Height();
private:
	struct CTreapNode {
		CTreapNode* LeftChild;
		CTreapNode* RightChild;
		KeyType Key;
		int Priority;
		CTreapNode() : LeftChild(nullptr), RightChild(nullptr), Priority(-1) {}
	};
	CTreapNode* root;

	enum TKeepEqualSide {
		KES_Right,
		KES_Left
	};

	static void split(const KeyType& key, CTreapNode* current,
		CTreapNode*& left, CTreapNode*& right, TKeepEqualSide side = KES_Right);
	static CTreapNode* merge(CTreapNode* left, CTreapNode* right);
	static bool has(CTreapNode* current, const KeyType& key);
	//Добавление вторым способом
	bool InsertSecondSolution(CTreapNode* parent, CTreapNode* current, const KeyType& key, int priority, TKeepEqualSide direction);
	//Высота дерева
	int height(CTreapNode* current);
	//Удаление поддерева
	void DestroyTree(CTreapNode* root);
};

template <typename KeyType>
CTreap<KeyType>::~CTreap() {
	DestroyTree(root);
}

template <typename KeyType>
void CTreap<KeyType>::DestroyTree(CTreapNode* node) {
	if (node == nullptr) {
		// Дошли до конца дерева.
		return;
	}
	DestroyTree(node->LeftChild);
	node->LeftChild = nullptr;
	DestroyTree(node->RightChild);
	node->RightChild = nullptr;
	delete node;
}

template<typename KeyType>
void CTreap<KeyType>::split(const KeyType& key, CTreapNode* current,
	CTreapNode*& left, CTreapNode*& right, TKeepEqualSide side)
{
	if (current == nullptr) {
		left = nullptr;
		right = nullptr;
		return;
	}
	if (current->Key == key) {
		if (side == KES_Right) {
			left = current->LeftChild;
			right = current;
			right->LeftChild = nullptr;
		}
		else if (side == KES_Left) {
			right = current->RightChild;
			left = current;
			left->RightChild = nullptr;
		}
		else {
			assert(false);
		}
		return;
	}
	if (key < current->Key) {
		right = current;
		split(key, current->LeftChild, left, current->LeftChild, side);
	}
	else {
		left = current;
		split(key, current->RightChild, current->RightChild, right, side);
	}
}

template<typename KeyType>
typename CTreap<KeyType>::CTreapNode* CTreap<KeyType>::merge(CTreapNode* left, CTreapNode* right)
{
	if (left == nullptr) {
		return right;
	}
	if (right == nullptr) {
		return left;
	}
	if (left->Priority < right->Priority) {
		left->RightChild = merge(left->RightChild, right);
		return left;
	}
	else {
		right->LeftChild = merge(left, right->LeftChild);
		return right;
	}
}

template<typename KeyType>
bool CTreap<KeyType>::Add(const KeyType& key)
{
	if (Has(key)) {
		return false;
	}
	CTreapNode* left = nullptr;
	CTreapNode* right = nullptr;
	split(key, root, left, right, KES_Right);
	CTreapNode* middle = nullptr;
	split(key, right, middle, right, KES_Left);
	bool result = false;
	if (middle == nullptr) {
		middle = new CTreapNode();
		middle->Key = key;
		middle->Priority = rand();
		result = true;
	}
	left = merge(left, middle);
	root = merge(left, right);
	return result;
}

//Эффективная версия вставки
template<typename KeyType>
void CTreap<KeyType>::Insert(const KeyType& key, int priority) {
	if (root == nullptr) {
		root = new CTreapNode();
		root->Key = key;
		root->Priority = priority;
	}
	else {
		if (root->Priority < priority) {
			CTreapNode* left = nullptr;
			CTreapNode* right = nullptr;
			split(key, root, left, right, KES_Right);
			CTreapNode* newElement = new CTreapNode();
			newElement->LeftChild = left;
			newElement->RightChild = right;
			newElement->Key = key;
			newElement->Priority = priority;
			root = newElement;
		}
		else {
			InsertSecondSolution(nullptr, root, key, priority, KES_Right);
		}

	}
}

template<typename KeyType>
bool CTreap<KeyType>::InsertSecondSolution(CTreapNode* parent, CTreapNode* current,
	const KeyType& key, int priority, TKeepEqualSide direction)
{
	
	std::stack <std::pair< std::pair<CTreapNode*, CTreapNode*>, TKeepEqualSide>> CStack;
	CStack.push(std::make_pair(std::make_pair(root, root), KES_Right));
	while (!CStack.empty()) {
		parent = CStack.top().first.first;
		current = CStack.top().first.second;
		direction = CStack.top().second;
		CStack.pop();
		if (current->Priority >= priority) {
			if (current->Key == key) {
				return false;
			}
			if (current->Key > key) {
				if (current->LeftChild == nullptr) {
					current->LeftChild = new CTreapNode();
					current->LeftChild->Key = key;
					current->LeftChild->Priority = priority;
					return true;
				}
				CStack.push(std::make_pair(std::make_pair(current, current->LeftChild), KES_Left));
				//return InsertSecondSolution(current, current->LeftChild, key, priority, KES_Left);
			}
			else {
				if (current->RightChild == nullptr) {
					current->RightChild = new CTreapNode();
					current->RightChild->Key = key;
					current->RightChild->Priority = priority;
					return true;
				}
				CStack.push(std::make_pair(std::make_pair(current, current->RightChild), KES_Right));
				//return InsertSecondSolution(current, current->RightChild, key, priority, KES_Right);
			}
		}
		else {
			break;
		}
		
	}
	CTreapNode* left = nullptr;
	CTreapNode* right = nullptr;
	split(key, current, left, right, KES_Right);
	CTreapNode* newElement = new CTreapNode();
	newElement->LeftChild = left;
	newElement->RightChild = right;
	newElement->Key = key;
	newElement->Priority = priority;
	if (direction == KES_Left) {
		parent->LeftChild = newElement;
	}
	else {
		parent->RightChild = newElement;
	}
}

template<typename KeyType>
bool CTreap<KeyType>::Has(const KeyType& key) const
{
	return has(root, key);
}

template<typename KeyType>
bool CTreap<KeyType>::has(CTreapNode* current, const KeyType& key)
{
	if (current == nullptr) {
		return false;
	}
	if (current->Key == key) {
		return true;
	}
	return has(((current->Key < key) ? current->LeftChild : current->RightChild), key);
}

template<typename KeyType>
bool CTreap<KeyType>::Delete(const KeyType& key)
{
	CTreapNode* left = nullptr;
	CTreapNode* right = nullptr;
	CTreapNode* middle = nullptr;
	split(key, root, left, right, KES_Right);
	split(key, right, middle, right, KES_Left);
	root = merge(left, right);
	delete middle;
	return middle != nullptr;
}

template<typename KeyType>
int CTreap<KeyType>::Height() {
	return height(root);
}

template<typename KeyType>
int CTreap<KeyType>::height(CTreapNode* current) {
	if (current == nullptr) {
		return 0;
	}
	else {
		return std::max(height(current->LeftChild), height(current->RightChild)) + 1;
	}
}

int main()
{
	CTreap<int> treap;
	CMyTree<int> searchTree;

	int n;
	std::cin >> n;
	int x, y;
	for (int i = 0; i < n; ++i) {
		std::cin >> x >> y;
		treap.Insert(x, y);
		searchTree.Add(x);
	}
	//std::cout << searchTree.Height() << std::endl;
	//std::cout << treap.Height() << std::endl;
	std::cout << searchTree.Height() - treap.Height() << std::endl;
	

	/*_CrtDumpMemoryLeaks();
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	*/
	return 0;
}
