****************************
``pl32-memory``: ``plFOpen``
****************************

Declaration
-----------

.. code-block:: c

    /* pl32-file.h declaration */
    plfile_t* plFOpen(string_t filename, string_t mode, plmt_t mt);


Explanation
-----------

``plFOpen`` opens file ``filename`` in ``mode`` file mode. If ``filename`` equals ``NULL``, it will create a file handle that points to a memory buffer.

Usage Example
-------------

.. code-block:: c

    