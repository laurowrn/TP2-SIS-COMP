#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *mem_area;  // Ponteiro que apontara para a area de memoria contigua alocada

#pragma pack(push, 1)
struct mem_block {           // Estrutura padrao para o cabecalho de um bloco de momoria
    int is_free;             // Se o bloco esta livre (1) ou nao (0).
    size_t size;             // Tamanho do bloco de memoria.
    void *mem_ptr;           // Ponteiro para o bloco de memoria.
    struct mem_block *next;  // Ponteiro para o proximo bloco na lista.
};
#pragma pack(pop)

// Funcao que aloca a memoria contigua livre inicial do tamanho desejado
void allocate_initial_memory(int initial_size) {  // Recebe como parametro o tamanho da memoria alocada desejada
    initial_size += sizeof(struct mem_block);     // Adiciona ao tamanho inicial desejado o tamanho do cabecalho do bloco
    mem_area = (void *)malloc(initial_size);      // Aloca um bloco de momoria do tamanho initial_size e faz o mem_area apontar pra ele

    void *current_block = mem_area;                                               // Cria um novo ponteiro e faz apontar pra regiao mem_area
    struct mem_block *current_block_content = (struct mem_block *)current_block;  // Faz o casting do ponteiro criado para o tipo struct mem_block e cria um novo ponteiro do mesmo tipo que aponta para o endereco de memoria current_block

    // Atribuicao das informacoes do cabecalho do bloco inicial criado
    current_block_content->is_free = 1;
    current_block_content->size = initial_size - sizeof(struct mem_block);
    current_block_content->mem_ptr = current_block + sizeof(struct mem_block);
    current_block_content->next = NULL;
}

// Funcao que printa o estado das alocacoes de memoria
void print_mem_block() {
    void *current_block;
    current_block = mem_area;

    // Cabeçalho do primeiro bloco de memória
    struct mem_block *current_block_content = (struct mem_block *)current_block;

    printf("\n");
    printf("-----------------------------\n");
    printf("------Comeco da memoria------\n");
    printf("-----------------------------\n");
    while (current_block_content) {  // Enquanto o bloco de memoria nao for NULL
        printf("is_free: %d\n", current_block_content->is_free);
        printf("size: %ld\n", current_block_content->size);
        printf("mem_ptr: %p\n", current_block_content->mem_ptr);
        printf("next: %p\n", current_block_content->next);
        printf("-----------------------------\n");

        current_block_content = current_block_content->next;  // Aponta para o proximo bloco de memoria
    }
    printf("--------Fim da memoria-------\n");
    printf("-----------------------------\n");
}

// Encontra um espaco para o novo bloco de momoria desejado e retorna o endereco de memoria do bloco
void *smalloc(size_t size) {
    void *current_block = mem_area;  // Cria um ponteiro do tipo void que aponta para o inicio do bloco

    struct mem_block *current_block_content = (struct mem_block *)current_block;  // realiza o casting do ponteiro para o tipo struct mem_block e atribui ao novo ponteiro current_block_content
    struct mem_block *found_block_content = NULL;                                 // Cria um ponteiro do tipo struct mem_block que inicialmente aponta para NULL mas futuramente indicara o espaco de memoria encontrado

    while (current_block_content) {                                                                              // enquanto ponteiro current_block_content nao apontar para null
        if (current_block_content->is_free && current_block_content->size >= size + sizeof(struct mem_block)) {  // Verifica se o bloco atual esta livre e o espaco disponivel dele e maior ou igual ao tamanho de alocacao desejado
            found_block_content = current_block_content;                                                         // Caso seja condicao verdadeira, found_block_content aponta para tal regiao
            break;
        }
        current_block_content = current_block_content->next;  // Caso contario, aponta-se para o proximo bloco da lista
    }

    if (found_block_content == NULL) {  // Verifica se um bloco de memoria do tamanho desejado foi encontrado
        printf("Memória insuficiente! Não é possível alocar um bloco com %ld bytes.\n", size);
        return NULL;
    }

    // Verifica se o tamanho do bloco atual permite a criacao de um novo bloco livre
    if (found_block_content->size > size + sizeof(struct mem_block)) {
        // Calcula o endereco do novo bloco livre
        struct mem_block *new_block = (struct mem_block *)((void *)found_block_content + sizeof(struct mem_block) + size);

        // Configura o novo bloco livre
        new_block->is_free = 1;
        new_block->size = found_block_content->size - size - sizeof(struct mem_block);
        new_block->mem_ptr = (void *)new_block + sizeof(struct mem_block);
        new_block->next = found_block_content->next;

        // Atualiza a lista de blocos, inserindo o novo bloco livre
        found_block_content->next = new_block;

        // Atualiza o bloco alocado
        found_block_content->is_free = 0;
        found_block_content->size = size;
    } else {
        // Caso o tamanho do bloco seja exatamente o necessario, apenas marca o bloco como alocado
        found_block_content->is_free = 0;
    }

    // Retorna o endereco do bloco de memoria encontrado
    return found_block_content->mem_ptr;
}

// Faz a juncao dos blocos coadjacentes que estejam livres
void coalesce_free_blocks() {
    struct mem_block *current_block = (struct mem_block *)mem_area;  // Ponteiro para o bloco atual da regiao de memoria alocada

    while (current_block && current_block->next) {                     // Enquanto o atual e o proximo bloco nao apontar pra null
        if (current_block->is_free && current_block->next->is_free) {  // verifica se o bloco atual e o proximo bloco estão livres
            // Se o bloco atual e o proximo bloco estao livres, funde-os em um unico bloco
            current_block->size += current_block->next->size + sizeof(struct mem_block);
            current_block->next = current_block->next->next;
        } else {
            current_block = current_block->next;  // caso contrario, anda um bloco na lista
        }
    }
}

// Libera o espaco de memoria previamente utilizado
void sfree(void *p) {
    struct mem_block *current_block = (struct mem_block *)mem_area;
    while (current_block) {
        if (current_block->mem_ptr == p && !current_block->is_free) {  // Checa se e o mesmo endereco de memoria e ve se ele nao ta livre
            // Marca o bloco como livre
            current_block->is_free = 1;
            // Funde blocos livres adjacentes
            coalesce_free_blocks();
            return;
        }
        current_block = current_block->next;  // Anda para o proximo da lista
    }

    printf("O bloco de memória não pode ser liberado.\n");
}

int main() {
    allocate_initial_memory(10000);

    void *p1 = smalloc(500);
    void *p2 = smalloc(1000);
    void *p3 = smalloc(1600);
    void *p4 = smalloc(3000);
    void *p5 = smalloc(2000);
    // sfree(p3);
    // sfree(p4);
    // sfree(p1);
    // sfree(p2);
    // sfree(p5);

    print_mem_block();

    free(mem_area);
    return 0;
}