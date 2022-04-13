/* Am avut de implementat algoritmii de sortare directa: Bubble Sort, Insertion Sort si Selection Sort 
* 
 Bubble Sort: stabil, deoarece parcurgerea si schimbarea elementelor se face pe rand, pastrand ordinea valorilor egale
	-Best case: un sir ordonat crescator (1, 2, 3, 4, 5), cu complexitatile:
			-Atribuiri: O(1) -> nu se ajunge la niciun swap
			-Comparatii: O(n) ->se verifica 2 conditii pentru fiecare bucla a lui for
			-Total: O(n)
	-Worst case: un sir ordonat descrescator(5, 4, 3, 2, 1)
			-Atribuiri: O(n^2) -> 3 atribuiri din swap, de n ori pentru for si n ori pentru while
			-Comparatii: O(n^2) ->  1 comparatie in for, 1 in while, fiecare repetandu-se de n ori
			-Total: O(n^2)
	-Average case: un sir neordonat
			-Atribuiri: O(n^2)
			-Comparatii: O(n^2)
			-Total: O(n^2)

Insertion Sort: stabil, datorita inecuatiei aux < v[j]
	-Best case: un sir ordonat crescator
			-Atribuiri: O(n) -> 2 atribuiri care se repeta de n-1 ori
			-Comparatii: O(n) -> 1 comparatie de n-1 ori
			-Total: O(n)
	-Worst case: un sir ordonat descrescator
			-Atribuiri: O(n^2) -> initial: 3 atriburi; cu fiecare inc. a lui i, se adauga 1 la nr. de atribuiri
			-Comparatii: O(n^2) -> initial: 2 comp., valoarea creste pana la n+1 la fiecare bucla
			-Total: O(n^2)
	-Average case: un sir oarecare
			-Atribuiri: O(n^2)
			-Comparatii: O(n^2)
			-Total: O(n^2)

Selection sort: instabil, din cauza functiei swap
	-Best case: un sir crescator
			-Atribuiri: O(1) -> nu se realizeaza nicio atribuire deoarece fiecare element e in locul corect
			-Comparatii: O(n^2) -> initial: n-1 comparatii, valoarea scade la fiecare bucla
			-Total: O(n^2)
	-Worst case: primul element: maximul, restul sirului ordonat crescator (5, 1, 2, 3, 4)
			-Atribuiri: O(n) -> pentru fiecare valoare a lui i, se realizeaza cate 3 atribuiri
			-Comparatii: O(n^2) -> acelasi caz ca la Best
			-Total: O(n^2)
	-Average case: sir oarecare
			-Atribuiri: O(n)
			-Comparatii: O(n^2)
			-Total: O(n^2)

	Consider ca cel mai eficient algoritm dintre cele 3 este Selection Sort, deoarece in cazurile majoritare 
(Average) si chiar si in cel mai rau caz (Worst), complexitatile sunt O(n) respectiv O(n^2), fata de celelalte
2 metode care au in aceste cazuri ambele complexitati O(n^2).
			*/

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "Profiler.h"
using namespace std;

Profiler p("Lab 1");

#define MAX_SIZE 10000
#define STEP 100
#define NR_TESTS 5

void bubbleSort(int v[], int n)
{
	Operation opA = p.createOperation("Atribuiri Bubble", n);
	Operation opC = p.createOperation("Comparatii Bubble", n);
	Operation opT = p.createOperation("Total Bubble", n);

	int unsorted;
	do
	{
		unsorted = 0;
		for (int i = 0; i < n - 1; i++)
		{
			opC.count();
			if (v[i] > v[i + 1])
			{
				opA.count(3);
				swap(v[i], v[i + 1]);
				unsorted = 1;
			}
		}
	} while (unsorted == 1);
}

void insertionSort(int v[], int n)
{
	Operation opA = p.createOperation("Atribuiri Insertion", n);
	Operation opC = p.createOperation("Comparatii Insertion", n);
	Operation opT = p.createOperation("Total Insertion", n);

	int aux, j;
	for (int i = 0; i < n - 1; i++)
	{
		j = i;
		opA.count();
		aux = v[i + 1];
		opC.count();
		while (j >= 0 && aux < v[j])
		{
			opA.count();
			v[j + 1] = v[j];
			j--;
			opC.count();
		}
		opA.count();
		v[j + 1] = aux;
	}
}

void selectionSort(int v[], int n)
{
	Operation opA = p.createOperation("Atribuiri Selection", n);
	Operation opC = p.createOperation("Comparatii Selection", n);
	Operation opT = p.createOperation("Total Selection", n);

	int j, index;
	for (int i = 0; i < n - 1; i++)
	{
		index = i;
		for (j = i + 1; j < n; j++)
		{
			opC.count();
			if (v[j] < v[index])
				index = j;
		}
		if (i != index)
		{
			opA.count(3);
			swap(v[i], v[index]);
		}
	}
}

void demo(char sortMethod) {
	int v[] = { 5, 6, 2, 9, 3, 7 };
	int n = sizeof(v) / sizeof(v[0]);

	switch (sortMethod)
	{
	case 'B':
		bubbleSort(v, n);
		break;
	case 'I':
		insertionSort(v, n);
		break;
	case 'S':
		selectionSort(v, n);
		break;
	default:
		break;
	}

	for (int i = 0; i < n; i++)
		printf("%d ", v[i]);
}

void perfSelection() {
	int v[MAX_SIZE], n;

	p.reset("BEST");
	for (n = STEP; n <= MAX_SIZE; n += STEP)
		{
			FillRandomArray(v, n, 0, 50000, false, ASCENDING);
			selectionSort(v, n);
			p.addSeries("Total Selection", "Atribuiri Selection", "Comparatii Selection");
		}

	p.reset("WORST");
	for (n = STEP; n <= MAX_SIZE; n += STEP)
	{
		FillRandomArray(v, n, 0, 49999, false, ASCENDING);
		v[0] = 50000;
		selectionSort(v, n);
		p.addSeries("Total Selection", "Atribuiri Selection", "Comparatii Selection");
	}

	p.reset("AVERAGE");
	for (n = STEP; n <= MAX_SIZE; n += STEP)
	{
			for (int test = 0; test < NR_TESTS; test++)
			{
				FillRandomArray(v, n);
				selectionSort(v, n);
			}
	}
	p.divideValues("Atribuiri Selection", NR_TESTS);
	p.divideValues("Comparatii Selection", NR_TESTS);
	p.addSeries("Total Selection", "Atribuiri Selection", "Comparatii Selection");
	//p.showReport();
} 

void perfInsertion()
{
	int v[MAX_SIZE], n;

	p.reset("BEST");
	for (int n = STEP; n <= MAX_SIZE; n += STEP)
	{
		FillRandomArray(v, n, 0, 50000, false, ASCENDING);
		insertionSort(v, n);
		p.addSeries("Total Insertion", "Atribuiri Insertion", "Comparatii Insertion");
	}

	p.reset("WORST");
	for (int n = STEP; n <= MAX_SIZE; n += STEP)
	{
		FillRandomArray(v, n, 0, 50000, false, DESCENDING);
		insertionSort(v, n);
		p.addSeries("Total Insertion", "Atribuiri Insertion", "Comparatii Insertion");
	}
	
	p.reset("AVERAGE");
	for (int n = STEP; n <= MAX_SIZE; n += STEP)
	{
		for (int test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(v, n, 0, 50000, false, UNSORTED);
			insertionSort(v, n);
		}
		
	}
	p.divideValues("Atribuiri Insertion", NR_TESTS);
	p.divideValues("Comparatii Insertion", NR_TESTS);
	p.addSeries("Total Insertion", "Atribuiri Insertion", "Comparatii Insertion");
	//p.showReport();
}

void perfBubble()
{
	int v[MAX_SIZE], n;

	p.reset("BEST");
	for (int n = STEP; n <= MAX_SIZE; n += STEP)
	{
		FillRandomArray(v, n, 0, 50000, false, ASCENDING);
		bubbleSort(v, n);
		p.addSeries("Total Bubble", "Atribuiri Bubble", "Comparatii Bubble");
	}

	p.reset("WORST");
	for (int n = STEP; n <= MAX_SIZE; n += STEP)
	{
		FillRandomArray(v, n, 0, 50000, false, DESCENDING);
		bubbleSort(v, n);
		p.addSeries("Total Bubble", "Atribuiri Bubble", "Comparatii Bubble");
	}

	p.reset("AVERAGE");
	for (int n = STEP; n <= MAX_SIZE; n += STEP)
	{
		for (int test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(v, n, 0, 50000, false, UNSORTED);
			bubbleSort(v, n);
		}
		
	}
	p.divideValues("Atribuiri Bubble", NR_TESTS);
	p.divideValues("Comparatii Bubble", NR_TESTS);
	p.addSeries("Total Bubble", "Atribuiri Bubble", "Comparatii Bubble");
	p.showReport();
}

void comparison()
{
	int v1[MAX_SIZE];
	int v2[MAX_SIZE];
	int v3[MAX_SIZE];
	int n, test;

	for (n = STEP; n <= MAX_SIZE; n += STEP)
	{
		for (test = 0; test < NR_TESTS; test++)
		{
			FillRandomArray(v1, n);
			for (int i = 0; i <= n; i++)
			{
				v2[i] = v1[i];
				v3[i] = v1[i];
			}
			insertionSort(v1, n);
			selectionSort(v2, n);
			bubbleSort(v3, n);
		}
	}


	p.divideValues("Atribuiri Insertion", NR_TESTS);
	p.divideValues("Comparatii Insertion", NR_TESTS);
	p.addSeries("Total Insertion", "Atribuiri Insertion", "Comparatii Insertion");

	p.divideValues("Atribuiri Selection", NR_TESTS);
	p.divideValues("Comparatii Selection", NR_TESTS);
	p.addSeries("Total Selection", "Atribuiri Selection", "Comparatii Selection");

	p.divideValues("Atribuiri Bubble", NR_TESTS);
	p.divideValues("Comparatii Bubble", NR_TESTS);
	p.addSeries("Total Bubble", "Atribuiri Bubble", "Comparatii Bubble");

	p.createGroup("Comparatie Atribuiri", "Atribuiri Insertion", "Atribuiri Selection", "Atribuiri Bubble");
	p.createGroup("Comparatie Comparatii", "Comparatii Insertion", "Comparatii Selection", "Comparatii Bubble");
	p.createGroup("Comparatie Total", "Total Insertion", "Total Selection", "Total Bubble");

	p.showReport();
}

int main() {
	demo('B');
	//perfSelection();
	//perfInsertion();
	//perfBubble();
	//comparison();
	return 0;
}