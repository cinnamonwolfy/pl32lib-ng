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