/**     Pentru acest laborator am avut de implementat transformari intre cele 3 reprezentari ale arborilor multicai:
 *          r1: vector de parinti (parent)
 *          r2: arbore multicai   (nodeM)
 *          r3: arbore binar      (nodeB)
 *
 *      Pentru prima transformare (r1 -> r2), am folosit in plus un vector de liste (structura nodeL), care sa reprezinte
 *  copiii nodului cu cheia indexului (ex. pt index 7 avem nodurile 2 -> 5 -> 6), si un vector pentru a verifica daca se introduce
 *  in lista primul element (firstElement).
 *      Initial am creat vectorul de liste in functie de r1, iar apoi am apelat functia buildMWT pentru a construi arborele
 *  multicai.
 *      Pentru a doua transformare, intai am creat prima ramura a arborelui, inserand primul copil al fiecarui nod. Cand am ajuns
 *  la un nod fara copii, am inceput sa apelez recursiv functia pentru fratii nodului respectiv. Aceasta functie nu foloseste memorie
 *  aditionala.
 *      Ambele functii tind spre O(n) in timp, iar din punct de vedere al memoriei, prima transformare are complexitate O(n) iar cea
 *  de-a doua O(1).
 */

#include <iostream>

using namespace std;

#pragma warning(disable: 4996);

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define MAX_NODES 10

struct nodeM {
    int key;
    nodeM* children[MAX_NODES];
};

struct nodeL {
    int key;
    nodeL* next;
};

struct nodeB{
    int key;
    nodeB* child;
    nodeB* sibling;
};

nodeL* createNode(int key)
{
    nodeL* inserted = new nodeL();
    inserted->key = key;
    inserted->next = nullptr;
    return inserted;
}

void addNode(nodeL** head, int data)
{
    nodeL* temp = createNode(data);
    nodeL* last = *head;

    if (*head == nullptr)
    {
        *head = temp;
    }
    else
    {
        while (last->next != nullptr)
            last = last->next;
        last->next = temp;
    }
}

void emptyList(nodeL* list[], int n)
{
    for (int i = 0; i < n; i++)
        list[i] = nullptr;
}

nodeM* createTreeNode(int key)
{
    nodeM* temp = new nodeM();
    temp->key = key;
    for (int i = 0; i < MAX_NODES; i++)
        temp->children[i] = nullptr;
    return temp;
}

nodeB* createBNode(int key)
{
    nodeB* temp = new nodeB();
    temp->key = key;
    temp->child = nullptr;
    temp->sibling = nullptr;
    return temp;
}

nodeM* buildMWT(nodeL* list[], int root, nodeM* tree)
{
    tree = createTreeNode(root);
    int index = 0;
    nodeL* head = list[root];

    while (head != nullptr)
    {
        tree->children[index] = buildMWT(list, head->key, tree->children[index]);
        index++;
        head = head->next;
    }

    return tree;
}

int first[MAX_NODES];

nodeM* transformation1(int parent[], int n, nodeM* tree)
{
    nodeL* list[MAX_NODES];
    int root = -1;

    emptyList(list, n);

    for (int i = 0; i < n; i++)
    {
        if (first[parent[i]] == 0)
        {
            list[parent[i]] = createNode(i);
            first[parent[i]]++;
        }
        else
            if(parent[i] != -1)
                 addNode(&list[parent[i]], i);

        if (parent[i] == -1)
            root = i;
    }

    tree = buildMWT(list, root, tree);
    return tree;
}

void printMWT(nodeM* tree, int spaces)
{
    int index = 0;
    nodeM* head = tree;

    for (int i = 0; i < 2 * spaces; i++)
        printf(" ");
    printf("%d\n", head->key);

    while (head->children[index] != nullptr)
    {
        printMWT(head->children[index], spaces + 1);
        index++;
    }
}

nodeB* transformation2(nodeM* tree)
{
    nodeM* head = tree;
    int index = 1;

    nodeB* bTree = createBNode(head->key);
    if(head->children[0] != nullptr)
        bTree->child = transformation2(head->children[0]);

    nodeB* child = bTree->child;
    while(head->children[index] != nullptr)
    {
        for(int i = 0; i < index - 1; i++)
            child = child->sibling;
        child->sibling = transformation2(head->children[index]);
        index++;
    }
    return bTree;
}

void prettyPrint(nodeB* tree, int spaces)
{
    nodeB* head = tree;
    for (int i = 0; i < 2 * spaces; i++)
        printf(" ");
    printf("%d\n", head->key);

    if(head->child != nullptr)
        prettyPrint(head->child, spaces+1);
    if(head->sibling != nullptr)
        prettyPrint(head->sibling, spaces);
}


int main()
{
    int parent[] = { 6, 2, 7, 5, 2, 7, 7, -1, 5, 2 };
    int n = sizeof(parent) / sizeof(parent[0]);
    nodeM* tree = new nodeM();

    printf("R1: \n");
    for(int i = 0; i < n; i++)
        printf("%d ", i);
    printf("\n");
    for(int i = 0; i < n; i++)
        printf("%d ", parent[i]);

    printf("\n\nR2:\n");

    tree = transformation1(parent, n, tree);
    printMWT(tree, 0);

    printf("\nR3:\n");

    nodeB* btree = transformation2(tree);

    prettyPrint(btree, 0);

    return 0;
}