***************************
``pl32-file``: ``plfile_t``
***************************

Declaration and Definition
--------------------------

.. code-block:: c

    /* pl32-memory.h declaration */
    typedef struct plfile plfile_t;

    /* pl32-memory.c definition */
    struct plmt {
        FILE* fileptr;
        byte_t* strbuf;
        string_t mode;
        size_t seekbyte;
        size_t bufsize;
        plmt_t* ptrmt;
    };

Explanation
-----------

``plfile_t`` is a structure representing an open file. It can be a pointer to an actual file or a block of memory treated as a file.

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
        string_t buffer = plMTAlloc(mt, 2048);

        /* Write some data (See plfputs.rst) */
        plFPuts(realFile, "writing to the real file");
        plFPuts(memFile, "hewo wowd to the memowy >w<");

        /* Read some data and then print it out (See plfgets.rst) */
        plFGets(buffer, 2047, realFile);
        printf("Real file: %s\n", buffer);
        plFGets(buffer, 2047, memFile);
        printf()

        /* Stop the memory tracker (See pl32-memory/plmtstop.rst) */
        /* NOTE: You can just stop the memory tracker instead of deallocating and then stopping */
        plMTStop(mt);
        return 0;
    }