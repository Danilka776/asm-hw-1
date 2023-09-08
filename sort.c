#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <math.h>



int exch = 0; // кол-во обменов
int comp = 0; // кол-во сравнений

int testing(double *a, double *b, int n) { //функция сравнения эквивалентности множеств элементов находящихся в двух массивах.
    int *buf = (int *)calloc(n, sizeof(int));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (buf[j] == 0 && a[i] == b[j]) {
                buf[j] = 1;
                break;
            }
        }
    }
    for (int i = 0; i < n; ++i) {
        if (buf[i] == 0) {
            free(buf);
            return 0;
        }
    }
    free(buf);
    return 1;
}

int is_sort(double *a, int n) {  // проверка неубывания элементов массива
    for (int i = 0; i < n - 1 ; ++i) {
        if (a[i] > a[i+1]) {
            return 0;
        }
    }
    return 1;
}

void swap(double *a, double *b) { //обмен элементов и счетчик обменов
    double temp = *a;
    *a = *b;
    *b = temp;

    exch++;
}

double random_double(void) {   //генерация случайного элемента типа double
    double *c = NULL;
    long long a;
    do {
        a = (long long)rand() * rand() * rand() * rand() * rand();
        long long *b = &a;
        c = (double*)b;
    } while (!isfinite(*c)); //проверка на inf и nan

    return *c;
}

char compare(double *a, double *b) {  // счетчик сравнений
    comp++;
    if (*a >= *b) {
        return 1;
    }
    else {
        return 0;
    }
}

int comp_order(const void *a, const void *b) { // функция сравнения для упорядочивания массива по неубыванию
    double x = *(double *) a;
    double y = *(double *) b;

    if (x > y) {
        return 1;
    }
    else if (x < y) {
        return -1;
    }
    else {
        return 0;
    }
}

int comp_reverse(const void *a, const void *b) { // функция сравнения для упорядочивания массива по невозрастанию
    double x = *(double *) a;
    double y = *(double *) b;

    if (x > y) {
        return -1;
    }
    else if (x < y) {
        return 1;
    }
    else {
        return 0;
    }
}

double *generate_arr(int n, int type) {
    // type - параметр: type = 1 - неубывающий, type = 2 - невозрастающий, type = 3,4 - произвольный массивы
    double *arr = (double *) malloc(sizeof(double) * n);

    for (int i = 0 ; i < n; i++) {  // генерация случайного массива
        arr[i] = random_double();
    }

    if (type == 1) {
        qsort(arr, n, sizeof(double), comp_order);  // сортировка по неубыванию
    }

    if (type == 2) {
        qsort(arr, n, sizeof(double), comp_reverse); // сортировка по невозрастанию
    }

    return arr;
}

double *duplicate_arr(double *a, int n) {    // дублирование сгенерированого массива а для сравнения сортировок на одних данных
    double *new_arr = (double *)malloc(sizeof(double) * n);

    for (int i = 0; i < n; i++) {
        new_arr[i] = a[i];
    }

    return new_arr;
}


void select_sort(double *a, int n) {

    for (int i = 0; i < n - 1; i++) {
        int cur = i;     // индекс текущего элемента (минимальный)

        for (int j = cur + 1; j < n; j++) { // поиск нового минимального элемента(если есть)
            if (!compare(&a[j], &a[cur])) {
                cur = j;    // индекс нового минимального элемента
            }
        }

        if (cur != i) {
            swap(&a[i], &a[cur]); // перестановка текущего элемента с минимальным
        }
    }

    printf("COMP: %d\n", comp);
    printf("SWAP: %d\n", exch);
}


void sift_down(double *arr, int ind, int n) {  // просеивание элемента  // ind - индикатор конца пирамиды
    while (n > 2 * ind + 1) {
        int l = 2 * ind + 1, r = 2 * ind + 2;  // левый и правый сыновья элемента на позиции ind

        int max_son = l;
        if (r < n && !compare(&arr[l], &arr[r])) {  // поиск наибольшего сына
            max_son = r;
        }

        if (compare(&arr[ind], &arr[max_son])) {  // усливия пирамиды выполняются
            break;
        }

        swap(&(arr[ind]), &(arr[max_son]));   // смена текущего и наибольшего сына
        ind = max_son;
    }
}

void heap_sort(double *arr, int n) {
    for (int i = n - 1; i >= 0; i--) {  // построение пирамиды (сверху элементы с большими значениями)
        sift_down(arr, i, n);           // элементы с маленькими значениями "присеиваются" вниз
    }

    while (n > 1) {
        swap(&arr[0], &arr[--n]);  // перемещение элеента из вершины(там наибольшее значение) в конец массива
        sift_down(arr, 0, n);    // просеивание перемещенного эл-та в вершину, для востановления условий пирамиды
    }

    printf("COMP: %d\n", comp);
    printf("SWAP: %d\n", exch);
}




int main(void) {
    srand(time(NULL));
    int size[4] = {10, 100, 1000, 10000};

    for (int i = 0; i < 4; i++) {
        for (int type = 1; type <= 4; type++) {
            int n = size[i];
            double *arr = malloc(sizeof(double) * n);
            double *mas = malloc(sizeof(double) * n);
            double *test = malloc(sizeof(double) * n);

            mas = generate_arr(n, type);
            arr = duplicate_arr(mas, n);
            test = duplicate_arr(mas, n);


            printf("type = %d | n = %d\n", type, n);

            printf("Select\n");
            select_sort(mas, n);
            if (testing(test, mas, n)) {
                printf("OK\n");
            }
            else {
                printf("ERROR\n");
            }
            if (is_sort(mas, n)) {
                printf("mas is sorting \n");
            }
            else {
                printf("ERROR\n");
            }
            exch = 0;
            comp = 0;

            printf("Heap\n");
            heap_sort(arr, n);
            if (testing(test, arr, n)) {
                printf("OK\n");
            }
            else {
                printf("ERROR\n");
            }
            if (is_sort(mas, n)) {
                printf("mas is sorting \n");
            }
            else {
                printf("ERROR\n");
            }
            exch = 0;
            comp = 0;

            free(mas);
            free(arr);
            free(test);
            printf("\n");
        }
    }
    return 0;
}
