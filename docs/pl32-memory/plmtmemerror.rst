********************************
``pl32-memory``: ``plMTMemError``
********************************

Declaration and Definition
--------------------------

.. code-block:: c

    /* pl32-memory.c declaration and definition */
    void plMTMemError(){
        printf("Error: Out of memory\n");
        abort();
    }


Explanation
-----------

``plMTMemError`` is an internal function that is used by |plMTAllocE|_ when the call to |plMTAlloc|_ returns a null, or when |plMTManage|_ has an internal error

Usage Example
-------------

Since this is an internal function, no example usage is available for this function

.. |plMTAllocE| replace:: ``plMTAllocE``
.. |plMTAlloc| replace:: ``plMTAlloc``
.. |plMTManage| replace:: ``plMTManage``

.. _plMTAllocE: plmtalloc.rst
.. _plMTAlloc: plmtalloc.rst
.. _plMTManage: plmtmanage.rst