/*	Pentru acest laborator am avut de implementat operatiile de Build_Tree, OS_Select si OS_Delete.Am folosit o structura
* care contine o cheie, dimensiunea subarborelor nodului respectiv si 3 pointeri spre parinte, spre copilul din dreapta si
* cel din stanga.
* 
*	Pentru Build_Tree am folosit o metoda Divide And Conquer, in care am adaugat pe rand elementul de la mijlocul unui vector
* iar apoi am apelat functia recursiv pe restul vectorului din stanga si din dreapta mijlocului. Complexitatea functiei e O(n).
* 
*	Pentru OS_Select m-am folosit de campul de size al nodului pentru a determina rangul acestuia, in functie de care se returneaza
* nodul, se apeleaza functia pe copilul din stanga sau pe cel din dreapta. Complexitatea pentru OS_Select este O(logn).
* 
*	Pentru OS_Delete am utilizat o functie findNode, care imi returneaza nodul in functie de cheia acestuia, o functie findSuccesor
* care returneaza cel mai mic element din subarborele drept si de o functie adjustSize, care decrementeaza size-ul fiecarui nod pornind
* de la nodul respectiv pana la root. Functia de OS_Delete verifica pentru fiecare din urmatoarele cazuri si sterge nodul dorit:
*		- nu are copii (se sterge direct)
*		- are un singur copil (se inlocuieste cu copilul)
*		-are ambii copii (se inlocuieste cu succesorul si cel din urma va fi sters clasic).
* Complexitatea e O(logn).
*/

#include <iostream>
#include "Profiler.h"
using namespace std;

#define MAX_SIZE 10000
#define STEP 100
#define TEST 5

int opS, opD, opB;

Profiler p("Lab 7");

struct node
{
	int key;
	int size;
	node* parent;
	node* left;
	node* right;
};

node* arrayToTree(int v[], int start, int end)
{
	if (start > end)
		return NULL;

	opB += 7;
	int mid = (start + end) / 2;

	node* temp = new node();
	temp->key = v[mid];
	temp->parent = NULL;

	temp->left = arrayToTree(v, start, mid - 1);
	if (temp->left != NULL)
	{
		opB++;
		temp->left->parent = temp;
	}

	temp->right = arrayToTree(v, mid + 1, end);
	if (temp->right != NULL)
	{
		opB++;
		temp->right->parent = temp;
	}

	temp->size = 0;
	
	if (temp->left != NULL)
	{
		opB++;
		temp->size++;
	}
	if (temp->right != NULL)
	{
		opB++;
		temp->size++;
	}

	if (temp->left != NULL && temp->right != NULL)
	{
		opB++;
		temp->size += temp->left->size + temp->right->size;
	}

	return temp;
}

node* buildTree(int n)
{
	int v[MAX_SIZE];
	
	for (int i = 0; i < n; i++)
		v[i] = i + 1;

	node* root = arrayToTree(v, 0, n - 1);

	return root;
}

node* osSelect(node* root, int i)
{
	int rank;
	node* ptr = root;

	opS++;
	if (ptr->left != NULL)
	{
		opS++;
		rank = ptr->left->size + 2;
	}
	else
	{
		opS++;
		rank = 1;
	}

	opS++;
	if (i == rank)
		return ptr;
	else
	{
		opS++;
		if (i < rank)
			return osSelect(ptr->left, i);
		else
			return osSelect(ptr->right, i - rank);
	}
}

node* findNode(node* root, int key)
{
	node* ptr = root;

	opD += 3;
	if (ptr == NULL)
		return ptr;

	if (ptr->key == key)
		return ptr;

	if (ptr->key > key)
		return findNode(ptr->left, key);

	return findNode(ptr->right, key);
}

void adjustSize(node* parent)
{
	node* ptr = parent;
	while (ptr != NULL)
	{
		opD += 2;
		ptr->size--;
		ptr = ptr->parent;
	}
}

node* findSuccessor(node* root)
{
	node* ptr = root;
	opD++;
	while (ptr->left != NULL)
	{
		opD += 2;
		ptr = ptr->left;
	}

	return ptr;
}

void osDelete(node* root, int key)
{
	opD += 4;
	node* deleted = findNode(root, key);

	if (deleted != NULL)
	{
		if (deleted->left == NULL && deleted->right == NULL)
		{
			opD++;
			if (deleted->parent != NULL)
				if (deleted->key < deleted->parent->key)
				{
					opD++;
					deleted->parent->left = NULL;
				}
				else
				{
					opD++;
					deleted->parent->right = NULL;
				}
			adjustSize(deleted->parent);
		}
		
		else if (deleted->left != NULL && deleted->right == NULL)
		{
			opD++;
			if(deleted->parent != NULL)
				if (deleted->key < deleted->parent->key)
				{
					opD += 2;
					deleted->parent->left = deleted->left;
					deleted->left->parent = deleted->parent;
				}
				else
				{
					opD += 2;
					deleted->parent->right = deleted->left;
					deleted->left->parent = deleted->parent;
				}

			adjustSize(deleted->parent);
		}

		else if (deleted->left == NULL && deleted->right != NULL)
		{
			opD++;
			if (deleted->parent != NULL)
				if (deleted->key < deleted->parent->key)
				{
					opD += 2;
					deleted->parent->left = deleted->right;
					deleted->right->parent = deleted->parent;
				}
				else
				{
					opD += 2;
					deleted->parent->right = deleted->right;
					deleted->right->parent = deleted->parent;
				}

			adjustSize(deleted->parent);
		}

		else if (deleted->right != NULL && deleted->left != NULL)
		{
			node* succ = findSuccessor(deleted->right);

			osDelete(root, succ->key);

			opD++;
			deleted->key = succ->key;

		}
	}
}

void prettyPrint(node* root, int spaces)
{
	if (root != NULL)
	{
		for (int i = 0; i < 2 * spaces; i++)
			printf("   ");

		printf("%d (%d)\n", root->key, root->size);

		if (root->left != NULL)
			prettyPrint(root->left, spaces + 1);
		if (root->right != NULL)
			prettyPrint(root->right, spaces + 1);
	}
}

void demo()
{
	node* root = buildTree(11);
	
	printf("Original tree: \n");
	prettyPrint(root, 0);
	printf("\n");

	node* found1 = osSelect(root, 2);
	printf("\n OS_SELECT: %d\n", found1->key);
	printf("\nTree after deleting %d:\n", found1->key);
	osDelete(root, found1->key);
	prettyPrint(root, 0);

	found1 = osSelect(root, 5);
	printf("\n OS_SELECT: %d\n", found1->key);
	printf("\nTree after deleting %d:\n", found1->key);
	osDelete(root, found1->key);
	prettyPrint(root, 0);

	found1 = osSelect(root, 7);
	printf("\n OS_SELECT: %d\n", found1->key);
	printf("\nTree after deleting %d:\n", found1->key);
	osDelete(root, found1->key);
	prettyPrint(root, 0);

}

void perf()
{
	int nrElements, index;
	node* root;
	node* selected;

	for (int n = 100; n <= MAX_SIZE; n += STEP)
	{
		for (int test = 0; test < TEST; test++)
		{
			opB = 0;
			nrElements = n;
			root = buildTree(n);

			for (int elements = 0; elements < n; elements++)
			{
				index = 1 + rand() % nrElements;
				opS = 0;
				selected = osSelect(root, index);

				opD = 0;
				osDelete(root, selected->key);

				p.countOperation("Operatii OS_Select", n, opS);
				p.countOperation("Operatii OS_Delete", n, opD);

				nrElements--;
			}

			p.countOperation("Operatii Build_Tree", n, opB);
		}


	}

	p.divideValues("Operatii OS_Select", TEST);
	p.divideValues("Operatii OS_Delete", TEST);
	p.divideValues("Operatii Build_Tree", TEST);
	p.createGroup("Operatii", "Operatii Build_Tree", "Operatii OS_Select", "Operatii OS_Delete");
	p.showReport();
}

int main()
{
	demo();
	//perf();
	return 0;
}