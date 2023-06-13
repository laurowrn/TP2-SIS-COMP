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
        FILE *output = fopen(output_file, "rb");
        fseek(output, 0L, SEEK_END);
        int destination_size = ftell(output);
        rewind(output);

        int current_position = 0;
        printf("----------------------------------------\n");
        printf("-Lista de arquivos dentro do arquivador-\n");
        printf("----------------------------------------\n");
        printf("-XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX-\n");

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
            fseek(output, 0L, current_position - 1);
            // fread(message_received, size_received + 1, 1, output);
            message_received[size_received] = '\0';
            printf("----------------------------------------\n");
            printf("Nome do Arquivo: %s\n", name_received);
            printf("Tamanho do Arquivo: %d\n", size_received);
            printf("----------------------------------------\n");
        }

        fclose(output);
        return 0;
    }

    // Criar arquivo.
    if (strcmp(opt, "-c") == 0)
    {
        char *output_file = argv[2];
        char **input_files = &argv[3];
        int num_input_files = argc - 3;

        printf("Arquivo de saída: %s\n", output_file);
        for (int i = 0; i < num_input_files; i++)
        {
            printf("Arquivo de entrada %d: %s\n", i, input_files[i]);
        }

        struct file_header *header = (struct file_header *)malloc(sizeof(struct file_header));
        int input_size = 0;
        char input_name[256];
        FILE *output = fopen(output_file, "wb");

        if (output == NULL)
        {
            printf("O arquivo %s nao existe!\n", output_file);
        }
        else
        {
            for (int i = 0; i < num_input_files; i++)
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
                    strcpy(header->name, input_files[i]);

                    fwrite(header, sizeof(struct file_header), 1, output);
                    fwrite(input_message, input_size + 1, 1, output);
                }
            }
        }
    }

    // Extrair arquivo.
    if (strcmp(opt, "-e") == 0)
    {
        char *extract_file = argv[2];
        printf("Arquivo para extrair: %s\n", extract_file);
    }

    return 0;
}