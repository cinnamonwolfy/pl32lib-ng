***************************
``pl32-memory``: ``plmt_t``
***************************

Declaration and Definition
--------------------------

.. code-block:: c

    /* pl32-memory.h declaration */
    typedef struct plmt plmt_t;

    /* pl32-memory.c definition */
    struct plmt {
        plptr_t* ptrList;
        size_t listAmnt;
        size_t allocListAmnt;
        size_t usedMemory;
        size_t maxMemory;
    };

Explanation
-----------

``plmt_t`` is the memory tracker structure. This structure is the core of
pl32lib-ng. Every module within pl32lib-ng uses this structure, and by proxy, so
does every pl32lib-ng program.

It consists of a pointer buffer, where it stores all of the pointers that were
obtained through the ``malloc`` suite of functions. The tracker keeps track of
the pointer buffer size and how many entries are currently taken. It also keeps
track of the currently used memory and the maximum amount of memory it is
allowed to allocate, in bytes.

Due to ``plmt_t`` being an opaque declaration, it can only be created with the
``plMTInit`` function. After you're done using the memory tracker, it can only
be properly deallocated with the ``plMTStop`` function.

Usage Example
-------------

.. code-block:: c

    #include <pl32.h>

    int main(int argc, string_t argv[]){
        /* Creates a memory tracker with a maximum size of 1MiB (See plmtinit.rst)*/
        plmt_t* mt = plMTInit(1024 * 1024);

        /* Allocates some memory to an integer array (See plmtalloc.rst) */
        int* intArray;
        intArray = plMTAlloc(mt, 4 * sizeof(int));

        /* Do some stuff */
        intArray[0] = 10;
        intArray[1] = 20;
        intArray[2] = 15;
        intArray[3] = 25;

        printf("Here's a bunch of random numbers: %d, %d, %d, %d\n", intArray[0], intArray,[1], intArray[2], intArray[3]);

        /* Deallocate the array (See plmtalloc.rst)*/
        plMTFree(mt, intArray);

        /* Stop the memory tracker (See plmtstop.rst) */
        /* NOTE: You can just stop the memory tracker instead of deallocating and then stopping */
        plMTStop(mt);
        return 0;
    }