/* Pentru acest laborator am avut de implementat operatiile de inserare si de cautare pentru Hash Tables.
 *
 *  Functia de inserarea calculeaza noua pozitie a datelor de inserare pana gaseste un loc neocupat; daca nu se verifica
 * toate celulele si celula gasita este neocupata, atunci se va insera elementul.
 *
 *  Functia de cautare, in aceeasi maniera ca inserarea, calculeaza repetitiv pozitia posibila a datelor, pana cand:
 *      -se verifica toate celulele
 *      -elementul a fost gasit
 *      -celula ce este verifica este neocupata
 *
 *  Pentru evaluarea functiei de cautare, in functie de un factor de umplere (80%, 85%, 90%, 95%, 99%), am cautat cate
 * 1500 de elemente de care stim sigur ca au fost inserate in tabel si 1500 de elemente ce nu puteau fi gasite
 *
 *  Din tabel, se observa ca o valoare mai mare a factorului de umplere cauzeaza un numar mai mare de celule accesate,
 * in special in cazul elementelor negasite.
 *  In cazul elementelor gasite, valoarea medie ramane aproximativ constanta, in jurul valoarei intregi 2. */

#include <iostream>
#include <string.h>
#include "Profiler.h"
using namespace std;

Profiler p("lab 5");

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#define N 9973

typedef struct {
    int id;
    char name[30];
}Entry;

float op;
int toFind[1500];

int hashF(Entry data, int i)
{
    int result = (199 * data.id * i * i + 607 * i + 919) % 9973;
    return result;
}

void emptyHash(Entry table[], int n)
{
    for (int i = 0; i < n; i++)
    {
        table[i].id = 0;
        strcpy(table[i].name, " ");
    }
}

void insert(Entry table[], Entry inserted, int n)
{
    int i = 0;
    int position = hashF(inserted, i);
    while (table[position].id != 0 && strcmp(table[position].name, " ") != 0 && i < n)
    {
        i++;
        position = hashF(inserted, i);
    }

    if (i < n)
        if (table[position].id == 0 && strcmp(table[position].name, " ") == 0)
            table[position] = inserted;
}

int search(Entry table[], Entry data, int n)
{
    int found = 0, i = 0, position;
    op++;
    position = hashF(data, i);

    while (found == 0 && i < n && table[position].id != 0 && strcmp(table[position].name, " ") != 0)
    {
        if (table[position].id == data.id && strcmp(table[position].name, data.name) == 0)
        {
            found = 1;
        }
        else
        {
            i++;
            position = hashF(data, i);
            op++;
        }
    }

    if (found == 1)
        return position;
    if (found == 0)
        return -1;
}

void searchDemo(Entry table[], Entry data, int n)
{
    int pos = search(table, data, n);
    if (pos >= 0)
    {
        printf("Name: %s and ID: %d found at index %d\n", data.name, data.id, pos);
        printf("\n");
    }
    else
    {
        printf("Name: %s and ID: %d not found\n", data.name, data.id);
        printf("\n");
    }
}

void demo()
{
    Entry table[N];
    emptyHash(table, N);

    Entry data1;
    data1.id = 2;
    strcpy(data1.name, "Name1");

    Entry data2;
    data2.id = 2;
    strcpy(data2.name, "Name2");

    Entry data3;
    data3.id = 5;
    strcpy(data3.name, "Name3");

    Entry data4;
    data4.id = 134141;
    strcpy(data4.name, "Name4");

    Entry data5;
    data5.id = 21341;
    strcpy(data5.name, "Name5");

    insert(table, data1, N);
    insert(table, data2, N);
    insert(table, data3, N);
    insert(table, data5, N);

    searchDemo(table, data1, N);
    searchDemo(table, data2, N);
    searchDemo(table, data3, N);
    searchDemo(table, data4, N);
    searchDemo(table, data5, N);
}

float perfTable[5][5];

void perf()
{
    float alfa[] = { 0.8, 0.85, 0.9, 0.95, 0.99 };
    Entry table[N];
    Entry data;
    int v[9773];
    float maxOp, totalOp;

    for (int fillFactor = 0; fillFactor < 5; fillFactor++)
    {
        perfTable[fillFactor][0] = alfa[fillFactor];

        int n = alfa[fillFactor] * N + 1;

        for (int test = 0; test < 5; test++)
        {
            emptyHash(table, N);
            int j = 0;

            FillRandomArray(v, n, 1, 10000, true, 0);

            for (int nrElements = 0; nrElements < n; nrElements++)
            {
                data.id = v[nrElements];
                strcpy(data.name, "Nume");

                if (nrElements % 5 == 0 && j < 1500)
                {
                    toFind[j] = data.id;
                    j++;
                }

                insert(table, data, N);
            }

            totalOp = 0;
            maxOp = 0;
            for (int foundElements = 0; foundElements < 1500; foundElements++)
            {
                data.id = toFind[foundElements];
                strcpy(data.name, "Nume");

                op = 0;
                int aux = search(table, data, N);
                if (maxOp < op)
                    maxOp = op;
                totalOp += op;
            }

            perfTable[fillFactor][2] += maxOp;
            perfTable[fillFactor][1] += (totalOp / 1500);

            totalOp = 0;
            maxOp = 0;
            for (int notFound = 0; notFound < 1500; notFound++)
            {
                data.id = v[notFound * 3];
                strcpy(data.name, "Negasit");

                op = 0;
                int aux = search(table, data, N);
                if (maxOp < op)
                    maxOp = op;
                totalOp += op;
            }

            perfTable[fillFactor][4] += maxOp;
            perfTable[fillFactor][3] += (totalOp / 1500);
        }

        for (int column = 1; column < 5; column++)
        {
            perfTable[fillFactor][column] /= 5;
        }

    }

    printf("Fill AvgFound MaxFound AvgNotF MaxNotF\n");
    for (int row = 0; row < 5; row++)
    {
        for (int column = 0; column < 5; column++)
        {
            if(column == 2 || column == 4)
                printf("%d      ", (int)perfTable[row][column]);
            else
                if(column == 0)
                    printf("%.2f    ", perfTable[row][column]);
                else
                    printf("%.3f    ", perfTable[row][column]);
        }
        printf("\n");
    }

}

int main()
{
    //demo();
    perf();
    return 0;
}