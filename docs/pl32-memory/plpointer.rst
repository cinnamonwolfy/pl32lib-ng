****************************
``pl32-memory``: ``plptr_t``
****************************

Declaration and Definition
--------------------------

.. code-block:: c

    /* pl32-memory.c declaration and definition */
    typedef struct plpointer {
        memptr_t pointer;
        size_t size;
    } plptr_t;

Explanation
-----------

``plptr_t`` is an internal type used by the ``plmt_t`` structure as an easy way
to keep track of all dynamically allocated memory buffer pointer points. It is a
simple struct, reminiscent of early ``plarray_t``.


Usage Example
-------------

Since this is only used internally, there is no example usage for this type