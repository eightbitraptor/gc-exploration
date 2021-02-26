#include <stdio.h>
#include <stdlib.h>

/*
 * Create an alias called field to an unsigned long. This will make sure that
 * all the fields in our objects are the same length;
 */
typedef unsigned long field;

/* 
 * This is the object in GC Handbook parlance. It's going to consist of a
 * series of 4 slots that are unsigned longs, which may or may not be a
 * reference to another object on the heap
 */
typedef struct object {
    /*
     * These flags provide is with a way of setting information about the
     * object, we could use this to store it's length, or the type etc
     */
    field flags;

    /*
     * An object can be in one of two states: free, or not free.
     */
    union {
        /* When we create a heap we will pre-allocate it with free objects
         * joined together to form a freelist. When we allocate objects in the
         * heap, we will remove them from the freelist; objects that are swept
         * are put back on the freelist so they can be reused
         */
        struct {
            field next;
        } free;

        /* when an object is not free it will have four fields. These are all
         * the same size and may or may not be references to other objects
         */
        struct {
            field v1;
            field v2;
            field v3;
            field v4;
        } values;
    };
} object_t;

#define OBJECT(p) ((object_t *)p)

/*
 * We can set this as the flags of an object when it's removed from the freelist.
 */
#define OBJ_USED 0x01

typedef struct heap {
    field flags;
    /*
     * This is the pointer to the first object in the heap, so that we have an
     * anchor for when we walk the heap;
     */
    object_t *start;

    /*
     * This is a pointer to the first empty slot on the heap - the start of the freelist
     */
    object_t *freelist;
} heap_t;


heap_t *
heap_create(int slots)
{
    // Create the heap, and an object for assignment
    size_t length = slots * sizeof(object_t);
    heap_t *heap = calloc(1, length);
    object_t *p;

    // Make the starting object be the memory directly after the flags
    field start = (field)heap + sizeof(field);

    //calculate where the end of the heap page is based on how many objects
    //it's going to contain
    object_t *end = OBJECT(((field)heap + (slots * sizeof(object_t))));

    //assign the start pointer
    heap->start = (void *)heap;

    // starting at the start pointer, create a number of objects
    for (p = (object_t *)start; p < end; p++){
        // Ensure the flags are all zero
        p->flags = 0;
        // Add it back on to the front of the freelist
        p->free.next = (field)heap->freelist;
        heap->freelist = p;
    }

    return heap;
}

int
main(int argc, char * argv[])
{
    heap_t * heap = heap_create(40);
    
    object_t *obj = heap->start;
    for (int i = 0; i < 40; i++) {
        printf("%i: object at %p with flags %lu", i, (void *)obj, obj->flags);

        if ((object_t *)obj->free.next == NULL) {
            printf("end of free list reached");
            break;
        } else {
            obj = (object_t *)obj->free.next;
        }
    }


    return 0;
}
