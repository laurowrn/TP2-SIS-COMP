#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

// Function to list files in a specific directory
char** list_files_in_directory(const char* directory, int* file_count) {
    DIR* dir;
    struct dirent* entry;
    int count = 0;

    // Open the specified directory
    dir = opendir(directory);

    if (dir == NULL) {
        printf("Failed to open the directory.\n");
        return NULL;
    }

    // Count the number of files in the directory
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Check if it's a regular file
            count++;
        }
    }

    // Allocate memory for the array of strings
    char** file_paths = (char**)malloc(count * sizeof(char*));
    if (file_paths == NULL) {
        printf("Failed to allocate memory.\n");
        return NULL;
    }

    // Reset the directory pointer to the beginning
    rewinddir(dir);

    int i = 0;
    // Store the file paths in the array
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) { // Check if it's a regular file
            char* file_path = (char*)malloc((strlen(directory) + strlen(entry->d_name) + 2) * sizeof(char));
            sprintf(file_path, "%s/%s", directory, entry->d_name);
            file_paths[i] = file_path;
            i++;
        }
    }

    // Set the number of files found
    *file_count = count;

    // Close the directory
    closedir(dir);

    return file_paths;
}

// Function to free the memory allocated by the array of strings
void free_string_array(char** array, int size) {
    if (array == NULL) {
        return;
    }

    for (int i = 0; i < size; i++) {
        free(array[i]);
    }

    free(array);
}

int main() {
    const char* directory = "parte2_arquivador/arquivos"; // Replace with the desired directory

    int file_count;
    char** file_paths = list_files_in_directory(directory, &file_count);

    if (file_paths != NULL) {
        printf("Files found in the directory '%s':\n", directory);
        for (int i = 0; i < file_count; i++) {
            printf("%s\n", file_paths[i]);
        }

        free_string_array(file_paths, file_count);
    }

    return 0;
}
