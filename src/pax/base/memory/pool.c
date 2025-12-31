#ifndef PX_BASE_MEMORY_POOL_C
#define PX_BASE_MEMORY_POOL_C

#include "pool.h"

#define PX_MEMORY_POOL_NODE_SIZE ((ssize) sizeof (PxMemoryPoolNode))

typedef enum PxMemoryPoolFlag
{
    PxMemoryPoolFlag_None = 0,
    PxMemoryPoolFlag_Free = 1 << 0,
}
PxMemoryPoolFlag;

typedef struct PxMemoryPoolNode
{
    PxMemoryPoolFlag flag;

    struct PxMemoryPoolNode* list_next;
}
PxMemoryPoolNode;

PxMemoryPool
pxMemoryPoolMake(void* pntr, ssize size, ssize step)
{
    PxMemoryPool result;

    pxMemorySet(&result, sizeof result, 0xAB);

    if (pntr == PX_NULL || size <= 0 || step <= 0) return result;

    result.pntr_base = (u8*) pntr;
    result.pntr_next = result.pntr_base;
    result.list_head = PX_NULL;
    result.size      = size;

    result.step = pxMemoryAlignSizeForw(step,
        PX_MEMORY_DEFAULT_ALIGNMENT);

    pxMemorySet(result.pntr_base, result.size, 0xAB);

    return result;
}

void*
pxMemoryPoolPntr(PxMemoryPool* self)
{
    return self->pntr_base;
}

ssize
pxMemoryPoolSize(PxMemoryPool* self)
{
    return self->size;
}

void
pxMemoryPoolClear(PxMemoryPool* self)
{
    pxMemorySet(self->pntr_base, self->size, 0xAB);

    self->pntr_next = self->pntr_base;
    self->list_head = PX_NULL;
}

void*
pxMemoryPoolReserve(PxMemoryPool* self, ssize count, ssize size)
{
    ssize size_header = pxMemoryAlignSizeForw(
        PX_MEMORY_POOL_NODE_SIZE, PX_MEMORY_DEFAULT_ALIGNMENT);

    if (count <= 0 || size <= 0 || count > PX_MAX_SSIZE / size - size_header)
        return PX_NULL;

    ssize             bytes = count * size;
    PxMemoryPoolNode* node  = self->list_head;

    if (bytes < 0 || bytes > self->step) return PX_NULL;

    if (node == PX_NULL) {
        u8* next = ((u8*) self->pntr_next) + size_header + self->step;

        if (next < self->pntr_base || next > self->pntr_base + self->size)
            return PX_NULL;

        node            = (PxMemoryPoolNode*) self->pntr_next;
        self->pntr_next = next;
    }
    else self->list_head = node->list_next;

    pxMemorySet(node, size_header + self->step, 0xAB);

    node->flag      = PxMemoryPoolFlag_None;
    node->list_next = PX_NULL;

    return ((u8*) node) + size_header;
}

b32
pxMemoryPoolRelease(PxMemoryPool* self, void* pntr)
{
    ssize size_header = pxMemoryAlignSizeForw(
        PX_MEMORY_POOL_NODE_SIZE, PX_MEMORY_DEFAULT_ALIGNMENT);

    if (pntr == PX_NULL) return 0;

    u8*   head = ((u8*) pntr) - size_header;
    ssize dist = head - self->pntr_base;

    if (head < self->pntr_base || head >= self->pntr_next)
        return 0;

    if (dist % (size_header + self->step) != 0) return 0;

    PxMemoryPoolNode* node = (PxMemoryPoolNode*) head;

    if ((node->flag & PxMemoryPoolFlag_Free) != 0) return 0;

    pxMemorySet(head, size_header + self->step, 0xAB);

    node->flag      = PxMemoryPoolFlag_Free;
    node->list_next = self->list_head;
    self->list_head = node;

    return 1;
}

#endif // PX_BASE_MEMORY_POOL_C
