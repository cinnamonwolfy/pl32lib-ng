*****************************
``pl32-memory``: ``plMTInit``
*****************************

Declaration
-----------

.. code-block:: c

    /* pl32-memory.h declaration */
    plmt_t* plMTInit(size_t maxMemoryInit);


Explanation
-----------

``plMTInit`` creates and initializes a memory tracker (See |plmt_t|_ for more information)

Usage Example
-------------

.. code-block:: c

    #include <pl32.h>

    int main(int argc, string_t argv[]){
        /* Creates a memory tracker with a maximum size of 1MiB (See plmt.rst)*/
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

        /* Deallocate the array (See plmtalloc.rst) */
        plMTFree(mt, intArray);

        /* Stop the memory tracker (See plmtstop.rst) */
        /* NOTE: You can just stop the memory tracker instead of deallocating and then stopping */
        plMTStop(mt);
        return 0;
    }


.. |plmt_t| replace:: ``plmt_t``

.. _`plmt_t`: plmt.rst
