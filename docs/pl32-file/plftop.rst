*************************
``pl32-file``: ``plFToP``
*************************

Declaration
-----------

.. code-block:: c

    /* pl32-file.h declaration */
    plfile_t* plFToP(FILE* pointer, string_t mode, plmt_t* mt);


Explanation
-----------

``plFToP`` converts an existing Standard C file handle into a ``pl32lib-ng`` file handle. The ``mode`` parameter doesn't need to have a meaningful value, as I'm only keeping it for API compatibility with version 1.00 of ``pl32lib-ng``. The |plmt_t|_ value cannot be ``NULL``

Usage Example
-------------

.. code-block:: c

    #include <pl32.h>

    int main(int argc, string_t argv[]){
        /* Creates a memory tracker with a maximum size of 1MiB (See pl32-memory/plmtinit.rst)*/
        plmt_t* mt = plMTInit(1024 * 1024);

        /* Open a file using Standard C Library function fopen() */
        FILE* filePtr = fopen("path/to/file");

        /* Convert a FILE* into a plfile_t* */
        plfile_t* realFile = plFToP(filePtr, NULL, mt);
        string_t buffer = plMTAlloc(mt, 2048); /* Allocate a buffer of 2048 bytes (See pl32-memory/plmtalloc.rst) */

        /* Write some data (See plfputs.rst) */
        plFPuts(realFile, "writing to the real file");

        /* Read some data and then print it out (See plfgets.rst) */
        plFGets(buffer, 2047, realFile);
        printf("Real file: %s\n", buffer);

        /* Close the file (See plfclose.rst) */
        plFClose(realFile);
        plMTFree(mt, buffer) /* Free the buffer (See pl32-memory/plmtalloc.rst) */

        /* Stop the memory tracker (See pl32-memory/plmtstop.rst) */
        /* NOTE: You can just stop the memory tracker instead of deallocating and then stopping */
        plMTStop(mt);
        return 0;
    }

.. |plmt_t| replace:: ``plmt_t``
.. _plmt_t: ../pl32-memory/plmt.rst