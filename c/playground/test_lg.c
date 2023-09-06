//Read ip from user store in array in sorted order
// 5 int from users and strore in array

// read 2nd aaray and sort it like first 

//merge the 2 arrays and print the outputs

#include <stdio.h>

#define ARR_SIZE1   5
#define ARR_SIZE2   5

#if 0
void print_array(int arr[], int len)
{
    for(int i = 0; i < len; i++)
    {
        printf("%d \t", arr[i]);
    }
    printf("\n");
}

void sort_array(int arr[], int len)
{
    int i,j;

    for(i = 0; i < len ; i++)
    {
        int temp;

        for(j = 0; j < len - i -1; j++)
        {

            if(arr[j] > arr[j+1])
            {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
            else
            {
                // Nothing
            }
        }

    }
}

void join_array(int arr1[], int len1, int arr2[], int len2, int arr3[], int len3)
{
    for (int i = 0; i < len3; i++)
    {
        /* code */
        if( i < len1)
        {
            arr3[i] = arr1[i];
        }
        else
        {
            arr3[i] = arr2[ i - len2];
        }
    }
    
}

int main()
{
    // Test case 1
    int arr1[ARR_SIZE1];


    printf("Enter %d elements", ARR_SIZE1);
    for(int i = 0; i < ARR_SIZE1; i++)
    {
        scanf("%d",&arr1[i]);

        //Dynamic coding wala part
        // if (i == 0)
        // {
        //     temp = arr1[i];
        //     arr3[i] = arr1[i];
        // }
        // else if( i < ARR_SIZE1)
        // {
        //     // Sort Fill arr3 with arr1
        //     if(arr1[i] < arr1[i-1])
        //     {
        //         //Swap
        //         temp = arr1[i-1];
        //         arr1[i-1] = arr1[i];
        //         arr1[i] = temp;

        //         //Do the same with arr3
        //         temp = arr3[i-1];
        //         arr3[i-1] = arr3[i];
        //         arr3[i] = temp;
        //     }
        // }
        // else
        // {
        //     //Seg fault
        // }
    }
    

    //Sort the array
    sort_array(arr1,(sizeof(arr1)/sizeof(arr1[0])));
    print_array(arr1,(sizeof(arr1)/sizeof(arr1[0])));


    //Test case 2
    int arr2[ARR_SIZE2];
    printf("Enter %d elements", ARR_SIZE2);
    for(int i = 0; i < ARR_SIZE2; i++)
    {
        scanf("%d",&arr2[i]);
    }

    //Sort the array
    sort_array(arr2,(sizeof(arr2)/sizeof(arr2[0])));
    print_array(arr2,(sizeof(arr2)/sizeof(arr2[0])));

    //Test case 3
    //Join the 2 arrays
    int new_len = ARR_SIZE1 +ARR_SIZE2;
    int arr3[new_len];

    join_array(arr1,ARR_SIZE1,arr2, ARR_SIZE2, arr3, new_len);
    print_array(arr3, (sizeof(arr3)/sizeof(arr3[0])));

    return 0;
}
#endif


//Dynamic Programming 
#if 1
void swap_code(int arr[], int i, int j)
{
    // j > i
    int temp = arr[j];
    arr[j] = arr[i];
    arr[i] = temp;
}
void print_array(int arr[], int len)
{
    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
int main() {
    int n1,n2,n3, i, j, temp;

    printf("Enter the number of elements in the array1: ");
    scanf("%d", &n1);

    printf("Enter the number of elements in the array2: ");
    scanf("%d", &n2);

    n3 = n1+n2;

    int arr[n1], arr1[n2], arr2[n3];

    printf("Enter %d elements of arr 1:\n", n1);
    for (i = 0; i < n1; i++) {
        scanf("%d", &arr[i]);
        arr2[i] = arr[i];
        // Sort the array elements immediately within the loop
        for (j = 0; j < i; j++) {
            if (arr[j] > arr[i]) {
                // Swap elements if they are out of order
                // temp = arr[j];
                // arr[j] = arr[i];
                // arr[i] = temp;
                // Swap code
                swap_code(arr, i, j);
                swap_code(arr2,i,j);
            }
        }
    }
    printf("Enter %d elements of arr 2:\n", n2);
    for (i = 0; i < n2; i++) {
        scanf("%d", &arr1[i]);
        arr2[n1+i] = arr1[i];
        // Sort the array elements immediately within the loop
        for (j = 0; j < i; j++) {
            if (arr1[j] > arr1[i]) {
                // Swap elements if they are out of order
                // temp = arr[j];
                // arr[j] = arr[i];
                // arr[i] = temp;
                // Swap code
                swap_code(arr1, i, j);
                swap_code(arr2, n1+i,n1+j);
            }
        }
    }

    printf("Sorted array: ");
    print_array(arr,n1);
    print_array(arr1,n2);
    print_array(arr2,n3);
    

    return 0;
}

#endif