#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>


const size_t size = 20;
void quickSort(int num[], size_t start, size_t end);
void swapArray(int num[], size_t a, size_t b);
void generateRandomArray(int numbers[]);
int leonardo(int n);
void smooth_sort(std::vector<int>& arr);
void heapify(std::vector<int>& arr, int start, int end);


int main(){
    int numbers[size];
    srand(time(nullptr));
    generateRandomArray(numbers);
    int numbers1[size];

    for (size_t i = 0; i < size; i++) {
        numbers1[i] = numbers[i];
    }

    std::vector<int> numbersVector(size);
    for (size_t i = 0; i < size; i++) {
        numbersVector[i] = numbers[i];
    }

    printf("Быстрая сортировка\n");
    size_t start_time = clock();
    quickSort(numbers, 0, size - 1);
    size_t end_time = clock();
    size_t time = end_time - start_time;
    for (int element: numbers) {
        printf("%d ", element);
    }
    printf("\n");
    std::cout << "время " << time << "\n";

    //сортировка расчёской
    size_t combsortSize = size;
    start_time = clock();

    while (combsortSize > 1) {
        combsortSize = combsortSize / 1.247;
        int j = 0;
        for (int i = combsortSize; i < size; i++) {
            if (numbers1[i] < numbers1[j]) {
                swapArray(numbers1, i, j);
            }
            j++;
        }
    }

    end_time = clock();
    time = end_time - start_time;
    printf("Сортировка расчёской\n");

    for (int i = 0; i < size; i++) {
        printf("%d ", numbers[i]);
    }

    std::cout << "\nвремя " << time << "\n";
    // Плавная сортировка
    start_time = clock();
    smooth_sort(numbersVector);
    end_time = clock();
    time = end_time - start_time;
    printf("Плавная сортировка\n");

    for (int i = 0; i < size; i++) {
        printf("%d ", numbersVector[i]);
    }

    std::cout << "\nвремя " << time << "\n";
    return 0;
}

void swapArray(int num[], size_t a, size_t b)
{
    int temp = num[a];
    num[a] = num[b];
    num[b] = temp;
}

void quickSort(int num[], size_t start, size_t end) {
    if (start >= end)
        return;
    size_t current = start + 1;
    for (size_t i = start + 1; i <= end; i++) {
        if (num[i] < num[start]) {
            swapArray(num, i, current);
            current++;
        }
    }
    swapArray(num, start, current - 1);
    quickSort(num, start, current - 1);
    quickSort(num, current, end);
}

void generateRandomArray(int numbers[]) {
    for (size_t i = 0; i < size; i++) {
        numbers[i] = rand();
    }
    printf("unsorted\n");
    for (int i = 0; i < size; i++) {
        printf("%d ", numbers[i]);
    }
}

int leonardo(int n) {
    if (n < 2) {
        return 1;
    }
    return leonardo(n - 1) + leonardo(n - 2) + 1;
}


void heapify(std::vector<int>& arr, int start, int end)
{
    int i = start;
    int j = 0;
    int k = 0;

    while (k < end - start + 1) {
        if (k & 0xAAAAAAAA) {
            j = j + i;
            i = i >> 1;
        }
        else {
            i = i + j;
            j = j >> 1;
        }

        k = k + 1;
    }

    while (i > 0) {
        j = j >> 1;
        k = i + j;
        while (k < end) {
            if (arr[k] > arr[k - i]) {
                break;
            }
            std::swap(arr[k], arr[k - i]);
            k = k + i;
        }

        i = j;
    }
}

void smooth_sort(std::vector<int>& arr)
{
    int n = arr.size();

    int p = n - 1;
    int q = p;
    int r = 0;

    //Создаём кучу леонардо
    while (p > 0) {
        if ((r & 0x03) == 0) {
            heapify(arr, r, q);
        }

        if (leonardo(r) == p) {
            r = r + 1;
        }
        else {
            r = r - 1;
            q = q - leonardo(r);
            heapify(arr, r, q);
            q = r - 1;
            r = r + 1;
        }

        std::swap(arr[0], arr[p]);
        p = p - 1;
    }

    // Преобразуем кучу Леонардо обратно в массив
    for (int i = 0; i < n - 1; i++) {
        int j = i + 1;
        while (j > 0 && arr[j] < arr[j - 1]) {
            std::swap(arr[j], arr[j - 1]);
            j = j - 1;
        }
    }
}
