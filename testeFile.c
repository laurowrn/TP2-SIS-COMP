#define _OPEN_SYS_ITOA_EXT
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


    FILE *input;
    FILE *output;
    int input_size = 0;
    char input_name[256];
    char input_path[300];
    char output_path[300] = "arquivo_saida.txt";
    output = fopen(output_path, "wb");
    for(int i = 0; i < 10; i++){
        snprintf(input_name, 256, "%s%d%s", "arquivo", i, ".txt");
        snprintf(input_path, 300, "%s%s", "arquivos/", input_name);
        input = fopen(input_path, "r");
        fseek(input, 0L, SEEK_END);
        input_size = ftell(input);
        rewind(input);

        char input_message[input_size];
        fread(input_message, input_size, 1, input);
        input_message[input_size] = '\0';
        // printf("%s\n", input_message);
        fclose(input);


        fwrite(&input_size, sizeof(int), 1, output);
        fwrite(input_name, sizeof(input_name), 1, output);
        fwrite(input_message, input_size + 1, 1, output);

    }

    fclose(output);

    output = fopen (output_path, "rb");
    fseek(output, 0L, SEEK_END);
    int destination_size = ftell(output);
    rewind(output);

    // printf("%d\n", destination_size);

    int current_position = 0;
    while(current_position < destination_size){
        int size_received1;
        char name_received1[256];
        fread(&size_received1, sizeof(int), 1, output);
        current_position += (int) sizeof(int);
        fread(name_received1, sizeof(name_received1), 1, output);
        current_position += (int) sizeof(name_received1);

        char message_received1[size_received1 + 1];
        fread(message_received1, size_received1 + 1, 1, output);
        current_position += size_received1 + 1;
        message_received1[size_received1] = '\0';

        printf("%d\n", size_received1);
        printf("%s\n", name_received1);
        // printf("%s\n", message_received1);
    }

    fclose(output);










    // FILE *input1 = fopen("origem1.txt", "r");
    // fseek(input1, 0L, SEEK_END);
    // int input_size1 = ftell(input1);
    // rewind(input1);

    // char input_message1[input_size1];
    // fread(input_message1, input_size1, 1, input1);

    // fclose(input1);

    // char name1[256] = "arquivo.txt";




    // FILE *input2 = fopen("origem2.txt", "r");
    // fseek(input2, 0L, SEEK_END);
    // int input_size2 = ftell(input2);
    // rewind(input2);

    // char input_message2[input_size2];
    // fread(input_message2, input_size2, 1, input2);

    // fclose(input2);

    // char name2[256] = "arquivo.txt";






    // FILE *destination_file;
    // destination_file = fopen ("arquivo_saida.txt", "wb");



    // fwrite(&input_size1, sizeof(int), 1, destination_file);
    // fwrite(name1, sizeof(name1), 1, destination_file);
    // fwrite(input_message1, input_size1 + 1, 1, destination_file);

    // fwrite(&input_size2, sizeof(int), 1, destination_file);
    // fwrite(name2, sizeof(name2), 1, destination_file);
    // fwrite(input_message2, input_size2 + 1, 1, destination_file);

    // fclose(destination_file);

    



    // destination_file = fopen ("data.txt", "rb");
    // fseek(destination_file, 0L, SEEK_END);
    // int destination_size = ftell(destination_file);
    // rewind(destination_file);

    // int size_received1;
    // char name_received1[256];
    // fread(&size_received1, sizeof(int), 1, destination_file);
    // fread(name_received1, sizeof(name_received1), 1, destination_file);

    // char message_received1[size_received1 + 1];
    // fread(message_received1, size_received1 + 1, 1, destination_file);
    // message_received1[size_received1] = '\0';

    // printf("%d\n", size_received1);
    // printf("%s\n", name_received1);
    // printf("%s\n", message_received1);




    // int size_received2;
    // char name_received2[256];
    // fread(&size_received2, sizeof(int), 1, destination_file);
    // fread(name_received2, sizeof(name_received2), 1, destination_file);

    // char message_received2[size_received2 + 1];
    // fread(message_received2, size_received2 + 1, 1, destination_file);
    // message_received2[size_received2] = '\0';

    // printf("%d\n", size_received2);
    // printf("%s\n", name_received2);
    // printf("%s\n", message_received2);

    // fclose(destination_file);

    // printf("%d\n", destination_size);







    // destination_file = fopen ("arquivo_saida.txt", "rb");
    // fseek(destination_file, 0L, SEEK_END);
    // int destination_size = ftell(destination_file);
    // rewind(destination_file);

    // printf("%d\n", destination_size);

    // int current_position = 0;
    // while(current_position < destination_size){
    //     int size_received1;
    //     char name_received1[256];
    //     fread(&size_received1, sizeof(int), 1, destination_file);
    //     current_position += (int) sizeof(int);
    //     fread(name_received1, sizeof(name_received1), 1, destination_file);
    //     current_position += (int) sizeof(name_received1);

    //     char message_received1[size_received1 + 1];
    //     fread(message_received1, size_received1 + 1, 1, destination_file);
    //     current_position += size_received1 + 1;
    //     message_received1[size_received1] = '\0';

    //     printf("%d\n", size_received1);
    //     printf("%s\n", name_received1);
    //     printf("%s\n", message_received1);
    // }

    // fclose(destination_file);
}