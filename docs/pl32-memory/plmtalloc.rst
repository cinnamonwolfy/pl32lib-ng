************************************
``pl32-memory``: ``plMTAlloc`` suite
************************************

Declarations
------------

.. code-block:: c

    /* pl32-memory.h declarations */
    memptr_t plMTAlloc(plmt_t* mt, size_t size);
    memptr_t plMTAllocE(plmt_t* mt, size_t size);
    memptr_t plMTCalloc(plmt_t* mt, size_t amount, size_t size);
    memptr_t plMTRealloc(plmt_t* mt, memptr_t pointer, size_t size);
    void plMTFree(plmt_t* mt, memptr_t pointer);

Explanation
-----------

The ``plMTAlloc`` suite of functions are wrappers around the Standard C99 ``malloc`` suite of functions. Just like the ``malloc`` suite, it allocates memory, but it also keeps track of all of the pointers to allocated memory. This can be used as a debugging tool to get rid of memory leaks or as a very simple per-function semi-automatic garbage collector. This is the main way of using the memory tracker.

Usage Example
-------------

.. code-block:: c

    #include <pl32.h>

    int main(int argc, string_t argv[]){
        /* Creates a memory tracker with a maximum size of 1MiB (See plmtinit.rst)*/
        plmt_t* mt = plMTInit(1024 * 1024);

        /* Allocates some memory to an integer array */
        int* intArray;
        intArray = plMTAlloc(mt, 4 * sizeof(int));

        /* Do some stuff */
        intArray[0] = 10;
        intArray[1] = 20;
        intArray[2] = 15;
        intArray[3] = 25;

        printf("Here's a bunch of random numbers: %d, %d, %d, %d\n", intArray[0], intArray,[1], intArray[2], intArray[3]);

        /* Deallocate the array */
        plMTFree(mt, intArray);

        /* Stop the memory tracker (See plmtstop.rst) */
        /* NOTE: You can just stop the memory tracker instead of deallocating and then stopping */
        plMTStop(mt);
        return 0;
    }
