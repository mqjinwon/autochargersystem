#include "treeBFS.h"

mapTree::mapTree(int x, int y){
	Node *newNode = new Node;
	int pos[] = { x,y };
	newNode->putPos(pos);
	map.putCheck(x, y);
}

bool mapTree::add(int data)
{
	Node *newNode = new Node;

	newNode->putData(data);

	if (isEmpty())
	{
		root = newNode;
	}
	else
	{
	}
}
bool mapTree::remove(int m_key)
{
	Node *ptrNode = root;
	Node *beforeSmallNode = nullptr;

	if (isEmpty())
	{
		cout << "it is empty!!" << endl;
		return false;
	}
	else
	{
		while (1)
		{
			int node_Key = ptrNode->getData().key;

			if (node_Key > m_key) // left side
			{
				if (ptrNode->left == nullptr)
				{
					cout << "there isn't right key!!" << endl;
					return false;
				} 

				beforeSmallNode = ptrNode;
				ptrNode = ptrNode->left;
			}
			else if (node_Key < m_key) // right side
			{
				if (ptrNode->right == nullptr)
				{
					cout << "there isn't right key!!" << endl;
					return false;
				}

				beforeSmallNode = ptrNode;
				ptrNode = ptrNode->right;

			}
			else // if it is right key...
			{
				if (ptrNode == root)
				{
					Node *delNode = ptrNode;

					if (root->left == nullptr && root->right == nullptr)
					{
						delete delNode;
					}
					else if (root->left != nullptr && root->right == nullptr) //change root left direction's node
					{
						root = root->left;
						delete delNode;
					}
					else //change root right direction's node
					{
						ptrNode = ptrNode->right;

						while (ptrNode->left != nullptr)
						{
							beforeSmallNode = ptrNode;
							ptrNode = ptrNode->left;
							beforeSmallNode->left = ptrNode->right;

						}
						cout << "root is changed from " << root->getData().key << " to " << ptrNode->getData().key << endl;

						ptrNode->left = root->left;
						ptrNode->right = root->right;
						root = ptrNode;

						delete delNode;
					}

					return true;
				}
				else
				{
					if (ptrNode->left == nullptr && ptrNode->right == nullptr)
					{
						if (beforeSmallNode->left == ptrNode)
						{
							beforeSmallNode->left = nullptr;
						}
						else
						{
							beforeSmallNode->right = nullptr;
						}

						delete ptrNode;
					}
					else if (ptrNode->left != nullptr && ptrNode->right != nullptr)
					{
						Node* delNode = ptrNode;

						while (ptrNode->left != nullptr) // going to smallest node
							ptrNode = ptrNode->left;

						if (beforeSmallNode->left == delNode)
						{
							beforeSmallNode->left = ptrNode;
							ptrNode->right = delNode->right;
							delete delNode;
						}
						else
						{
							beforeSmallNode->right = ptrNode;
							ptrNode->right = delNode->right;
							delete delNode;
						}

					}
					else if (ptrNode->left == nullptr)
					{
						if (beforeSmallNode->left == ptrNode)
						{
							beforeSmallNode->left = ptrNode->right;
						}
						else
						{
							beforeSmallNode->right = ptrNode->right;
						}

						delete ptrNode;
					}
					else
					{
						if (beforeSmallNode->left == ptrNode)
						{
							beforeSmallNode->left = ptrNode->left;
						}
						else
						{
							beforeSmallNode->right = ptrNode->left;
						}

						delete ptrNode;
					}
					return true;
				}

			}
		}
	}


}

