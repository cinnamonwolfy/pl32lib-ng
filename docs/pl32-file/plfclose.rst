***************************
``pl32-file``: ``plFClose``
***************************

Declaration
-----------

.. code-block:: c

    /* pl32-file.h declaration */
    void plFClose(plfile_t* ptr);


Explanation
-----------

``plFClose`` closes an open file handle. If it's a file in memory, it just frees all of the memory it has dynamiaclly allocated. If it points to an actual file, it closes the file and then frees any memory it has dynamically allocated

Usage Example
-------------

.. code-block:: c

    #include <pl32.h>

    int main(int argc, string_t argv[]){
        /* Creates a memory tracker with a maximum size of 1MiB (See pl32-memory/plmtinit.rst)*/
        plmt_t* mt = plMTInit(1024 * 1024);

        /* Open two files, one real, one in memory (See plfopen.rst) */
        plfile_t* realFile = plFOpen("path/to/file", "r+", mt);
        plfile_t* memFile = plFOpen(NULL, "w+", mt);
        string_t buffer = plMTAlloc(mt, 2048); /* Allocate a buffer of 2048 bytes (See pl32-memory/plmtalloc.rst) */

        /* Write some data (See plfputs.rst) */
        plFPuts(realFile, "writing to the real file");
        plFPuts(memFile, "hewo wowd to the memowy >w<");

        /* Read some data and then print it out (See plfgets.rst) */
        plFGets(buffer, 2047, realFile);
        printf("Real file: %s\n", buffer);
        plFGets(buffer, 2047, memFile);
        printf("In-memory file: %s\n", buffer);

        /* Close the files */
        plFClose(realFile);
        plFClose(memFile);
        plMTFree(mt, buffer) /* Free the buffer (See pl32-memory/plmtalloc.rst) */

        /* Stop the memory tracker (See pl32-memory/plmtstop.rst) */
        /* NOTE: You can just stop the memory tracker instead of deallocating and then stopping */
        plMTStop(mt);
        return 0;
    }

.. |plmt_t| replace:: ``plmt_t``
.. _plmt_t: ../pl32-memory/plmt.rst