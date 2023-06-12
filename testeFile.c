#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 256
#pragma pack(push, 1)
struct file_header{
    char name [MAXNAME]; // Nome do arquivo
    unsigned int file_size; // Tamanho do arquivo em bytes
};
#pragma pack (pop)

int main(int argc, char **argv) {
    FILE *fp = fopen ("data.txt", "wb");
    int myValue = 1500;
    fwrite(&myValue, sizeof(myValue), 1, fp);
    fclose(fp);

    fp = fopen ("data.txt", "rb");
    int received;
    fread(&received, sizeof(received), 1, fp);
    printf("%d\n", received);
}