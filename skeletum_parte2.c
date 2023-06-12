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

    printf("%ld\n", sizeof(unsigned int));

    if (strcmp(opt, "-l") == 0) {
        // Listar todos os arquivos e tamanhos no arquivamento
        return 0;
    }

    // Criar arquivo. 
    if (strcmp(opt, "-c") == 0){
        char *output_file = argv[2];
        char **input_files = &argv[3];
        int num_input_files = argc - 3;


        printf("Arquivo de saída: %s\n", output_file);
        for(int i=0; i < num_input_files; i++) {
            printf("Arquivo de entrada %d: %s\n", i, input_files[i]);
        }

        FILE *destinationFile;
        destinationFile = fopen(output_file, "w");

        for(int i=0; i < num_input_files; i++) {
            FILE *currentFile;
            currentFile = fopen(input_files[i], "rb");

            if(currentFile == NULL) {
                printf("O arquivo %s nao existe!\n", input_files[i]);
            }else{
                char content[100];
                fgets(content, 100, currentFile);
                struct file_header *header = (struct file_header *) malloc(sizeof(struct file_header));
                header->file_size = strlen(content);
                memset(header->name, '!', MAXNAME - header->file_size);
                strcat(header->name, input_files[i]);

                printf("%s", content);
                fprintf(destinationFile, "%d", header->file_size);
                fprintf(destinationFile, "%s", header->name);
                fprintf(destinationFile, "%s", content);
                fclose(currentFile);
            }
        }
        fclose(destinationFile);
    }

    // Extrair arquivo. 
    if (strcmp(opt, "-e") == 0){
        char *extract_file = argv[2];
        printf("Arquivo para extrair: %s\n", extract_file);
    }

    return 0;
}