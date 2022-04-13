/*	Am avut de implementat interclasarea a k liste sortate, cu n elemente in total. 
* Pentru acest algoritm am avut urmatoarele functii:
*	-createNode & addNode, pentru a crea, respectiv adauga un nod intr-o lista inlantuita
*	-heapify & buildHeap, folosite pentru a detecta elementele minime din liste
*	-generateList, care genereaza un array de liste bazat pe parametrii n si k
*	-kMerge, care functioneaza astfel:
* 
*		*ia primul element si il adauga intr-un heap iar dupa se reconstituie heap-ul;
*		*se extrage primul element din heap (minimul) si se adauga in lista rezultata;
*		*se introduce in heap urmatorul element din lista de unde a apartinut minimul anterior;
*		*reconstituim heap-ul si repetam aceiasi pasi pana mai ramane o singura lista;
*		*introducem ultima lista cum este in lista rezultata;
*		*returnam lista rezultata.
* 
*	Pentru fiecare element din lista se produce un heapify care are complexitatea log(k). Astfel,
* avand k liste si n/k elemente in fiecare lista, vom avea un numar total de n elemente. Complexitatea
* finala a algoritmului de interclasare a k liste este O(nlogk). */

#include <iostream>
#include "Profiler.h"
using namespace std;

#pragma warning(disable:4996)
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#define MAX_HEAP_SIZE 100
#define STEP 100
#define MAX_SIZE 10000

Profiler p("K-Way Merge");

int op;

struct Node {
	int data;
	Node* next;
};

struct HeapI {
	int value;
	int index;
};

Node* createNode(int data)
{
	Node* inserted = new Node();
	inserted->data = data;
	inserted->next = NULL;
	return inserted;
}

void addNode(Node** head, int data)
{
	Node* temp = createNode(data);
	Node* last;

	if (*head == NULL)
	{
		*head = temp;
		op++;
	}
	else
	{
		last = *head;
		op++;
		while (last->next != NULL)
			last = last->next;
		last->next = temp;
		op++;
	}
}

void printList(Node* node)
{
	while (node != NULL)
	{
		printf(" %d ", node->data);
		node = node->next;
	}
}

void heapify(HeapI heap[], int size, int root)
{
	int left = 2 * root + 1;
	int right = 2 * root + 2;
	int smallest;

	op++;
	if ((left < size) && (heap[left].value < heap[root].value))
		smallest = left;
	else
		smallest = root;

	op++;
	if ((right < size) && (heap[right].value < heap[smallest].value))
		smallest = right;

	if (smallest != root)
	{
		op += 3;
		swap(heap[root], heap[smallest]);
		heapify(heap, size, smallest);
	}
}

void buildHeap(HeapI heap[], int size)
{
	for (int i = size / 2; i >= 0; i--)
		heapify(heap, size, i);
}

Node* kMerge(Node* list[], int k)
{
	Node* sortedList = new Node();
	HeapI heap[MAX_HEAP_SIZE];
	int heapSize = k, firstElement = 0;

	for (int i = 0; i < heapSize; i++)
	{
		op += 2;
		heap[i].value = list[i]->data;
		heap[i].index = i;
	}
	buildHeap(heap, heapSize);

	while (heapSize > 1)
	{
		if (firstElement == 0)
			sortedList = createNode(heap[0].value);
		else
			addNode(&sortedList, heap[0].value);

		firstElement = 1;

		op++;
		if (list[heap[0].index]->next != NULL)
		{
			op += 2;
			list[heap[0].index] = list[heap[0].index]->next;
			heap[0].value = list[heap[0].index]->data;
			heapify(heap, heapSize, 0);
		}
		else
		{
			op++;
			heap[0] = heap[heapSize - 1];
			heapSize--;
			heapify(heap, heapSize, 0);
		}
	}

	op++;
	while (list[heap[0].index] != NULL)
	{
		op += 2;
		addNode(&sortedList, list[heap[0].index]->data);
		list[heap[0].index] = list[heap[0].index]->next;
	}

	return sortedList;
}

void generateList(Node* list[], int n, int k, int minV, int maxV)
{
	int v[2000];
	for (int i = 0; i < k; i++)
	{
		FillRandomArray(v, n / k, minV, maxV, false, 1);
		list[i] = createNode(v[0]);
		for (int j = 1; j < n / k; j++)
			addNode(&list[i], v[j]);
	}
}

void demo()
{
	Node* sorted = new Node();
	Node* list[4];
	HeapI heap[4];

	generateList(list, 20, 4, 0, 100);
	for (int j = 0; j < 4; j++)
	{
		Node* printed = list[j];
		while (printed != NULL)
		{
			printf("%d ", printed->data);
			printed = printed->next;
		}
		printf("\n");
	}

	sorted = kMerge(list, 4);
	printList(sorted);
}

void perf1()
{
	Node* list[MAX_HEAP_SIZE];
	Node* sortedList;
	int k[] = { 5, 10, 100 };
	for (int nrLists = 0; nrLists < 3; nrLists++)
	{
		for (int n = STEP; n <= MAX_SIZE; n += STEP)
		{
			generateList(list, n, k[nrLists], 0, 50000);
			op = 0;
			sortedList = kMerge(list, k[nrLists]);
			switch (k[nrLists])
			{
			case 5:
				p.countOperation("Operatii 5 Liste", n, op);
				break;
			case 10:
				p.countOperation("Operatii 10 Liste", n, op);
				break;
			default:
				p.countOperation("Operatii 100 Liste", n, op);
			}
		}
	}

	p.createGroup("Operatii", "Operatii 5 Liste", "Operatii 10 Liste", "Operatii 100 Liste");
	p.showReport();
}

int main()
{
	demo();
	//perf1();
}