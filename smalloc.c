#include<stdio.h>
#include <unistd.h>

#pragma pack(push, 1)
struct mem_block {
    int is_free;   // Se o bloco esta livre 1 senao 0.
    size_t size;   // Tamanho do bloco de memoria.
    void* mem_ptr; // Ponteiro para o bloco de memoria.
    struct mem_block* next; // Ponteiro para o proximo bloco na lista.
};
#pragma pack(pop)


void* smalloc(size_t size) {

    return NULL;
}


void sfree(void* p) {

}


void print_mem_block(struct mem_block *m){
    printf("-----------------------------\n");    
    while(m){

        printf("is_free: %d\n", m->is_free);
        printf("size: %ld\n", m->size);
        printf("mem_ptr: %p\n", m->mem_ptr);
        printf("next: %p\n", m->next);
        printf("-----------------------------\n");
        m = m->next;
    }
};


int main(){
    /* Exemplo de uso do smalloc(), sfree() e print_mem_block() */
    char *p1 = (char*) smalloc(200);
    char *p2 = (char*) smalloc(100);
    char *p3 = (char*) smalloc(300);

    print_mem_block((struct mem_block*) mem_area);        

    sfree(p3);
    sfree(p2);

    print_mem_block((struct mem_block*) mem_area);            

    return 0;
}


