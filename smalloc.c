#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#pragma pack(push, 1)
struct mem_block {
    int is_free;   // Se o bloco esta livre 1 senao 0.
    size_t size;   // Tamanho do bloco de memoria.
    void* mem_ptr; // Ponteiro para o bloco de memoria.
    struct mem_block* next; // Ponteiro para o proximo bloco na lista.
};
#pragma pack(pop)

struct mem_block *mem_area;

void* smalloc(size_t size) {
    struct mem_block *m = mem_area;
    struct mem_block *foundBlock = NULL;
    struct mem_block *nextBlock;
    while(m){
        if(m->size > (size + sizeof(struct mem_block)) && m->is_free == 1){
            foundBlock = m;
            nextBlock = m->next;
            break;
        }
        m = m->next;
    }


    if(foundBlock){
        struct mem_block *newBlock = (struct mem_block *) malloc(sizeof(struct mem_block));
        newBlock->is_free = 1;
        newBlock->mem_ptr = (void *) newBlock;
        newBlock->size = foundBlock->size - (size + (int) sizeof(struct mem_block));
        newBlock->next = nextBlock;

        foundBlock->is_free = 0;
        foundBlock->size = size + (int )sizeof(struct mem_block);
        foundBlock->next = newBlock;

        return (void *) foundBlock;
    }else{
        printf("Memoria cheia! Nenhum bloco suficientemente grande encontrado.\n");

        struct mem_block *nullBlock = (struct mem_block *) malloc(sizeof(struct mem_block));
        nullBlock->is_free = 0;
        nullBlock->size = 0;
        nullBlock->mem_ptr = NULL;
        nullBlock->next = NULL;

        return (void *) nullBlock;
    }
}


void sfree(void* p) {

}


void print_mem_block(struct mem_block *m){
    printf("\n");
    printf("-----------------------------\n");
    printf("------Comeco da memoria------\n");
    printf("-----------------------------\n");    
    while(m){
        printf("is_free: %d\n", m->is_free);
        printf("size: %ld\n", m->size);
        printf("mem_ptr: %p\n", m->mem_ptr);
        printf("next: %p\n", m->next);
        printf("-----------------------------\n");
        m = m->next;
    }
    printf("--------Fim da memoria-------\n");
    printf("-----------------------------\n");  
};


int main(){
    mem_area = (struct mem_block *) malloc(sizeof(struct mem_block));
    mem_area->is_free = 1;
    mem_area->size = 8192;
    mem_area->mem_ptr = (void *) mem_area;
    mem_area->next = NULL;

    // struct mem_block *second_block = (struct mem_block *) malloc(sizeof(struct mem_block));
    // second_block->is_free = 1;
    // second_block->size = 8192;
    // second_block->mem_ptr = (void *) malloc(sizeof(void));
    // second_block->next = NULL;

    // mem_area->next = second_block;

    print_mem_block(mem_area);

    smalloc(200);
    smalloc(300);
    smalloc(1000);
    smalloc(5000);
    smalloc(5000);

    print_mem_block(mem_area);

    // printf("%d", (int) sizeof(struct mem_block));


    // print_mem_block(foundBlock);

    /* Exemplo de uso do smalloc(), sfree() e print_mem_block() */
    // char *p1 = (char*) smalloc(200);
    // char *p2 = (char*) smalloc(100);
    // char *p3 = (char*) smalloc(300);

    // print_mem_block((struct mem_block*) mem_area);        

    // sfree(p3);
    // sfree(p2);

    // print_mem_block((struct mem_block*) mem_area);            

    return 0;
}