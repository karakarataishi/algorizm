#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bubbleSort_basic(int A[], int n);        // バブルソート（打切りなし）
void bubbleSort_optmized(int A[], int n);     // バブルソート（打切りあり）
void selectionSort(int A[], int n);           // 選択ソート
void insertionSort(int A[], int n);           // 挿入ソート
void mergeSort(int A[], int left, int right); // マージソート
void quickSort(int A[], int left, int right); // クイックソート
void heapSort(int A[], int n);                // ヒープソート

void merge(int A[], int left, int mid, int right);
int partition(int A[], int left, int right);
void heapify(int A[], int n, int i);
void build_heap(int A[], int n);

void implement(int A[], int opt, int now_size, int reset);
int power(int a, int n);
int swap(int *a, int *b);
void fisher_yates_shuffle(int *A, int n);

// 　比較回数、交換・代入回数をグローバル変数で管理
long long cmp_cnt = 0;
long long swap_cnt = 0;
// 追加
int main()
{
    FILE *fp = fopen("output.csv", "w");
    int N = 10000; // 要素数
    int WAY = 7;   // ソートの種類数
    int OPT = 5;   // 配列の初期化条件(昇順、降順、ランダム（シード値0,1,2）)
    int SIZE = 4;  // 配列のサイズの種類
    fprintf(fp, "アルゴリズム,並び方,データ数,比較回数,交換・代入回数\n");
    char str_sort[][256] = {"バブルソート（打ち切りなし）", "バブルソート（打ち切りあり）", "選択ソート", "挿入ソート", "マージソート", "クイックソート", "ヒープソート"};
    char str_opt[][32] = {"昇順", "降順", "ランダム"};
    int SWAP[WAY][OPT - 2][SIZE];
    int CMP[WAY][OPT - 2][SIZE];
    // 　ソートの種類、配列の個数の区別、配列の初期化方法、配列の要素という四次元配列を作成
    int ****array = calloc(sizeof(int ***), WAY);
    for (int i = 0; i < WAY; i++)
    {
        array[i] = calloc(sizeof(int **), SIZE);
        for (int j = 0; j < SIZE; j++)
        {
            array[i][j] = calloc(sizeof(int *), OPT);
            for (int k = 0; k < OPT; k++)
            {
                array[i][j][k] = calloc(sizeof(int), N);
            }
        }
    }
    for (int i = 0; i < WAY; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            // j = 0 ← 10個の要素を格納
            // j = 1 ← 100個の要素を格納 ...
            for (int k = 0; k < OPT; k++)
            {
                // k = 0 昇順
                // k = 1 降順
                // k = 2 ランダム(シード値　0)
                // k = 3 ランダム(シード値　1)
                // k = 4 ランダム(シード値　2)
                int now_size = power(10, j + 1);
                for (int l = 0; l < now_size; l++)
                {
                    if (k != 1)
                    {
                        array[i][j][k][l] = l + 1;
                    }
                    else
                    {
                        array[i][j][k][l] = now_size - l;
                    }
                }
                if (k >= 2)
                {
                    srand(k - 2);
                    fisher_yates_shuffle(array[i][j][k], now_size);
                }
                // ソートを実行
                // printf("8329");
                implement(array[i][j][k], i, now_size, k);
                if (k < 2)
                {
                    SWAP[i][k][j] = swap_cnt;
                    CMP[i][k][j] = cmp_cnt;
                }
                else if (k == 4)
                {
                    SWAP[i][k - 2][j] = swap_cnt / 3;
                    CMP[i][k - 2][j] = cmp_cnt / 3;
                }
            }
        }
    }
    for (int i = 0; i < WAY; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            for (int k = 0; k < SIZE; k++)
            {
                int now_size = power(10, k + 1);
                fprintf(fp, "%s,%s,%d,%d,%d\n", str_sort[i], str_opt[j], now_size, CMP[i][j][k], SWAP[i][j][k]);
            }
        }
    }
    for (int i = 0; i < WAY; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            for (int k = 0; k < OPT; k++)
            {
                free(array[i][j][k]);
            }
            free(array[i][j]);
        }
        free(array[i]);
    }
    free(array);
    fclose(fp);
}

void bubbleSort_basic(int A[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            cmp_cnt++;
            if (A[j] > A[j + 1])
            {
                swap_cnt += 3;
                swap(&A[j], &A[j + 1]);
            }
        }
    }
}
void bubbleSort_optmized(int A[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        int cnt = 0;
        for (int j = 0; j < n - i - 1; j++)
        {
            cmp_cnt++;
            if (A[j] > A[j + 1])
            {
                swap_cnt += 3;
                cnt++;
                swap(&A[j], &A[j + 1]);
            }
        }
        if (!cnt)
        {
            return;
        }
    }
}
void selectionSort(int A[], int n)
{
    for (int i = n - 1; i >= 1; i--)
    {
        int idx = 0;
        for (int j = 0; j <= i; j++)
        {
            cmp_cnt++;
            if (A[idx] < A[j])
                idx = j;
        }
        swap_cnt += 3;
        swap(&A[idx], &A[i]);
    }
}

void insertionSort(int A[], int n)
{
    for (int i = 1; i < n; i++)
    {
        int idx = i - 1;
        int num = A[i];
        while (idx >= 0 && A[idx] > num)
        {
            cmp_cnt++;
            swap_cnt++;
            A[idx + 1] = A[idx];
            idx--;
        }
        swap_cnt++;
        if (idx != -1)
            cmp_cnt++;
        A[idx + 1] = num;
    }
}

void mergeSort(int A[], int left, int right)
{
    if (left >= right)
        return;
    int mid = (left + right) / 2;

    // 左半分をマージソート
    mergeSort(A, left, mid);

    // 右半分をマージソート
    mergeSort(A, mid + 1, right);

    merge(A, left, mid, right);
}

void merge(int A[], int left, int mid, int right)
{
    int array_left[mid - left + 1];
    int array_right[right - mid];
    int i = 0;
    int j = 0;
    for (int k = 0; k <= mid - left; k++)
    {
        swap_cnt++;
        array_left[k] = A[k + left];
    }
    for (int k = 0; k < right - mid; k++)
    {
        swap_cnt++;
        array_right[k] = A[k + mid + 1];
    }
    while (i <= mid - left && j < right - mid)
    {
        cmp_cnt++;
        if (array_left[i] <= array_right[j])
        {
            swap_cnt++;
            A[i + j + left] = array_left[i];
            i++;
        }
        else
        {
            swap_cnt++;
            A[i + j + left] = array_right[j];
            j++;
        }
    }
    while (i <= mid - left)
    {
        swap_cnt++;
        A[i + j + left] = array_left[i];
        i++;
    }
    while (j < right - mid)
    {
        swap_cnt++;
        A[i + j + left] = array_right[j];
        j++;
    }
}

void quickSort(int A[], int left, int right)
{
    if (left < right)
    {
        int div_idx = partition(A, left, right);

        quickSort(A, left, div_idx - 1);
        quickSort(A, div_idx + 1, right);
    }
}

int partition(int A[], int left, int right)
{
    int div_idx = (left + right) / 2;
    int i = left;
    int j = right - 1;
    swap_cnt += 3;
    swap(&A[div_idx], &A[right]);
    while (i <= j)
    {
        cmp_cnt++;
        while (A[i] < A[right])
        {
            cmp_cnt++;
            i++;
        }
        cmp_cnt++;
        while (A[j] >= A[right] && (j >= i))
        {
            cmp_cnt++;
            j--;
        }

        if (i < j)
        {
            swap_cnt += 3;
            swap(&A[i], &A[j]);
        }
    }
    swap_cnt += 3;
    swap(&A[i], &A[right]);
    return i;
}

void heapSort(int A[], int n)
{
    build_heap(A, n);

    for (int i = n - 1; i > 0; i--)
    {
        swap_cnt += 3;
        swap(&A[0], &A[i]);
        heapify(A, i, 0);
    }
}

void heapify(int A[], int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    cmp_cnt++;
    if (left < n && A[left] > A[largest])
    {
        largest = left;
    }
    cmp_cnt++;
    if (right < n && A[right] > A[largest])
    {
        largest = right;
    }
    if (largest != i)
    {
        swap_cnt += 3;
        swap(&A[largest], &A[i]);
        heapify(A, n, largest);
    }
}

void build_heap(int A[], int n)
{
    for (int i = n / 2 - 1; i >= 0; i--)
    {
        heapify(A, n, i);
    }
}

void implement(int A[], int opt, int now_size, int reset)
{
    // 比較回数、交換・代入回数を初期化
    if (reset != 3 && reset != 4)
    {
        cmp_cnt = 0;
        swap_cnt = 0;
    }
    if (opt == 0)
    {
        bubbleSort_basic(A, now_size);
    }
    if (opt == 1)
    {
        bubbleSort_optmized(A, now_size);
    }
    if (opt == 2)
    {
        selectionSort(A, now_size);
    }
    if (opt == 3)
    {
        insertionSort(A, now_size);
    }
    if (opt == 4)
    {
        mergeSort(A, 0, now_size - 1);
    }
    if (opt == 5)
    {
        quickSort(A, 0, now_size - 1);
    }
    if (opt == 6)
    {
        heapSort(A, now_size);
    }
}

int power(int a, int n)
{
    int num = 1;
    while (n--)
    {
        num *= a;
    }
    return num;
}
int swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
void fisher_yates_shuffle(int *A, int n)
{
    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        int tmp = A[i];
        A[i] = A[j];
        A[j] = tmp;
    }
}