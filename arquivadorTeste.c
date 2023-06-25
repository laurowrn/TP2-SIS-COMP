#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXNAME 256
#pragma pack(push, 1)
struct file_header
{
    char name[MAXNAME];     // Nome do arquivo
    unsigned int file_size; // Tamanho do arquivo em bytes
};
#pragma pack(pop)

struct file_item
{
    struct file_header header;
    char *data;
    struct file_item *next;
};

struct file_item *file_list;

void archive_files(char **input_files, char *output_file, int number_of_input_files)
{
    file_list = (struct file_item *)malloc(sizeof(struct file_item));
    file_list->header.file_size = -1;
    file_list->next = NULL;

    int input_size = 0;
    char input_name[256];
    FILE *output = fopen(output_file, "wb");

    if (output == NULL)
    {
        printf("O arquivo %s nao existe!\n", output_file);
    }
    else
    {
        for (int i = 0; i < number_of_input_files; i++)
        {
            FILE *input;
            input = fopen(input_files[i], "rb");

            if (input == NULL)
            {
                printf("O arquivo %s nao existe!\n", input_files[i]);
            }
            else
            {
                fseek(input, 0L, SEEK_END);
                input_size = ftell(input);
                rewind(input);
                char input_message[input_size];
                fread(input_message, input_size, 1, input);
                input_message[input_size] = '\0';
                fclose(input);

                header->file_size = (unsigned int)input_size;

                char *token = strtok(input_files[i], "/");
                char last_token[256];
                while (token != NULL)
                {
                    strcpy(last_token, token);
                    // printf(" %s\n", token); // printing each token
                    token = strtok(NULL, "/");
                }
                // printf("%s\n", last_token);

                strcpy(header->name, last_token);

                fwrite(header, sizeof(struct file_header), 1, output);
                fwrite(input_message, input_size + 1, 1, output);
            }
        }
    }
    fclose(output);
}

void get_file_list(char *output_file)
{
    FILE *output = fopen(output_file, "rb");

    if (output == NULL)
    {
        printf("O arquivo %s nao existe!\n", output_file);
    }
    else
    {
        fseek(output, 0L, SEEK_END);
        int destination_size = ftell(output);
        rewind(output);

        int current_position = 0;
        while (current_position < destination_size)
        {
            int size_received;
            char name_received[256];

            fread(name_received, sizeof(name_received), 1, output);
            current_position += (int)sizeof(name_received);
            fread(&size_received, sizeof(int), 1, output);
            current_position += (int)sizeof(int);

            char message_received[size_received];
            current_position += size_received + 1;
            fread(message_received, size_received + 1, 1, output);
            message_received[size_received] = '\0';
            // printf("%s\n", message_received);

            if (file_list->header.file_size == -1)
            {
                file_list->header.file_size = (unsigned int)size_received;
                strcat(file_list->header.name, name_received);
                file_list->data = malloc(size_received + 1);
                strcat(file_list->data, message_received);
            }
            else
            {
                struct file_item *last_file = (struct file_item *)malloc(sizeof(struct file_item));
                struct file_item *current_file = (struct file_item *)malloc(sizeof(struct file_item));
                current_file->next = NULL;
                current_file->header.file_size = size_received;
                strcat(current_file->header.name, name_received);

                current_file->data = malloc(size_received + 1);
                strcat(current_file->data, message_received);

                last_file = file_list;
                while (last_file->next)
                {
                    last_file = last_file->next;
                }
                last_file->next = current_file;
            }
        }
    }
    fclose(output);
}

void print_file_list()
{
    struct file_item *current_file = (struct file_item *)malloc(sizeof(struct file_item));
    current_file = file_list;
    printf("----------------------------------------\n");
    printf("-Lista de arquivos dentro do arquivador-\n");
    printf("----------------------------------------\n");
    printf("-XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX-\n");
    while (current_file != NULL)
    {
        printf("----------------------------------------\n");
        printf("Nome do Arquivo: %s\n", current_file->header.name);
        printf("Tamanho do Arquivo: %d\n", (unsigned int)current_file->header.file_size);
        printf("Conteúdo: %s\n", current_file->data);
        printf("----------------------------------------\n");
        current_file = current_file->next;
    }
}

int main(int argc, char **argv)
{

    // Verifique se temos os argumentos mínimos necessários
    if (argc < 2)
    {
        printf("Uso:\n");
        printf("\t%s -c <arquivo_de_saida> <arquivo1> <arquivo2> ... <arquivoN> - Cria arquivo.\n", argv[0]);
        printf("\t%s -l - Lista arquivos.\n", argv[0]);
        printf("\t%s -e <arquivo> - Extrai conteúdo.\n", argv[0]);
        return 1;
    }

    // O primeiro argumento é o arquivo de saída ou opção de listagem -l
    char *opt = argv[1];

    if (strcmp(opt, "-l") == 0)
    {
        char *output_file = argv[2];

        get_file_list(output_file);
        print_file_list();

        return 0;
    }

    // Criar arquivo.
    if (strcmp(opt, "-c") == 0)
    {
        char *output_file = argv[2];
        char **input_files = &argv[3];
        int number_of_input_files = argc - 3;

        printf("Arquivo de saída: %s\n", output_file);
        for (int i = 0; i < number_of_input_files; i++)
        {
            printf("Arquivo de entrada %d: %s\n", i, input_files[i]);
        }

        archive_files(input_files, output_file, number_of_input_files);

        return 0;
    }

    // Extrair arquivo.
    if (strcmp(opt, "-e") == 0)
    {
        char *extract_file = argv[2];
        struct file_item *current_file_item = (struct file_item *)malloc(sizeof(struct file_item));
        FILE *current_file;

        printf("Arquivo para extrair: %s\n", extract_file);

        get_file_list(extract_file);
        while (current_file_item)
        {
            // current_file = fopen(current_file_item->header.name);
            printf("%s\n", current_file_item->data);
        }
    }

    return 0;
}