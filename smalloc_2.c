#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *mem_area;

#pragma pack(push, 1)
struct mem_block
{
    int is_free;            // Se o bloco esta livre 1 senao 0.
    size_t size;            // Tamanho do bloco de memoria.
    void *mem_ptr;          // Ponteiro para o bloco de memoria.
    struct mem_block *next; // Ponteiro para o proximo bloco na lista.
};
#pragma pack(pop)

void allocate_initial_memory(int initial_size)
{
    initial_size += sizeof(struct mem_block);
    mem_area = (void *)malloc(initial_size);

    void *current_block = mem_area;
    struct mem_block *current_block_content = (struct mem_block *)current_block;
    current_block_content->is_free = 1;
    current_block_content->size = initial_size - sizeof(struct mem_block);
    current_block_content->mem_ptr = current_block + sizeof(struct mem_block);
    current_block_content->next = NULL;
}

void print_mem_block()
{
    void *current_block;
    current_block = mem_area;

    // Cabeçalho do primeiro bloco de memória
    struct mem_block *current_block_content = (struct mem_block *)current_block;

    printf("\n");
    printf("-----------------------------\n");
    printf("------Comeco da memoria------\n");
    printf("-----------------------------\n");
    while (current_block_content)
    {
        printf("is_free: %d\n", current_block_content->is_free);
        printf("size: %ld\n", current_block_content->size);
        printf("mem_ptr: %p\n", current_block_content->mem_ptr);
        printf("next: %p\n", current_block_content->next);
        printf("-----------------------------\n");
        current_block_content = current_block_content->next;
    }
    printf("--------Fim da memoria-------\n");
    printf("-----------------------------\n");
};

void *smalloc(size_t size)
{
    void *current_block;
    current_block = mem_area;
    struct mem_block *current_block_content = (struct mem_block *)current_block;
    struct mem_block *found_block_content;
    while (current_block_content)
    {
        if (current_block_content->size > (size + sizeof(struct mem_block)) && current_block_content->is_free)
        {
            found_block_content = current_block_content;
            break;
        }
        current_block_content = current_block_content->next;
    }

    if (found_block_content == NULL)
    {
        printf("Nenhum bloco livre encontrado!\n");
        return NULL;
    }

    void *next_block;
    next_block += sizeof(struct mem_block) + size;
    struct mem_block *next_block_content = (struct mem_block *)next_block;
    next_block_content->is_free = 1;
    next_block_content->mem_ptr = next_block + sizeof(struct mem_block);
    next_block_content->next = found_block_content->next;
    next_block_content->size = found_block_content->size - sizeof(struct mem_block) - size;

    found_block_content->is_free = 0;
    found_block_content->mem_ptr = (void *)(found_block_content + sizeof(struct mem_block));
    found_block_content->next = next_block;
    found_block_content->size = size;

    return found_block_content->mem_ptr;
}

void remove_mem_block(struct mem_block *p)
{
    struct mem_block *targetBlock = p;
    struct mem_block *previousBlock = mem_area;
    struct mem_block *nextBlock;
    struct mem_block *currentBlock = mem_area;
    int index = 0;
    while (currentBlock->mem_ptr != targetBlock->mem_ptr)
    {
        currentBlock = currentBlock->next;
        index++;
    }

    for (int i = 0; i < index - 1; i++)
    {
        previousBlock = previousBlock->next;
    }

    nextBlock = currentBlock->next;

    previousBlock->next = nextBlock;
    free(currentBlock);
}

void sfree(void *p)
{
    struct mem_block *current_block = (struct mem_block *)mem_area;
    struct mem_block *previous_block = (struct mem_block *)mem_area;
    struct mem_block *next_block;

    // struct mem_block *previousBlock;
    // struct mem_block *nextBlock;

    int index = 0;
    while (current_block->mem_ptr != p)
    {
        current_block = current_block->next;
        index++;
    }

    if (current_block->is_free == 0)
    {
        for (int i = 0; i < index - 1; i++)
        {
            previous_block = previous_block->next;
        }

        next_block = current_block->next;
        int free_memory = current_block->size + sizeof(struct mem_block);

        if (previous_block->is_free)
        {
            free_memory += previous_block->size + sizeof(struct mem_block);
            remove_mem_block(previous_block);
        }
        if (next_block->is_free)
        {
            free_memory += next_block->size + sizeof(struct mem_block);
            // remove_mem_block(next_block);
        }

        current_block->is_free = 1;
        current_block->size = free_memory;
        current_block->mem_ptr = (void *)(current_block + sizeof(struct mem_block));
    }
    else
    {
        printf("O bloco já está livre!\n");
    }
}

int main()
{
    allocate_initial_memory(10000);

    void *p1 = smalloc(100);
    void *p2 = smalloc(200);
    void *p3 = smalloc(500);
    sfree(p2);
    sfree(p1);
    print_mem_block();

    return 0;
}