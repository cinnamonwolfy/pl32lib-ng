******************************
``pl32-token``: ``plTokenize``
******************************

Declaration
-----------

.. code-block:: c

    /* pl32-token.h declaration */
    string_t plTokenize(string_t string, string_t* leftoverStr, plmt_t* mt);


Explanation
-----------

``plTokenize`` is a tokenizer designed to mimic the tokenization of a shell interpreter or a TOML parser. It supports basic and literal strings and just normal tokens separated by spaces. It uses |plStrtok|_ internally, and thus generates tokens in a similar way to it. This is the most used part of pl32lib-ng in my other projects (plinterpretlib, plml-parselib)

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

        tokens[0] = plTokenize(string, &holder, mt);
        tokens[1] = plTokenize(holder, &holder, mt);

        printf("Token 1: %s\n", tokens[0]);
        printf("Token 2: %s\n", tokens[1]);

        /* Deallocate the array (See pl32-memory/plmtfree.rst) */
        plMTFree(mt, tokens[0]);
        plMTFree(mt, tokens[1]);

        /* Stop the memory tracker (See pl32-memory/plmtstop.rst) */
        /* NOTE: You can just stop the memory tracker instead of deallocating and then stopping */
        plMTStop(mt);
        return 0;
    }

.. |plStrtok| replace:: ``plStrtok``

.. _plStrtok: plstrtok.rst