#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *mem_area;

#pragma pack(push, 1)
struct mem_block {
    int is_free;             // Se o bloco esta livre (1) ou não (0).
    size_t size;             // Tamanho do bloco de memória.
    void *mem_ptr;           // Ponteiro para o bloco de memória.
    struct mem_block *next;  // Ponteiro para o próximo bloco na lista.
};
#pragma pack(pop)

void allocate_initial_memory(int initial_size) {
    initial_size += sizeof(struct mem_block);
    mem_area = (void *)malloc(initial_size);

    void *current_block = mem_area;
    struct mem_block *current_block_content = (struct mem_block *)current_block;
    current_block_content->is_free = 1;
    current_block_content->size = initial_size - sizeof(struct mem_block);
    current_block_content->mem_ptr = current_block + sizeof(struct mem_block);
    current_block_content->next = NULL;
}

void print_mem_block() {
    void *current_block;
    current_block = mem_area;

    // Cabeçalho do primeiro bloco de memória
    struct mem_block *current_block_content = (struct mem_block *)current_block;

    printf("\n");
    printf("-----------------------------\n");
    printf("------Comeco da memoria------\n");
    printf("-----------------------------\n");
    while (current_block_content) {
        printf("is_free: %d\n", current_block_content->is_free);
        printf("size: %ld\n", current_block_content->size);
        printf("mem_ptr: %p\n", current_block_content->mem_ptr);
        printf("next: %p\n", current_block_content->next);
        printf("-----------------------------\n");
        current_block_content = current_block_content->next;
    }
    printf("--------Fim da memoria-------\n");
    printf("-----------------------------\n");
}

void *smalloc(size_t size) {
    void *current_block = mem_area;

    struct mem_block *current_block_content = (struct mem_block *)current_block;
    struct mem_block *found_block_content = NULL;

    while (current_block_content) {
        if (current_block_content->is_free && current_block_content->size >= size) {
            found_block_content = current_block_content;
            break;
        }
        current_block_content = current_block_content->next;
    }

    if (found_block_content == NULL) {
        printf("Memória insuficiente! Não é possível alocar um bloco com %ld bytes.\n", size);
        return NULL;
    }

    // Verifica se o tamanho do bloco atual permite a criação de um novo bloco livre
    if (found_block_content->size > size + sizeof(struct mem_block)) {
        // Calcula o endereço do novo bloco livre
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
        // Caso o tamanho do bloco seja exatamente o necessário, apenas marca o bloco como alocado
        found_block_content->is_free = 0;
    }

    return found_block_content->mem_ptr;
}

void coalesce_free_blocks() {
    struct mem_block *current_block = (struct mem_block *)mem_area;
    
    while (current_block && current_block->next) {
        if (current_block->is_free && current_block->next->is_free) {
            // Se o bloco atual e o próximo bloco estão livres, funde-os em um único bloco
            current_block->size += current_block->next->size + sizeof(struct mem_block);
            current_block->next = current_block->next->next;
        } else {
            current_block = current_block->next;
        }
    }
}

void sfree(void *p) {
    struct mem_block *current_block = (struct mem_block *)mem_area;
    while (current_block) {
        if (current_block->mem_ptr == p && !current_block->is_free) {
            // Marca o bloco como livre
            current_block->is_free = 1;
            // Funde blocos livres adjacentes
            coalesce_free_blocks();
            return;
        }
        current_block = current_block->next;
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
    //sfree(p3);
    //sfree(p4);
    sfree(p1);
    sfree(p2);
    //sfree(p5);
    void *p6 = smalloc(1600);
    // void *p7 = smalloc(1600);

    print_mem_block();

    free(mem_area);
    return 0;
}