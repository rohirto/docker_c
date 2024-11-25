#include <cjson/cJSON.h>
#include <stdio.h>

#define cert_buff "{\"person\": {\"name\": \"John\", \"age\": 30}, \"array\": [{\"name\": \"Alice\"}, {\"name\": \"Bob\"}]}"
unsigned char array[] = cert_buff;

// Function to recursively search for a field
cJSON* search_json(cJSON *json, const char *field) 
{
    cJSON *item = NULL;

    // If the current element is an object, search its keys
    if (cJSON_IsObject(json)) 
    {
        item = cJSON_GetObjectItem(json, field);
        if (item != NULL) 
        {
            return item;
        }
        // Recursively search all object fields
        cJSON *child = json->child;
        while (child) 
        {
            cJSON *result = search_json(child, field);
            if (result != NULL) 
            {
                return result;
            }
            child = child->next;
        }
    }

    // If the current element is an array, search all array elements
    if (cJSON_IsArray(json)) 
    {
        int array_size = cJSON_GetArraySize(json);
        for (int i = 0; i < array_size; i++) 
        {
            cJSON *element = cJSON_GetArrayItem(json, i);
            cJSON *result = search_json(element, field);
            if (result != NULL) 
            {
                return result;
            }
        }
    }

    return NULL; // Field not found
}

int main()
{
    cJSON* root = cJSON_Parse((char *)array);
    if(root == NULL)
    {
        printf("Error parsing JSON\n");
        return 1;
    }
   // Search for the field "name"
    cJSON *found = search_json(root, "name");
    if (found) {
        printf("Field found: %s\n", cJSON_GetStringValue(found));
    } else {
        printf("Field not found\n");
    }

    // Clean up
    cJSON_Delete(root);
    return 0;
}