#ifndef P_BASE_MEMORY_POOL_C
#define P_BASE_MEMORY_POOL_C

#include "pool.h"

#define P_MEMORY_POOL_NODE_SIZE ((Int) sizeof (PMemoryPoolNode))

typedef enum PMemoryPoolFlag
{
    PMemoryPoolFlag_None = 0,
    PMemoryPoolFlag_Free = 1 << 0,
}
PMemoryPoolFlag;

typedef struct PMemoryPoolNode PMemoryPoolNode;

struct PMemoryPoolNode
{
    PMemoryPoolFlag  flag;
    PMemoryPoolNode* list_next;
};

PMemoryPool pMemoryPoolMake(void* pntr, Int size, Int step)
{
    PMemoryPool result;

    pMemorySet(&result, sizeof result, 0xAB);

    if (pntr == NULL || size <= 0 || step <= 0) return result;

    result.pntr_base = (U8*) pntr;
    result.pntr_next = result.pntr_base;
    result.list_head = NULL;
    result.size      = size;

    result.step = pMemoryAlignSize(step, P_MEMORY_DEFAULT_ALIGNMENT);

    pMemorySet(result.pntr_base, result.size, 0xAB);

    return result;
}

void* pMemoryPoolPntr(PMemoryPool* self)
{
    return self->pntr_base;
}

Int pMemoryPoolSize(PMemoryPool* self)
{
    return self->size;
}

void pMemoryPoolClear(PMemoryPool* self)
{
    pMemorySet(self->pntr_base, self->size, 0xAB);

    self->pntr_next = self->pntr_base;
    self->list_head = NULL;
}

void* pMemoryPoolReserve(PMemoryPool* self, Int count, Int size)
{
    if (count <= 0 || size <= 0 || count > P_INT_MAX / size)
        return NULL;

    Int size_header = pMemoryAlignSize(
        P_MEMORY_POOL_NODE_SIZE, P_MEMORY_DEFAULT_ALIGNMENT);

    PMemoryPoolNode* node  = self->list_head;
    Int              bytes = count * size;

    if (bytes < 0 || bytes > self->step) return NULL;

    if (node == NULL) {
        U8* next = ((U8*) self->pntr_next) + size_header + self->step;

        if (next < self->pntr_base || next > self->pntr_base + self->size)
            return NULL;

        node = (PMemoryPoolNode*) self->pntr_next;

        self->pntr_next = next;
    }
    else self->list_head = node->list_next;

    pMemorySet(node, size_header + self->step, 0xAB);

    node->flag      = PMemoryPoolFlag_None;
    node->list_next = NULL;

    return ((U8*) node) + size_header;
}

Bool pMemoryPoolRelease(PMemoryPool* self, void* pntr)
{
    Int size_header = pMemoryAlignSize(
        P_MEMORY_POOL_NODE_SIZE, P_MEMORY_DEFAULT_ALIGNMENT);

    if (pntr == NULL) return 0;

    U8* head = ((U8*) pntr) - size_header;
    Int dist = head - self->pntr_base;

    if (head < self->pntr_base || head >= self->pntr_next)
        return 0;

    if (dist % (size_header + self->step) != 0) return 0;

    PMemoryPoolNode* node = (PMemoryPoolNode*) head;

    if ((node->flag & PMemoryPoolFlag_Free) != 0) return 0;

    pMemorySet(head, size_header + self->step, 0xAB);

    node->flag      = PMemoryPoolFlag_Free;
    node->list_next = self->list_head;
    self->list_head = node;

    return 1;
}

#endif // P_BASE_MEMORY_POOL_C
