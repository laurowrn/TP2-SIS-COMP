#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define MAXNAME 256
#pragma pack(push, 1)
struct file_header {
    char name[MAXNAME];       // Nome do arquivo
    unsigned long file_size;  // Tamanho do arquivo em bytes
};
#pragma pack(pop)

void archive_files(char **input_files, char *output_file, int number_of_input_files) {
    struct file_header *header = (struct file_header *)malloc(sizeof(struct file_header));
    FILE *output = fopen(output_file, "wb");

    FILE *input;
    size_t bytes_read;

    for (int i = 0; i < number_of_input_files; i++) {
        input = fopen(input_files[i], "rb");

        if (input == NULL) {
            printf("Arquivo %s não encontrado!\n", input_files[i]);
            return;
        }

        fseek(input, 0L, SEEK_END);
        unsigned long input_size = ftell(input);
        rewind(input);

        char *input_data = malloc(input_size);
        bytes_read = fread(input_data, 1, input_size, input);

        header->file_size = (unsigned long)input_size;

        char *token = strtok(input_files[i], "/");
        char last_token[256];
        while (token != NULL) {
            strcpy(last_token, token);
            token = strtok(NULL, "/");
        }

        // Salva o nome do arquivo em uma estrutura
        strcpy(header->name, last_token);

        // Escreve o header no arquivo de saida
        fwrite(header, sizeof(struct file_header), 1, output);

        // Escreve o conteúdo do arquivo de entrada no arquivo de saida
        fwrite(input_data, bytes_read, 1, output);

        free(input_data);

        fclose(input);
    }
    fclose(output);
    free(header);
}

void extract_all_files(char *file_path) {
    FILE *file = fopen(file_path, "rb");

    if (file == NULL) {
        printf("Arquivo não encontrado!\n");
        return;
    }

    FILE *new_file;
    size_t bytes_read;

    char name[MAXNAME];
    unsigned long size;
    char *data;

    unsigned long total_size;
    unsigned long current_position = 0;

    fseek(file, 0L, SEEK_END);
    total_size = ftell(file);
    rewind(file);

    // printf("%ld\n", total_size);

    while (current_position < total_size) {
        fread(name, 1, MAXNAME, file);
        // printf("%s\n", name);
        current_position += MAXNAME;
        fread(&size, sizeof(unsigned long), 1, file);
        // printf("%ld\n", size);
        current_position += sizeof(unsigned long);

        data = malloc(size);
        bytes_read = fread(data, 1, size, file);
        current_position += size;

        new_file = fopen(name, "wb");

        fwrite(data, bytes_read, 1, new_file);

        // printf("%s\n", name);
        // printf("%lu\n", size);
        // printf("%s\n", data);

        fclose(new_file);
    }
    // printf("%ld\n", current_position);

    fclose(file);
    free(data);
}

void list_all_files(char *file_path) {
    FILE *file = fopen(file_path, "rb");
    size_t bytes_read;

    char name[MAXNAME];
    unsigned long size;
    char *data;

    unsigned long total_size;
    unsigned long current_position = 0;
    ;
    fseek(file, 0L, SEEK_END);
    total_size = ftell(file);
    rewind(file);

    // printf("%ld\n", total_size);

    printf("-----------------------------------------------\n");
    printf("Lista de arquivos arquivados em %s\n", file_path);
    printf("-----------------------------------------------\n");
    int i = 0;
    while (current_position < total_size) {
        fread(name, 1, MAXNAME, file);
        // printf("%s\n", name);
        current_position += MAXNAME;
        fread(&size, sizeof(unsigned long), 1, file);
        // printf("%ld\n", size);
        current_position += sizeof(unsigned long);

        data = malloc(size);
        bytes_read = fread(data, 1, size, file);
        current_position += size;

        printf("-----------------------------------------------\n");
        printf("Índice: %d\n", i);
        printf("Nome: %s\n", name);
        printf("Tamanho: %lu bytes\n", size);
        printf("-----------------------------------------------\n");
        i++;
    }
    // printf("%ld\n", current_position);

    fclose(file);
    free(data);
}

int main(int argc, char **argv) {
    // Verifique se temos os argumentos mínimos necessários
    if (argc < 2) {
        printf("Uso:\n");
        printf("\t%s -c <arquivo_de_saida> <arquivo1> <arquivo2> ... <arquivoN> - Cria arquivo.\n", argv[0]);
        printf("\t%s -l - Lista arquivos.\n", argv[0]);
        printf("\t%s -e <arquivo> - Extrai conteúdo.\n", argv[0]);
        return 1;
    }

    // O primeiro argumento é o arquivo de saída ou opção de listagem -l
    char *opt = argv[1];

    if (strcmp(opt, "-l") == 0) {
        char *output_file = argv[2];

        list_all_files(output_file);

        return 0;
    }

    // Criar arquivo.
    if (strcmp(opt, "-c") == 0) {
        char *output_file = argv[2];
        char **input_files = &argv[3];
        int number_of_input_files = argc - 3;

        printf("Arquivo de saída: %s\n", output_file);
        for (int i = 0; i < number_of_input_files; i++) {
            printf("Arquivo de entrada %d: %s\n", i, input_files[i]);
        }

        archive_files(input_files, output_file, number_of_input_files);

        return 0;
    }

    // Extrair arquivo.
    if (strcmp(opt, "-e") == 0) {
        char *extract_file = argv[2];

        extract_all_files(extract_file);
    }

    return 0;
}