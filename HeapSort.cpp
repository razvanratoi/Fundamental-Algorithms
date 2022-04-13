/* Ratoi Razvan 30421
	Am avut de implementat cele 2 metode de construire a heap-ului, Top Down si Bottom Up, si  algoritmul
de sortare folosind una din metodele mentionate mai sus, Heap Sort
 
	Pentru Top Down:
		-complexitatea e de O(nlogn), deoarece functia increaseKey parcurge in cel mai rau caz inaltimea 
 "arborelui", care este log n si functia topDown apeleaza functia insert (ce contine increaseKey) de n ori;

	Pentru Bottom Up:
		-complexitatea e de O(n), deoarece se apeleaza de n/2 ori heapify.

	Pentru Heap Sort:
		-complexitatea este de O(nlogn), deoarece Build Heap dureaza O(n) in cazul Bottom Up si O(nlogn) in 
cazul Top Down, iar loop-ul for O(nlogn) (fiecare heapify dureaza O(h) unde h are valori intre lgn si 1)
	*/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Profiler.h"
using namespace std;

#define MAX_SIZE 10000
#define STEP 100
#define NO_TESTS 5

#pragma warning(disable:4996);

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif 

Profiler p("Lab3");

void increaseKey(int heap[], int position, int value)
{
	p.countOperation("Atribuiri Top Down", position, 1);
	heap[position] = value;
	p.countOperation("Comparatii Top Down", position, 1);
	while (heap[(position - 1) / 2] < heap[position] && position >= 0)
	{
		p.countOperation("Atribuiri Top Down", position, 3);
		swap(heap[(position - 1) / 2], heap[position]);
		position = (position - 1) / 2;
	}
}

void insert(int heap[], int size, int value)
{
	p.countOperation("Atribuiri Top Down",size, 1);
	heap[size] = -1;
	increaseKey(heap, size, value);
}

void topDown(int heap[], int v[], int length)
{
	Operation opAT = p.createOperation("Atribuiri Top Down", length);
	Operation opCT = p.createOperation("Comparatii Top Down", length);
	for (int i = 0; i < length; i++)
		insert(heap, i, v[i]);
}

void heapify(int heap[],int size, int root)
{
	int left = 2 * root + 1;
	int right = 2 * root + 2;
	int largest;

	p.countOperation("Comparatii Bottom Up", size, 1);
	if ((left <= size) && (heap[left] > heap[root]))
		largest = left;
	else
		largest = root;

	p.countOperation("Comparatii Bottom Up", size, 1);
	if ((right <= size) && (heap[right] > heap[largest]))
		largest = right;

	if (largest != root)
	{
		p.countOperation("Atribuiri Bottom Up", size, 3);
		swap(heap[root], heap[largest]);
		heapify(heap, size, largest);
	}
}

void bottomUp(int heap[], int n)
{
	Operation opAB = p.createOperation("Atriburi Bottom Up", n);
	Operation opCB = p.createOperation("Comparatii Bottom Up", n);
	for (int i = n / 2; i >= 0; i--)
		heapify(heap, n, i);
}

void heapSortBU(int heap[], int n)
{
	bottomUp(heap, n);
	for (int j = n - 1; j >= 0; j--)
	{
		swap(heap[0], heap[j]);
		heapify(heap, j-1, 0);
	}
}

void heapSortTD(int heap[], int v[], int length)
{
	topDown(heap, v, length);
	for (int j = length - 1; j >= 0; j--)
	{
		swap(heap[0], heap[j]);
		heapify(heap, j - 1, 0);
	}
}

void demo()
{
	int heap1[] = { 4, 7, 1, 8, 6, 2, 9, 3, 5 };
	int heap2[MAX_SIZE];
	int v[] = { 4, 7, 1, 8, 6, 2, 9, 3, 5 };
	int length = sizeof(v) / sizeof(v[0]);
	int n = sizeof(heap1) / sizeof(heap1[0]);
	//heapSortBU(heap1, n);			  //Heap Sort with Bottom Up Build
	//heapSortTD(heap2, v, length);   //Heap Sort with Top Down Build
	//bottomUp(heap1, n);
	//topDown(heap2, v, length);
	for (int i = 0; i < n; i++)
	{
		//printf("%d ", heap1[i]);    //for Build Heap Bottom Up
		//printf("%d ", heap2[i]);    //for Build Heap Top Down
	}
		
}

void perf()
{
	int heap1[MAX_SIZE], heap2[MAX_SIZE], v[MAX_SIZE], n, test;
	for (n = STEP; n <= MAX_SIZE; n += STEP)
	{
		for (test = 0; test < NO_TESTS; test++)
		{
			FillRandomArray(heap1, n);
			for (int k = 0; k < n; k++)
				v[k] = heap1[k];

			bottomUp(heap1, n);
			topDown(heap2, v, n);
		}
	}

		p.divideValues("Atribuiri Bottom Up", NO_TESTS);
		p.divideValues("Comparatii Bottom Up", NO_TESTS);
		p.addSeries("Total Bottom Up", "Atribuiri Bottom Up", "Comparatii Bottom Up");
		
		p.divideValues("Atribuiri Top Down", NO_TESTS);
		p.divideValues("Comparatii Top Down", NO_TESTS);
		p.addSeries("Total Top Down", "Atribuiri Top Down", "Comparatii Top Down");

		p.createGroup("Atribuiri", "Atribuiri Top Down", "Atribuiri Bottom Up");
		p.createGroup("Comparatii", "Comparatii Top Down", "Comparatii Bottom Up");
		p.createGroup("Total", "Total Top Down", "Total Bottom Up");

		p.showReport();
}

int main()
{
	perf();
	//demo();
	return 0;
}