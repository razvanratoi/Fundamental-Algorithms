/* Pentru acest laborator am avut de implementat algoritmul de sortare QuickSort si algoritmul QuickSelect

	Quicksort: 
			-am folosit pentru implementarea algoritmului ultimul element al sirului ca si pivot pentru 
partitii.
			-complexitatea in cazul mediu este O(nlogn) si in cel mai rau caz O(n^2); cazul cel mai bun
este apropiat de cazul mediu.
	Quickselect: O(n), iar in cel mai rau caz O(n^2)
	
	In comparatie intre QuickSort si HeapSort, in cazul mediu, se poate observa cum numarul de operatii 
dintre cele 2 metode de sortare este mult mai mare in cazul lui HeapSort decat in cazul QuickSort-ului */

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Profiler.h"
using namespace std;

#pragma warning(disable:4996);

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define MAX_SIZE 10000
#define NO_TESTS 5
#define STEP 100

Profiler p("quicksort");
int opH, opQ;


void heapify(int heap[], int size, int root)
{
	int left = 2 * root + 1;
	int right = 2 * root + 2;
	int largest;

	opH++;
	if ((left <= size) && (heap[left] > heap[root]))
		largest = left;
	else
		largest = root;

	opH++;
	if ((right <= size) && (heap[right] > heap[largest]))
		largest = right;

	if (largest != root)
	{
		opH += 3;
		swap(heap[root], heap[largest]);
		heapify(heap, size, largest);
	}
}

void bottomUp(int heap[], int n)
{
	for (int i = n / 2; i >= 0; i--)
		heapify(heap, n, i);
}

void heapSort(int heap[], int n)
{
	bottomUp(heap, n);
	for (int j = n - 1; j >= 0; j--)
	{
		opH += 3;
		swap(heap[0], heap[j]);
		heapify(heap, j - 1, 0);
	}
}

int partition(int v[], int start, int end)
{
	opQ++;
	int pivot = v[end];
	int i = start;
	for (int j = start; j < end; j++)
	{
		opQ++;
		if (v[j] <= pivot)
		{
			opQ += 3;
			swap(v[i], v[j]);
			i++;
		}
	}
	opQ += 3;
	swap(v[i], v[end]);
	return i;
}

void quickSort(int v[], int start, int end)
{
	int q;
	if (start < end)
	{
		q = partition(v, start, end);
		quickSort(v, start, q - 1);
		quickSort(v, q, end);
	}
}

int randomPartition(int v[], int start, int end)
{
	int iRandom = rand() % (end+1);
	swap(v[end], v[iRandom]);
	return partition(v, start, end);
}

int quickSelect(int v[], int start, int end, int element)
{
	int q, k;
	if (start == end)
		return v[start];
	q = randomPartition(v, start, end);
	k = q - start + 1;
	if (element == k)
		return v[q];
	else
		if (element < k)
			return quickSelect(v, start, q - 1, element);
		else
			return quickSelect(v, q + 1, end, element - k);
}

void demo()
{
	int v[] = { 5, 3, 7, 2, 8, 1, 4, 6 };
	int n = sizeof(v) / sizeof(v[0]);
	quickSort(v, 0, n-1);
	//heapSort(v, n);
	for (int k = 0; k < n; k++)
		printf("%d ", v[k]);
}

void demoQuickSelect(int element)
{
	int v[] = { 5, 3, 7, 2, 8, 1, 4, 6 };
	int n = sizeof(v) / sizeof(v[0]);
	int result = quickSelect(v, 0, n - 1, element);
	printf("\n%d", result);
}



void perfQuick()
{
	int n, v1[MAX_SIZE], v2[MAX_SIZE];
	Operation opW = p.createOperation("Operatii Worst", MAX_SIZE);
	Operation opA = p.createOperation("Operatii Average", MAX_SIZE);

	for (int n = STEP; n <= MAX_SIZE; n += STEP)
	{
		opQ = 0;
		FillRandomArray(v1, n, 10, 50000, false, UNSORTED);
		quickSort(v1, 0, n - 1);
		p.countOperation("Operatii Average", n, opQ);

		opQ = 0;
		FillRandomArray(v2, n, 10, 50000, false, ASCENDING);
		quickSort(v2, 0, n - 1);
		p.countOperation("Operatii Worst", n, opQ);

	}


	p.createGroup("Comparison Avg-Worst", "Operatii Worst", "Operatii Average");
	p.showReport();
}

void perfComparison()
{
	int n, test, v1[MAX_SIZE], v2[MAX_SIZE];
	Operation oH = p.createOperation("HeapSort", MAX_SIZE);
	Operation oQ = p.createOperation("QuickSort", MAX_SIZE);

	for (n = STEP; n <= MAX_SIZE; n += STEP)
	{

		for (test = 0; test < NO_TESTS; test++)
		{
			opH = 0;
			opQ = 0;

			FillRandomArray(v1, n);
			CopyArray(v2, v1, n);

			quickSort(v1, 0, n - 1);
			heapSort(v2, n);

			p.countOperation("QuickSort", n, opQ);
			p.countOperation("Heapsort", n, opH);
		}
	}

	p.divideValues("QuickSort", NO_TESTS);
	p.divideValues("HeapSort", NO_TESTS);

	p.createGroup("HeapSort  QuickSort", "HeapSort", "QuickSort");
	p.showReport();
}

int main()
{
	demo();
	demoQuickSelect(5);
	//perfComparison();
	//perfQuick();
	return 0;
}