#pragma once

#include <algorithm>  
#include <iostream>  
#include <memory>
#include <stack>
#include <utility>
#include <queue>



template <class T>
class BST {
public:

	BST() = default;
	BST(BST const& source);
	BST(BST && move) noexcept;
	BST& operator=(BST &&move) noexcept;
	~BST();

	BST& operator=(BST const &rhs);
	bool operator==(BST  &tree);

	// Member functions
	int depth();
	bool empty() const { return root == nullptr; }
	void insert(const T& theData);
	void insert(T&& theData);
	void deleteNode(const T& theData);
	bool exists(const T& theData) const;

	template <typename Callable>
	void inorderTraversal(Callable&& visit) const;

	template <typename Callable>
	void preorderTraversal(Callable&& visit) const;

	template <typename Callable>
	void postorderTraversal(Callable&& visit) const;



private:

	struct TreeNode {
		T data;
		std::unique_ptr<TreeNode> left = nullptr;
		std::unique_ptr<TreeNode> right = nullptr;

		TreeNode(const T& x,
			std::unique_ptr<TreeNode>&& left = nullptr,
			std::unique_ptr<TreeNode>&& right = nullptr) :
			data(x),
			left(std::move(left)),
			right(std::move(right)) {}
	};
	std::unique_ptr<TreeNode> root = nullptr;

	// This is used for the copy constructor
	void copyTree(std::unique_ptr<TreeNode>& thisRoot, const TreeNode *sourceRoot){
		if (!sourceRoot) thisRoot = nullptr;
		else {
			thisRoot = std::make_unique<TreeNode>(sourceRoot->data);
			thisRoot->data = sourceRoot->data;
			copyTree(thisRoot->left, sourceRoot->left.get());
			copyTree(thisRoot->right, sourceRoot->right.get());
		}
	}

	std::unique_ptr<TreeNode>& find(const T& theData) {
		auto* node = &root;
		while (*node) {
			if ((*node)->data == theData) return *node;
			if ((*node)->data < theData) node = &((*node)->right);
			else node = &(*node)->left;
		}
		return *node;
	}

	auto findMin(std::unique_ptr<TreeNode>& root) {
		while (root->left) root = std::move(root->left);
		return root.get();
	}

	void delete_helper(std::unique_ptr<TreeNode>& root, const T& theData) {
		if (!root) return;
		else if (theData < root->data) delete_helper(root->left, theData);
		else if (theData > root->data) delete_helper(root->right, theData);
		
		else {
			// Case 1: No Children
			if (!root->left && !root->right) {
				root = nullptr;
			}
		
			// Case 2: One Children
			else if (!root->left) {
				root = std::move(root->right);
			}
			else if (!root->right) {
				root = std::move(root->left);
			}
		
			// Case 3: Two Children
			else {
				auto temp = findMin(root->right);
				root->data = temp->data;
				delete_helper(root->right, temp->data);
			}
		}
	}

	int depth_helper(std::unique_ptr<TreeNode>& root)  {
		if (!root) return 0;
		else {
			int l = depth_helper(root->left);
			int r = depth_helper(root->right);
			return std::max(l, r) + 1;
		}
	}

	bool traversalChecker(TreeNode* aNode, TreeNode* bNode) {
		if (aNode == nullptr && bNode == nullptr) return true;
		if (aNode == nullptr || bNode == nullptr ||
			aNode->data != bNode->data)       return false;
		bool areLftEq = traversalChecker(aNode->left.get(), bNode->left.get());
		bool areRhtEq = traversalChecker(aNode->right.get(), bNode->right.get());

		return (areLftEq && areRhtEq);
	}

};

template <class T>
BST<T>::BST(BST<T> const& source) {
	if (!source.root) root = nullptr;
	else {
		copyTree(this->root, source.root.get());
	}
}

template <class T>
BST<T>::BST(BST<T> && move) noexcept {
	move.swap(*this);
}

template <class T>
BST<T>& BST<T>::operator=(BST<T> && move) noexcept {
	move.swap(*this);
	return *this;
}

template <class T>
BST<T>::~BST() {

}

template <class T>
BST<T>& BST<T>::operator=(BST<T> const &rhs) {
	BST copy(rhs);
	swap(copy);
	return *this;
}

template <class T>
bool BST<T>::operator==(BST<T> &tree) {
	if (&tree == this) return true;
	if (depth() != tree.depth()) return false;
	
	return traversalChecker(root.get(), tree.root.get());
}

template <class T>
int BST<T>::depth()  {
	return depth_helper(root);
}

template <class T>
void BST<T>::insert(const T& theData) {
	auto& pos = find(theData);
	if (!pos) pos = std::make_unique<TreeNode>(theData);
}

template <class T>
void BST<T>::insert(T&& theData) {
	auto& pos = find(theData);
	if (!pos) pos = std::make_unique<TreeNode>(std::move(theData));
}

template <class T>
void BST<T>::deleteNode(const T& theData) {
	delete_helper(root, theData);
}

template <class T>
bool BST<T>::exists(const T& theData) const {
	if (!root) return false;

	auto current = root.get();
	while (current != nullptr) {
		if (current->data == theData) return true;
		else {
			if (theData > current->data) current = current->right.get();
			else current = current->left.get();
		}
	}
	return false;
}

template <class T>
template <typename Callable>
void BST<T>::inorderTraversal(Callable&& visit) const {
	auto node = root.get();
	auto to_be_visited = std::stack<TreeNode*>{};

	while (node || !to_be_visited.empty()) {
		if (node) {
			to_be_visited.push(node);
			node = node->left.get();
		}
		else {
			node = to_be_visited.top();
			to_be_visited.pop();
			visit(node->data);
			node = node->right.get();
		}
	}
}

template <class T>
template <typename Callable>
void BST<T>::preorderTraversal(Callable&& visit) const {
	if (!root) return;

	auto to_be_visited = std::stack<TreeNode*>{};
	to_be_visited.push(root.get());

	while (!to_be_visited.empty()) {
		auto node = to_be_visited.top();
		to_be_visited.pop();

		visit(node->data);
		if (node->right) to_be_visited.push(node->right.get());
		if (node->left) to_be_visited.push(node->left.get());
	}
}

template <class T>
template <typename Callable>
void BST<T>::postorderTraversal(Callable&& visit) const {
	if (!root) return;

	auto to_be_visited = std::stack<TreeNode*>{};
	TreeNode* last_visit = nullptr;
	auto node = root.get();

	while (!to_be_visited.empty() || node) {
		if (node) {
			to_be_visited.push(node);
			node = node->left.get();
		}
		else
		{
			auto peek = to_be_visited.top();
			if (peek->right.get() && last_visit != peek->right.get()) {
				node = peek->right.get();
			}
			else {
				visit(peek->data);
				last_visit = peek;
				to_be_visited.pop();
			}
		}
	}
}