**********************************
``pl32-memory``: ``plMTFreeArray``
**********************************

Declaration
-----------

.. code-block:: c

    void plMTFreeArray(plarray_t* array, bool is2DArray);

Explanation
-----------

``plMTFreeArray`` frees a dynamically allocated array (See |plarray_t|_)

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

        plMTFreeArray(array, true);
    }

    /* Makes the code less messy */
    void setDynIntArray(int* array, size_t offset, int x, int y){
        array[offset][0] = x;
        array[offset][1] = y;
    }

    int main(int argc, string_t argv[]){
        /* Creates a memory tracker for the dynamically allocated array (See plmt.rst and plmtalloc.rst) */
        plmt_t* mt = plMTInit(1024 * 1024);

        /* Create the test array */
        int* dynamicArray = plMTAlloc(mt, 4 * sizeof(int*));
        for(int i = 0; i < 4; i++)
            dynamicArray[i] = plMTAlloc(mt, 2 * sizeof(int));
        setDynIntArray(dynamicArray, 0, 5, 1);
        setDynIntArray(dynamicArray, 1, 10, 5);
        setDynIntArray(dynamicArray, 2, -2, 9);
        setDynIntArray(dynamicArray, 3, 0, 0);

        /* Creates a memory-safe array */
        plarray_t* safeArray = plMTAlloc(mt, sizeof(plarray_t));

        /* Set safeArray up as a dynamically allocated array */
        safeArray->array = dynamicArray;
        safeArray->size = 4;
        safeArray->isMemAlloc = false;
        safeArray->mt = mt;

        /* Print out the contents of the integer array */
        printIntArray(safeArray);

        return 0;
    }
.. |plarray_t| replace:: ``plarray_t``

.. _plarray_t: plarray.rst