#include <stdio.h>

int x2(int n) {
	return 2*n;
}

int x10(int n) {
	return 10*n;
}

void mutate_map(int arr[], int size, int(fp)(int)) {
	for(int i = 0; i < size; i++) {
		arr[i] = fp(arr[i]);
	}
}

void print_array(int arr[], int size) {
	for(int i = 0; i < size; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

int main(void) {
	int arr[] = {3, 1, 4}, size = 3;
	print_array(arr, size);
	mutate_map(arr, size, x2);
	print_array(arr, size);
	mutate_map(arr, size, x10);
	print_array(arr, size);
}
