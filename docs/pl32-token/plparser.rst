******************************
``pl32-token``: ``plParser``
******************************

Declaration
-----------

.. code-block:: c

    /* pl32-token.h declaration */
    plarray_t plParser(string_t input, plmt_t* mt);


Explanation
-----------

``plParser`` is a full tokenizer built on top of |plTokenize|_ that is meant to be used by a shell interpreter or markup language parser. It generates all of the tokens and puts them in a memory-allocated |plarray_t|_

Usage Example
-------------

.. code-block:: c

    #include <pl32.h>

    int main(int argc, string_t argv[]){
        /* Creates a memory tracker with a maximum size of 1MiB (See pl32-memory/plmt.rst)*/
        plmt_t* mt = plMTInit(1024 * 1024);

        /* Setup variables */
        byte_t string[16] = "hello, world!";
        plarray_t* tokens = plParser(string, mt);
        string_t tokensStr = tokens->array;

        printf("Token 1: %s\n", tokensStr[0]);
        printf("Token 2: %s\n", tokensStr[1]);

        /* Deallocate the array */
        plMTFreeArray(mt, );

        /* Stop the memory tracker (See plmtstop.rst) */
        /* NOTE: You can just stop the memory tracker instead of deallocating and then stopping */
        plMTStop(tokens, true);
        return 0;
    }

.. |plTokenize| replace:: ``plTokenize``
.. |plarray_t| replace:: ``plarray_t``

.. _plarray_t: ../pl32-memory/plarray.rst
.. _plTokenize: pltokenize.rst