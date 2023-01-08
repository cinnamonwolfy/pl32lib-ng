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

        /* Open two files, one real, one in memory */
        plfile_t* realFile = plFOpen("path/to/file", "r+");
        plfile_t* memFile = plFOpen(NULL, "w+");

        /* Stop the memory tracker (See plmtstop.rst) */
        /* NOTE: You can just stop the memory tracker instead of deallocating and then stopping */
        plMTStop(mt);
        return 0;
    }