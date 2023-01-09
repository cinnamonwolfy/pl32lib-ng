*******************************
``pl32-memory``: ``plMTManage``
*******************************

Declaration and Definition
--------------------------

.. code-block:: c

    /* pl32-memory.c declaration and definition */
    void plMTManage(plmt_t* mt, int mode, memptr_t ptr, size_t size, memptr_t ptr2){
        /* internal library code */
    }


Explanation
-----------

``plMTManage`` is an internal library function and it's the core of the memory tracker. It handles any and all memory allocation tracking, such as adding, removing or editing pointer entries in the memory tracker

Usage Example
-------------

Since this is an internal function, no example usage is available for this function