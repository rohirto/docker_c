#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// Define the size of the circular buffer
#define BUFFER_SIZE 10

// Circular buffer structure
typedef struct {
    uint8_t data[BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    bool full;
} CircularBuffer;

// Initialize the circular buffer
void CircularBuffer_Init(CircularBuffer* buffer) {
    buffer->head = 0;
    buffer->tail = 0;
    buffer->full = false;
}

// Check if the circular buffer is empty
bool CircularBuffer_IsEmpty(const CircularBuffer* buffer) {
    return !buffer->full && (buffer->head == buffer->tail);
}

// Check if the circular buffer is full
bool CircularBuffer_IsFull(const CircularBuffer* buffer) {
    return buffer->full;
}

// Get the number of elements in the circular buffer
uint8_t CircularBuffer_Size(const CircularBuffer* buffer) {
    if (buffer->full) {
        return BUFFER_SIZE;
    } else if (buffer->head >= buffer->tail) {
        return buffer->head - buffer->tail;
    } else {
        return BUFFER_SIZE - (buffer->tail - buffer->head);
    }
}

// Add data to the circular buffer
bool CircularBuffer_Put(CircularBuffer* buffer, uint8_t data) {
    if (!buffer->full) {
        buffer->data[buffer->head] = data;
        buffer->head = (buffer->head + 1) % BUFFER_SIZE;

        // Check if the buffer is now full
        if (buffer->head == buffer->tail) {
            buffer->full = true;
        }

        return true;
    }
    return false; // Buffer is full
}

// Get data from the circular buffer
bool CircularBuffer_Get(CircularBuffer* buffer, uint8_t* data) {
    if (!CircularBuffer_IsEmpty(buffer)) {
        *data = buffer->data[buffer->tail];
        buffer->tail = (buffer->tail + 1) % BUFFER_SIZE;
        buffer->full = false;
        return true;
    }
    return false; // Buffer is empty
}

int run_time_loop(CircularBuffer *buffer)
{
    uint8_t data;
    printf("Enter data (q to quit):\n");
    char input[10];
    while (1) {
        printf("Enter a number: ");
        scanf("%9s", input);

         switch (input[0])
        {
        case 'q':
            /* code */
        case 'Q':
            return 0;
            break;
        case 'g':   //Get the data
        case'G':
            printf("Data in the circular buffer: ");
            while (!CircularBuffer_IsEmpty(buffer)) {
                CircularBuffer_Get(buffer, &data);
                printf("%d ", data);
            }
            printf("\n");
            break; 

        
        default:
            uint8_t value;
            if (sscanf(input, "%hhu", &value) == 1) {
                if (!CircularBuffer_Put(buffer, value)) {
                printf("Buffer is full. Cannot add more data.\n");
                return 0;
                }
            }
            else {
                printf("Invalid input. Please enter a number.\n");
            }
            break;
        }


    }
    return 0;
}

int main() {
    CircularBuffer buffer;
    

    CircularBuffer_Init(&buffer);

    run_time_loop(&buffer);

    // printf("Enter data (q to quit):\n");

    // char input[10];
    //while (1) {
    //     printf("Enter a number: ");
    //     scanf("%s", input);

    //     if (input[0] == 'q' || input[0] == 'Q') {
    //         break;
    //     }

    //     uint8_t value;
    //     if (sscanf(input, "%hhu", &value) == 1) {
    //         if (!CircularBuffer_Put(&buffer, value)) {
    //             printf("Buffer is full. Cannot add more data.\n");
    //             break;
    //         }
    //     } else {
    //         printf("Invalid input. Please enter a number.\n");
    //     }
    // }

    // printf("Data in the circular buffer: ");
    // while (!CircularBuffer_IsEmpty(&buffer)) {
    //     CircularBuffer_Get(&buffer, &data);
    //     printf("%d ", data);
    // }
    // printf("\n");
    //}
    return 0;
}