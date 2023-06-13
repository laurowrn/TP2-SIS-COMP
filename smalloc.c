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

void print_single_mem_block(struct mem_block *m){
    printf("\n");
    printf("*****************************\n");
    printf("****** Bloco escolhido ******\n");
    printf("*****************************\n");    
    printf("is_free: %d\n", m->is_free);
    printf("size: %ld\n", m->size);
    printf("mem_ptr: %p\n", m->mem_ptr);
    printf("next: %p\n", m->next);
    printf("*****************************\n");
    printf("**-------------------------**\n");
    printf("*****************************\n");
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

void remove_mem_block(void* p) {
    struct mem_block *targetBlock = (struct mem_block *) p;
    struct mem_block *previousBlock = mem_area;
    struct mem_block *nextBlock;
    struct mem_block *currentBlock =  mem_area;
    int index = 0;
    while (currentBlock->mem_ptr != targetBlock->mem_ptr){
        currentBlock = currentBlock->next;
        index++;
    }

    for (int i = 0; i < index - 1; i++){
        previousBlock = previousBlock->next;
    }

    nextBlock = currentBlock->next;

    previousBlock->next = nextBlock;
    free(currentBlock);
}

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
    struct mem_block *targetBlock = (struct mem_block *) p;
    struct mem_block *previousBlock = mem_area;
    struct mem_block *nextBlock;
    struct mem_block *currentBlock =  mem_area;
    int index = 0;
    while (currentBlock->mem_ptr != targetBlock->mem_ptr){
        currentBlock = currentBlock->next;
        index++;
    }

    if (currentBlock->is_free == 0){
        for (int i = 0; i < index - 1; i++){
            previousBlock = previousBlock->next;
        }

        nextBlock = currentBlock->next;
        int freeMemory = currentBlock->size;

        if(previousBlock->is_free){
            freeMemory += previousBlock->size;
            remove_mem_block((void *) previousBlock);
        }
        if(nextBlock->is_free){
            freeMemory += nextBlock->size;
            remove_mem_block((void *) nextBlock);
        }

        currentBlock->is_free = 1;
        currentBlock->size = freeMemory;

    }else{
        printf("O bloco já está livre!\n");
    }
}


int main(){
    mem_area = (struct mem_block *) malloc(sizeof(struct mem_block));
    mem_area->is_free = 1;
    mem_area->size = 8192;
    mem_area->mem_ptr = (void *) mem_area;
    mem_area->next = NULL;

    print_mem_block(mem_area);

    struct mem_block *p1 = (struct mem_block *) smalloc(200);
    struct mem_block *p2 = (struct mem_block *) smalloc(300);
    struct mem_block *p3 = (struct mem_block *) smalloc(1000);
    struct mem_block *p4 = (struct mem_block *) smalloc(5000);

    print_mem_block(mem_area);

    sfree((void *) p2);
    print_mem_block(mem_area);

    sfree((void *) p3);
    print_mem_block(mem_area);

    sfree((void *) p4);
    print_mem_block(mem_area);

    sfree((void *) p1);
    print_mem_block(mem_area);

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