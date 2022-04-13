/* Ratoi Razvan, grupa 30421

	Pentru acest laborator am avut de implmentat algoritmul de parcurgere in adancime, sortarea topologica si algoritmul pentru 
componente tari conexe Tarjan. 
	DFS-ul are complexitate O(V+E), deoarece functia dfsVisit este apelata pentru fiecare nod (V) iar in interiorul acesteia
se ia in calcul fiecare muchie al grafului (E).
	Sortarea topologica are complexitate O(V+E), deoarece majoritatea algoritmului este realizat in interiorul DFS-ului.
	Tarjan are complexitatea O(V+E). */

#include <iostream>
#include <stack>
#include <stdlib.h>
#include "Profiler.h"
using namespace std;

int op;
Profiler p("dfs");

enum color{white, gray, black};

struct list
{
	int key;
	list* next;
};

struct node
{
	int key;
	int d;
	int f;
	int lowLink;
	node* parent;
	bool onStack;
	color color;
	int comp;
};

list* createNode(int key)
{
	list* new_node = new list();
	new_node->key = key;
	new_node->next = NULL;
	return new_node;
}

void addNode(list** head, int key)
{
	list* temp = createNode(key);
	list* ptr;
	temp->next = nullptr;

	if (*head == nullptr)
		*head = temp;
	else
	{
		ptr = *head;
		while (ptr->next != nullptr)
			ptr = ptr->next;
		ptr->next = temp;
	}
}

int fTime;
list* sorted = nullptr;
list* topSort = nullptr;

void dfsVisit(list* adj[], node* nodes[], node* start)
{
	op += 5;
	fTime++;
	start->d = fTime;
	start->color = gray;

	list* head = adj[start->key];
	while (head != nullptr)
	{
		op += 2;
		node* v = nodes[head->key];
		if (v->color == white)
		{
			op++;
			v->parent = start;
			dfsVisit(adj, nodes, v);
		}
		head = head->next;
		op += 2;
	}

	op += 3;
	start->color = black;
	fTime++;
	start->f = fTime;
	addNode(&sorted, start->key);
}

void dfs(list* adj[], node* nodes[], int n)
{
	for (int i = 0; i < n; i++)
	{
		op += 2;
		nodes[i]->color = white;
		nodes[i]->parent = nullptr;
	}

	op++;
	fTime = 0;

	for (int i = 0; i < n; i++)
	{
		op++;
		if (nodes[i]->color == white)
			dfsVisit(adj, nodes, nodes[i]);
	}
}

node* createVertex(int key)
{
	node* temp = new node();
	temp->key = key;
	return temp;
}

int index, nrComp;
std::stack<node*> st;

void strongConnect(list* adj[], node* nodes[], node* start)
{
	start->d = index;
	start->lowLink = index;
	index++;
	st.push(start);
	start->onStack = true;

	list* head = adj[start->key];
	while (head != nullptr)
	{
		if (nodes[head->key]->d == -1)
		{
			strongConnect(adj, nodes, nodes[head->key]);
			start->lowLink = min(start->lowLink, nodes[head->key]->lowLink);
		}
		else
			if (nodes[head->key]->onStack)
				start->lowLink = min(start->lowLink, nodes[head->key]->d);
		head = head->next;
	}

	if (start->lowLink == start->d)
	{
		node* v;
		nrComp++;
		do
		{
			v = st.top();
			st.pop();
			v->onStack = false;
			v->comp = nrComp;
		} while (v != start);
	}
}

void tarjan(list* adj[], node* nodes[], int n)
{
	for (int i = 0; i < n; i++)
	{
		nodes[i]->d = -1;
		nodes[i]->lowLink = -1;
		nodes[i]->onStack = false;
		nodes[i]->comp = -1;
	}

	for (int i = 0; i < n; i++)
		if (nodes[i]->d == -1)
			strongConnect(adj, nodes, nodes[i]);
}

void invertList()
{
	int arr[100];
	int i = 0;

	while (sorted != nullptr)
	{
		arr[i] = sorted->key;
		sorted = sorted->next;
		i++;
	}

	i--;
	for (int j = i; j >= 0; j--)
		addNode(&topSort, arr[j]);
}

bool isAcyclic(list* adj[], node* nodes[], int n)
{
	tarjan(adj, nodes, n);
	for (int i = 0; i < n-1; i++)
		for (int j = i+1; j < n; j++)
			if (nodes[i]->comp == nodes[j]->comp)
				return false;
	return true;
}

void printList(list* head)
{
	while (head != nullptr)
	{
		printf("%d ", head->key);
		head = head->next;
	}
}

void topologicalSort(list* adj[], node* nodes[], int n)
{
	if (isAcyclic(adj, nodes, n))
	{
		dfs(adj, nodes, n);
		invertList();
		printList(topSort);
	}
	else
		printf("\nThe graph contains cycles!\n");
}

void demo()
{
	node* nodes[6];
	for (int i = 0; i < 6; i++)
		nodes[i] = createVertex(i);

	list* adj[6];
	for (int i = 0; i < 6; i++)
		adj[i] = nullptr;

	addNode(&adj[0], 2);
	addNode(&adj[1], 0);
	addNode(&adj[2], 1);
	addNode(&adj[2], 3);
	addNode(&adj[3], 4);
	addNode(&adj[4], 3);
	addNode(&adj[4], 5);
	adj[5] = nullptr;

	list* adjL[6];
	for (int i = 0; i < 6; i++)
		adjL[i] = nullptr;
	addNode(&adjL[0], 2);
	addNode(&adjL[1], 2);
	addNode(&adjL[2], 4);
	addNode(&adjL[3], 4);
	addNode(&adjL[4], 5);
	adjL[5] = nullptr;

	for (int i = 0; i < 6; i++)
	{
		printf("%d: ", i);
		printList(adj[i]);
		printf("\n");
	}

	printf("\n");

	tarjan(adj, nodes, 6);
	for (int i = 0; i < 6; i++)
		printf("node %d in component %d\n", i, nodes[i]->comp);
	printf("\n");
	topologicalSort(adj, nodes, 6);
	printf("\n");

	for (int i = 0; i < 6; i++)
	{
		printf("%d: ", i);
		printList(adjL[i]);
		printf("\n");
	}

	printf("\n");

	index = 0;
	nrComp = 0;
	sorted = nullptr;
	topSort = nullptr;
	tarjan(adjL, nodes, 6);
	for (int i = 0; i < 6; i++)
		printf("node %d in component %d\n", i, nodes[i]->comp);
	printf("\n");
	topologicalSort(adjL, nodes, 6);
	printf("\n");
}

void perf()
{
	node* nodes[100];
	list* adj[5000];

	for (int i = 0; i < 100; i++)
		nodes[i] = createVertex(i);
	
	for (int n = 1000; n <= 5000; n += 100)
	{
		for (int i = 0; i < n; i++)
			adj[i] = nullptr;

		int nrEdges = 0;
		for (int i = 0; i < 99; i++)
			for (int j = i + 1; j < 100; j++)
			{
				if (nrEdges < n)
				{
					addNode(&adj[i], j);
					nrEdges++;
				}
			}

			for(int i = 99; i > 0; i--)
				for (int j = i - 1; j >= 0; j--)
				{
					if (nrEdges < n)
					{
						addNode(&adj[i], j);
						nrEdges++;
					}
				}

		op = 0;
		dfs(adj, nodes, 100);
		p.countOperation("DFS_Fixed_V",n, op);
	}

	node* node[200];
	list* adjL[9000];

	for (int i = 0; i < 200; i++)
		node[i] = createVertex(i);

	for (int n = 100; n <= 200; n += 10)
	{
		for (int i = 0; i < 9000; i++)
			adjL[i] = nullptr;

		int nrEdges = 0;
		for (int i = 0; i < n-1; i++)
			for (int j = i + 1; j < n; j++)
			{
				if (nrEdges < 9000)
				{
					addNode(&adjL[i], j);
					nrEdges++;
				}
			}

			for (int i = n-1; i > 0; i--)
				for (int j = i - 1; j >= 0; j--)
				{
					if (nrEdges < 9000)
					{
						addNode(&adjL[i], j);
						nrEdges++;
					}
				}

		op = 0;
		dfs(adjL, node, n);
		p.countOperation("DFS_Fixed_E",n, op);
	}

	p.showReport();

}

int main()
{
	demo();
	//perf();
	return 0;
}