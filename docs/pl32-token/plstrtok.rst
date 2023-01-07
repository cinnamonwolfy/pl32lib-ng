****************************
``pl32-token``: ``plStrtok``
****************************

Declaration
-----------

.. code-block:: c

    /* pl32-token.h declaration */
    string_t plStrtok(string_t string, string_t delimiter, string_t* leftoverStr, plmt_t* mt);


Explanation
-----------

``plStrtok`` is an implementation of Standard C library function ``strtok`` that returns memory-allocated strings instead of pointers to strings. This prevents common memory bugs caused by Standard C ``strtok`` (these are usually caused by developers not knowing that ``strtok`` has an internal buffer that it rewrites every single call, and that this buffer is the one being returned)

Usage Example
-------------

.. code-block:: c

    #include <pl32.h>

    int main(int argc, string_t argv[]){
        /* Creates a memory tracker with a maximum size of 1MiB (See pl32-memory/plmt.rst)*/
        plmt_t* mt = plMTInit(1024 * 1024);

        /* Setup variables */
        byte_t string[16] = "hello, world!";
        string_t tokens[2];
        string_t holder;

        tokens[0] = plStrtok(string, " ", &holder, mt);
        tokens[1] = plStrtok(holder, " ", &holder, mt);

        printf("Token 1: %s\n", tokens[0]);
        printf("Token 2: %s\n", tokens[1]);

        /* Deallocate the array */
        plMTFree(mt, tokens[0]);
        plMTFree(mt, tokens[1]);

        /* Stop the memory tracker (See plmtstop.rst) */
        /* NOTE: You can just stop the memory tracker instead of deallocating and then stopping */
        plMTStop(mt);
        return 0;
    }