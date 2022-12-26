******************************
``pl32-memory``: ``plarray_t``
******************************

Declaration and Definition
--------------------------

.. code-block:: c

    /* pl32-memory.h declaration and definition */
    typedef struct plarray {
        memptr_t array;
        size_t size;
        bool isMemAlloc;
        plmt_t* mt;
    }

Explanation
-----------

``plarray_t`` is a structure meant to represent an array, whether dynamically
allocated through plmt or statically allocated at compile time. This structure
helps create more memory-safe programs, as it can prevent
buffer overflows/overruns if used properly.

It consists of a memory pointer to an array, a number containing the size of the
array, a boolean which tells the callee function that this structure instance is dynamically allocated and a pointer to a memory tracker instance in case deallocation
is necessary.

Due to ``plarray_t`` being directly defined in the header, creating a
``plarray_t`` can only be done manually. A ``plarray_t`` creation function might
be added in a new version to pl32lib, although I do not see it necessary.

Usage Example
-------------

.. code-block:: c

    #include <pl32.h>

    void printIntArray(plarray_t* safeArray){
        /* Do stuff */
        printf("Here is a list of coords in (x, y) form that represent a quadrilateral:\n\n");
        for(int i = 0; i < safeArray->size; i++){
            printf("Point %d: (%d, %d)\n", i + 1, ((int*)safeArray)[i][0], ((int*)safeArray)[i][1]);
        }

        if(safeArray->mt != NULL){
            int* array = safeArray->array;
            for(int i = 0; i < safeArray->size; i++){
                plMTFree(array[i]);
            }
            plMTFree(array);
        }
    }

    /* Makes the code less messy */
    void setDynIntArray(int* array, size_t offset, int x, int y){
        array[offset][0] = x;
        array[offset][1] = y;
    }

    int main(int argc, string_t argv[]){
        /* Creates a memory tracker for the dynamically allocated array (See plmt.rst and plmtalloc.rst) */
        plmt_t* mt = plMTInit(1024 * 1024);

        /* Creates the test arrays */
        int staticArray[4][2] = { {4, 2 }, {10, 7 }, {-5, 8 }, { -8, 0 } };
        int* dynamicArray = plMTAlloc(mt, 4 * sizeof(int*));
        for(int i = 0; i < 4; i++)
            dynamicArray[i] = plMTAlloc(mt, 2 * sizeof(int));
        setDynIntArray(dynamicArray, 0, 5, 1);
        setDynIntArray(dynamicArray, 1, 10, 5);
        setDynIntArray(dynamicArray, 2, -2, 9);
        setDynIntArray(dynamicArray, 3, 0, 0);

        /* Creates a memory-safe array */
        plarray_t safeArray;

        /* Set safeArray up as a static array */
        safeArray.array = staticArray;
        safeArray.size = 4;
        safeArray.isMemAlloc = false;
        safeArray.mt = NULL;

        /* Print out the contents of the integer array */
        printIntArray(&safeArray);

        /* Set safeArray up as a dynamically allocated array */
        safeArray->array = dynamicArray;
        safeArray->size = 4;
        safeArray->isMemAlloc = false;
        safeArray->mt = mt;

        /* Print out the contents of the integer array again */
        printIntArray(&safeArray);

        return 0;
    }