#include <stdio.h>
#include <stdlib.h>

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

int swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}
int main()
{
    int n = 10;
    int x[] = {9, 2, 5, 1, 8, 4, 6, 0, 7, 3};
    int to_sort[8][n];
    char str[][256] = {"--- ソート前の配列 ---", "--- バブルソート(通常版)結果 ---", "--- バブルソート(改良版)結果 ---", "--- 選択ソート結果 ---", "--- 挿入ソート結果 ---", "--- マージソート結果 ---", "--- クイックソート結果 ---", "--- ヒープソート結果 ---"};
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < n; j++)
        {
            to_sort[i][j] = x[j];
        }
    }
    bubbleSort_basic(to_sort[1], n);
    bubbleSort_optmized(to_sort[2], n);
    selectionSort(to_sort[3], n);
    insertionSort(to_sort[4], n);
    mergeSort(to_sort[5], 0, n - 1);
    quickSort(to_sort[6], 0, n - 1);
    heapSort(to_sort[7], n);
    for (int i = 0; i < 8; i++)
    {
        printf("%s\n", str[i]);
        for (int j = 0; j < 10; j++)
        {
            printf(" %d", to_sort[i][j]);
        }
        printf("\n\n");
    }
}

void bubbleSort_basic(int A[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for (int j = 0; j < n - i - 1; j++)
        {
            if (A[j] > A[j + 1])
            {
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
            if (A[j] > A[j + 1])
            {
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
            if (A[idx] < A[j])
                idx = j;
        }
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
            A[idx + 1] = A[idx];
            idx--;
        }
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
        array_left[k] = A[k + left];
    }
    for (int k = 0; k < right - mid; k++)
    {
        array_right[k] = A[k + mid + 1];
    }
    while (i <= mid - left && j < right - mid)
    {
        if (array_left[i] <= array_right[j])
        {
            A[i + j + left] = array_left[i];
            i++;
        }
        else
        {
            A[i + j + left] = array_right[j];
            j++;
        }
    }
    while (i <= mid - left)
    {
        A[i + j + left] = array_left[i];
        i++;
    }
    while (j < right - mid)
    {
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
    swap(&A[div_idx], &A[right]);
    while (i <= j)
    {
        while (A[i] < A[right])
        {
            i++;
        }
        while (A[j] >= A[right] && (j >= i))
        {
            j--;
        }

        if (i < j)
        {
            swap(&A[i], &A[j]);
        }
    }
    swap(&A[i], &A[right]);
    return i;
}

void heapSort(int A[], int n)
{
    build_heap(A, n);

    for (int i = n - 1; i > 0; i--)
    {
        swap(&A[0], &A[i]);
        heapify(A, i, 0);
    }
}

void heapify(int A[], int n, int i)
{
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    if (left < n && A[left] > A[largest])
    {
        largest = left;
    }
    if (right < n && A[right] > A[largest])
    {
        largest = right;
    }
    if (largest != i)
    {
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