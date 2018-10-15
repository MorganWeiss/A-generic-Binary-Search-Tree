#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "BST.h"


TEST_CASE("An empty tree", "[Tree]") {
	BST<int> tree;

	REQUIRE(tree.empty());

	SECTION("Inserting an element makes the tree not empty") {
		tree.insert(50);

		REQUIRE(!tree.empty());
	}

	SECTION("Inserting an element increses the depth of the tree") {
		tree.insert(50);
		tree.insert(30);

		REQUIRE(tree.depth() == 2u);
	}

	SECTION("Delete a node from an empty tree does nothing") {
		tree.insert(50);
		tree.deleteNode(50);

		REQUIRE(tree.depth() == 0u);
		REQUIRE(tree.empty());
	}
}

TEST_CASE("Create a tree with multiple nodes", "[Tree]") {
	BST<int> tree;
	tree.insert(50);
	tree.insert(30);
	tree.insert(20);
	tree.insert(40);
	tree.insert(70);
	tree.insert(60);
	tree.insert(80);
	REQUIRE(!tree.empty());

	SECTION("Create copy of tree to test if they are equivalent") {
		BST<int> newTree = tree;
		newTree == tree ? std::cout << "Equal \n" : std::cout << "Not equal \n";
	}

	SECTION("Test inorder traversal function produce correct output") {
		std::stack<int> order;
		order.push(80);
		order.push(70);
		order.push(60);
		order.push(50);
		order.push(40);
		order.push(30);
		order.push(20);
		tree.inorderTraversal([&](auto val) {
			REQUIRE(val == order.top());
			order.pop();
		});
	}

	SECTION("Test preorder traversal function produce correct output") {
		std::queue<int> order;
		order.push(50);
		order.push(30);
		order.push(20);
		order.push(40);
		order.push(70);
		order.push(60);
		order.push(80);
		tree.preorderTraversal([&](auto val) {
			REQUIRE(val == order.front());
			order.pop();
		});
	}

	SECTION("Test postorder traversal function produce correct output") {
		std::queue<int> order;
		order.push(20);
		order.push(40);
		order.push(30);
		order.push(60);
		order.push(80);
		order.push(70);
		order.push(50);
		tree.postorderTraversal([&](auto val) {
			REQUIRE(val == order.front());
			order.pop();
		});
	}
}