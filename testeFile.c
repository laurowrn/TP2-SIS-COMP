#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 256
#pragma pack(push, 1)
struct file_header {
    char name[MAXNAME];      // Nome do arquivo
    unsigned int file_size;  // Tamanho do arquivo em bytes
};
#pragma pack(pop)

int main(int argc, char **argv) {
    // Coloca o arquivo de entrada no arquivo de saída
    struct file_header *header = (struct file_header *)malloc(sizeof(struct file_header));

    FILE *output = fopen("arquivo_saida", "wb");
    FILE *input1 = fopen("arquivos/arquivo0.txt", "rb");

    fseek(input1, 0L, SEEK_END);

    // Obtem a posicao atual do cursor para definir o tamanho do conteudo
    int input_size = ftell(input1);

    rewind(input1);

    char input_message1[input_size];
    fread(input_message1, input_size, 1, input1);

    header->file_size = (unsigned int)input_size;

    // Salva o nome do arquivo em uma estrutura
    strcpy(header->name, "arquivo0.txt");

    // Escreve o header no arquivo de saida
    fwrite(header, sizeof(struct file_header), 1, output);

    // Escreve o conteúdo do arquivo de entrada no arquivo de saida
    fwrite(input_message1, input_size, 1, output);

    printf("Tamanho: %d\n", input_size);

    fseek(input1, 0L, SEEK_END);

    // Obtem a posicao atual do cursor para definir o tamanho do conteudo
    input_size = ftell(input1);

    rewind(input1);

    char input_message2[input_size];
    fread(input_message2, input_size, 1, input1);

    header->file_size = (unsigned int)input_size;

    // Salva o nome do arquivo em uma estrutura
    strcpy(header->name, "arquivo1.txt");

    // Escreve o header no arquivo de saida
    fwrite(header, sizeof(struct file_header), 1, output);

    // Escreve o conteúdo do arquivo de entrada no arquivo de saida
    fwrite(input_message2, input_size, 1, output);

    fclose(input1);
    fclose(output);

    output = fopen("arquivo_saida", "rb");
    input1 = fopen("arquivo0.txt", "wb");

    char name_received[256];
    int size_received;
    char message_received[size_received];

    fread(name_received, sizeof(name_received), 1, output);
    fread(&size_received, sizeof(unsigned int), 1, output);
    fread(message_received, size_received, 1, output);

    fwrite(message_received, size_received, 1, input1);

    printf("%s\n", name_received);
    printf("%d\n", size_received);
    printf("%s\n", message_received);

    fread(name_received, sizeof(name_received), 1, output);
    fread(&size_received, sizeof(unsigned int), 1, output);
    fread(message_received, size_received, 1, output);

    fwrite(message_received, size_received, 1, input1);

    printf("%s\n", name_received);
    printf("%d\n", size_received);
    printf("%s\n", message_received);

    fclose(output);
    fclose(input1);
}