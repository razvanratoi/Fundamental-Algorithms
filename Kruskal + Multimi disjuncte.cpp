/* Ratoi Razvan Valeriu, grupa 30421

    Pentru acest laborator am avut de implementat operatii pe multimi disjuncte, pe care le-am folosit mai apoi in algoritmul 
 lui Kruskal. 
    Complexitatea celui din urma este astfel:
        -partea de Make Set pentru fiecare nod din graf este O(V); V - numarul de noduri ale grafului
        -sortarea tuturor muchiilor in functie de pondere : O(ElogE); E - numarul de muchii
        -al doilea for din algoritm ia in calcul fiecare muchie din graf, deci se va repeta bluca de E ori;
        -interiorul for-ului este direct proportional cu numarul de varfuri.
    Din toate aceste aspecte avem complexitatea finala a algortimului O(E log E). */

#include <iostream>
#include "Profiler.h"
#include <time.h>
using namespace std;

#define MAX_SIZE 10000
#define STEP 100
#define TEST 5

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

Profiler p("lab 8");
int opMS, opFS, opU;

struct node
{
    int key;
    struct node* parent;
    int rank;
};

struct edge
{
    node* head;
    node* tail;
    int weight;
};

node* createNode(int key)
{
    node* temp = new node();
    temp->key = key;
    temp->parent = nullptr;
    return temp;
}

void makeSet(node* x)
{
    opMS += 2;
    x->parent = x;
    x->rank = 0;
}

void linkSet(node* x, node* y)
{
    opU++;
    if (x->rank > y->rank)
    {
        opU++;
        y->parent = x;
    }
    else
    {
        opU += 2;
        x->parent = y;
        if (x->rank == y->rank)
        {
            opU++;
            y->rank++;
        }
    }
}

node* findSet(node* x)
{
    opFS++;
    if (x != x->parent)
        x->parent = findSet(x->parent);
    return x->parent;
}

void unionSet(node* x, node* y)
{
    linkSet(findSet(x), findSet(y));
}

int partition(edge* edges[], int start, int end)
{
    int pivot = edges[end]->weight;
    int k = start;
    for (int j = start; j < end; j++)
    {
        if (edges[j]->weight <= pivot)
        {
            swap(edges[k], edges[j]);
            k++;
        }
    }
    swap(edges[k], edges[end]);
    return k;
}

void quickSort(edge* edges[], int start, int end)
{
    if (start < end)
    {
        int q = partition(edges, start, end);
        quickSort(edges, start, q - 1);
        quickSort(edges, q, end);
    }
}

void kruskal(edge* edges[], node* v[], int sizeNodes, int sizeEdges, edge* tree[])
{
    int nrEdges = 0, indexMST = 0;

    for (int i = 0; i < sizeNodes; i++)
        makeSet(v[i]);

    quickSort(edges, 0, sizeEdges - 1);

    for (int i = 0; i < sizeEdges; i++)
        if (findSet(edges[i]->head) != findSet(edges[i]->tail))
        {
            tree[indexMST] = edges[i];
            indexMST++;

            unionSet(edges[i]->head, edges[i]->tail);
        }
}

void demoSet()
{
    node* v[11];
    for (int i = 0; i < 10; i++)
        v[i] = createNode(i);

    for (int i = 0; i < 10; i++)
        makeSet(v[i]);

    unionSet(v[0], v[1]);
    unionSet(v[0], v[3]);
    unionSet(v[2], v[5]);
    unionSet(v[3], v[7]);
    unionSet(v[6], v[9]);
    unionSet(v[2], v[6]);

    for (int i = 0; i < 10; i++)
        printf("%d with representative: %d (%d)\n", i, findSet(v[i])->key, findSet(v[i])->rank);
}

edge* createEdge(node* head, node* tail, int weight)
{
    edge* temp = new edge();
    temp->head = head;
    temp->tail = tail;
    temp->weight = weight;
    return temp;
}

void demoKruskal()
{
    edge* edges[15];
    int sizeEdges = sizeof(edges) / sizeof(edges[0]);
    node* nodes[10];
    int sizeNodes = sizeof(nodes) / sizeof(nodes[0]);

    for (int i = 0; i < 10; i++)
        nodes[i] = createNode(i);

    for (int i = 0; i < 11; i++)
        edges[i] = new edge();

    edges[0] = createEdge(nodes[0], nodes[6], 1);
    edges[1] = createEdge(nodes[0], nodes[1], 6);
    edges[2] = createEdge(nodes[0], nodes[7], 3);
    edges[3] = createEdge(nodes[1], nodes[2], 9);
    edges[4] = createEdge(nodes[2], nodes[7], 4);
    edges[5] = createEdge(nodes[2], nodes[3], 2);
    edges[6] = createEdge(nodes[3], nodes[8], 4);
    edges[7] = createEdge(nodes[3], nodes[5], 3);
    edges[8] = createEdge(nodes[8], nodes[9], 2);
    edges[9] = createEdge(nodes[9], nodes[4], 5);
    edges[10] = createEdge(nodes[4], nodes[5], 7);
    edges[11] = createEdge(nodes[3], nodes[4], 8);
    edges[12] = createEdge(nodes[1], nodes[9], 9);
    edges[13] = createEdge(nodes[0], nodes[6], 2);
    edges[14] = createEdge(nodes[5], nodes[4], 7);
    
    printf("Original Graph:\n");
    for (int i = 0; i < 15; i++)
        printf("(%d %d) -> %d\n", edges[i]->head->key, edges[i]->tail->key, edges[i]->weight);

    edge* tree[11];
    for (int i = 0; i < 11; i++)
        tree[i] = new edge();

    kruskal(edges, nodes, sizeNodes, sizeEdges, tree);

    printf("\nMST Tree:\n");
    for (int i = 0; tree[i]->weight > 0; i++)
    {
        printf("(%d %d) -> %d\n", tree[i]->head->key, tree[i]->tail->key, tree[i]->weight);
    }
}

int adjMatrix[MAX_SIZE][MAX_SIZE];

void genGraph(int n, node* nodes[], edge* edges[])
{

    int vertices[MAX_SIZE];
    FillRandomArray(vertices, n, 0, n - 1, true, 0);
    for (int i = 0; i < n - 1; i++)
    {
        edges[i] = createEdge(nodes[i], nodes[i + 1], rand());
        adjMatrix[i][i + 1]++;
    }

    int nr = n - 1;
    while (nr < 4 * n)
    {
        int h = rand() % n;
        int t = rand() % n;
        if (adjMatrix[h][t] == 0 && adjMatrix[t][h] == 0)
        {
            edges[nr] = createEdge(nodes[h], nodes[t], rand());
            adjMatrix[h][t]++;
            nr++;
        }
    }
}

void perf()
{
    node* nodes[MAX_SIZE];
    edge* tree[MAX_SIZE];
    edge* graph[MAX_SIZE * 4];

    for (int test = 0; test < TEST; test++)
        for (int n = STEP; n <= MAX_SIZE; n += STEP)
        {
            for (int nrNodes = 0; nrNodes < n; nrNodes++)
                nodes[nrNodes] = createNode(nrNodes);

            genGraph(n, nodes, graph);

            opFS = 0;
            opMS = 0;
            opU = 0;
            kruskal(graph, nodes, n, n * 4, tree);
            p.countOperation("Find Set", n, opFS);
            p.countOperation("Make Set", n, opMS);
            p.countOperation("Union", n, opU);
        }

    p.divideValues("Make Set", TEST);
    p.divideValues("Find Set", TEST);
    p.divideValues("Union", TEST);
    p.createGroup("Operatii", "Make Set", "Find Set", "Union");
    p.showReport();
} 

int main()
{
    demoSet();
    printf("\n");
    demoKruskal();
    //perf();
    return 0;
}