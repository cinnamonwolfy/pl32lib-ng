**************************************
``pl32-memory``: ``plMTMemAmnt``
**************************************

Declaration
-----------

.. code-block:: c

    /* pl32-memory.h definition */
    plmt_t* plMTMemAmnt(plmt_t* mt, int action, size_t size);


Explanation
-----------

``plMTMemAmnt`` gets the current memory usage and maximum memory allocation limit, and also sets the new maximum memory allocation limit of a memory tracker (See |plmt_t|_ for more information)

It uses ``PLMT_GET_USEDMEM``, ``PLMT_GET_MAXMEM``, and ``PLMT_SET_MAXMEM`` for the ``action`` constants

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

        /* Get memory tracker's current usage and then print it*/
        size_t mtSize = plMTMemAmnt(mt, PLMT_GET_USEDMEM, 0);
        printf("Current memory usage: %d\n", mtSize);

        /* Deallocate the array (See plmtalloc.rst) */
        plMTFree(mt, intArray);

        /* Stop the memory tracker (See plmtstop.rst) */
        /* NOTE: You can just stop the memory tracker instead of deallocating and then stopping */
        plMTStop(mt);
        return 0;
    }


.. |plmt_t| replace:: ``plmt_t``

.. _`plmt_t`: plmt.rst
