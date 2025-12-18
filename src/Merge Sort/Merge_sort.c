#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX 10000 


//notes
/*
threads are used in merge sort as divide and conquer way to do this so send ecah part on thread to sort it and then merge the results
merged by main thread 
parallelism is used 
*/
// int pthread_create(
//     pthread_t *thread,         
//     const pthread_attr_t *attr
//     void *(*start_routine)(void *)
//     void *arg                  
// );









// Struct for thread arguments
//start and end of subarray and pointer to the array
typedef struct {
    int left;
    int right;
    int *arr;
} ThreadArgs;

// Merge function (same as your original)
void merge(int arr[], int left, int mid, int right) {
    int i, j, k;
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));

    for (i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = R[j++];

    free(L);
    free(R);
}

void* threaded_merge_sort(void* arg) {

    // you have to Implement the full function to be using the merge function
    // and to call it in the main
    //each threads runs
    ThreadArgs *merge_arguments=(ThreadArgs *)arg;
    int left=merge_arguments->left;
    int right=merge_arguments->right;
    int *arr=merge_arguments->arr;

    if (left>=right){
        return NULL;
    }

    else{

        int middle=(left+right) / 2 ;
        //dividing into 2 parts 
        ThreadArgs left_arr={left,middle,arr};
        ThreadArgs right_arr={middle+1,right,arr};
        //creating 2 threads 
        pthread_t left_thread,right_thread;
        //dividing till one element and then merging will take place 
        pthread_create(&left_thread,NULL,threaded_merge_sort,&left_arr);
        pthread_create(&right_thread,NULL,threaded_merge_sort,&right_arr);

        //waiting for ecah to finish and then merge 
        pthread_join(left_thread,NULL);
        pthread_join(right_thread,NULL);

        merge(arr,left,middle,right);
        return NULL;

    }


}

int main() {
    FILE* file = fopen("input", "r");
    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }

    int n;
    fscanf(file, "%d", &n);
    int arr[MAX];
    for (int i = 0; i < n; i++) {
        fscanf(file, "%d", &arr[i]); 
    }
    fclose(file);

    // Make the required changes inorder to be able to call the Thread fn
    /*You code*/

    ThreadArgs main_args = {0, n - 1, arr};
    threaded_merge_sort(&main_args);
     


    printf("Sorted array:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    return 0;
}
