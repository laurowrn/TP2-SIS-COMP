#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 256

#pragma pack(push, 1)
struct file_header {
    char name[MAXNAME];       // Nome do arquivo
    unsigned long file_size;  // Tamanho do arquivo em bytes
};
#pragma pack(pop)

int main() {
    struct file_header *header = malloc(sizeof(struct file_header));

    FILE *output = fopen("arquivo_saida", "wb");
    FILE *input1 = fopen("arquivos/arquivo0.txt", "rb");

    fseek(input1, 0L, SEEK_END);
    long input_size1 = ftell(input1);
    rewind(input1);

    char *input_message1 = malloc(input_size1);
    size_t bytes_read = fread(input_message1, 1, input_size1, input1);

    header->file_size = (unsigned long)input_size1;
    strcpy(header->name, "arquivo0.txt");

    fwrite(header, sizeof(struct file_header), 1, output);
    fwrite(input_message1, bytes_read, 1, output);

    free(input_message1);
    fclose(input1);

    FILE *input2 = fopen("arquivos/arquivo1.txt", "rb");
    fseek(input2, 0L, SEEK_END);
    long input_size2 = ftell(input2);
    rewind(input2);

    char *input_message2 = malloc(input_size2);
    bytes_read = fread(input_message2, 1, input_size2, input2);

    header->file_size = (unsigned long)input_size2;
    strcpy(header->name, "arquivo1.txt");

    fwrite(header, sizeof(struct file_header), 1, output);
    fwrite(input_message2, bytes_read, 1, output);

    free(input_message2);
    fclose(input2);

    fclose(output);

    output = fopen("arquivo_saida", "rb");
    FILE *input1_out = fopen("arquivo0_out.txt", "wb");

    char name_received1[MAXNAME];
    unsigned long size_received1;
    fread(name_received1, sizeof(char), MAXNAME, output);
    fread(&size_received1, sizeof(unsigned long), 1, output);

    char *message_received1 = malloc(size_received1);
    bytes_read = fread(message_received1, 1, size_received1, output);

    fwrite(message_received1, bytes_read, 1, input1_out);

    printf("%s\n", name_received1);
    printf("%lu\n", size_received1);
    printf("%s\n", message_received1);

    fclose(input1_out);
    free(message_received1);

    FILE *input2_out = fopen("arquivo1_out.txt", "wb");

    char name_received2[MAXNAME];
    unsigned long size_received2;
    fread(name_received2, sizeof(char), MAXNAME, output);
    fread(&size_received2, sizeof(unsigned long), 1, output);

    char *message_received2 = malloc(size_received2);
    bytes_read = fread(message_received2, 1, size_received2, output);

    fwrite(message_received2, bytes_read, 1, input2_out);

    printf("%s\n", name_received2);
    printf("%lu\n", size_received2);
    printf("%s\n", message_received2);

    fclose(input2_out);
    free(message_received2);

    fclose(output);

    free(header);
}
