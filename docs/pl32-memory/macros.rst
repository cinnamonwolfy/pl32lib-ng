***********************
``pl32-memory``: Macros
***********************

Here are all of the macros defined within the header:

``PL32LIBNG_VERSION``
---------------------

Type: String

A string containing the version of the library


``PL32LIBNG_API_VER``
---------------------

Type: Integer

A number denoting the version of the API

``PL32LIBNG_FEATURELVL``
------------------------

Type: Integer

A number denoting the feature level


``PL32LIBNG_PATCHLVL``
----------------------

Type: Integer

A number denoting the patch level

``PLMT_STOP``
-------------

Type: Integer/Constant

One of the constants used by internal function ``plMTManage``. It tells the
memory manager to clear and free the memory tracker

``PLMT_SEARCHPTR``
------------------

Type: Integer/Constant

One of the constants used by internal function ``plMTManage``. It tells the
memory manager to return the index number a pointer is located within the
tracker buffer


``PLMT_ADDPTR``
---------------

Type: Integer/Constant

One of the constants used by internal function ``plMTManage``. It tells the
memory manager to add a pointer to the tracker buffer

``PLMT_RMPTR``
--------------

Type: Integer/Constant

One of the constants used by internal function ``plMTManage``. It tells the
memory manager to remove a pointer from the tracker buffer

``PLMT_REALLOC``
----------------

Type: Integer/Constant

One of the constants used by internal function ``plMTManage``. It tells the
memory manager to do special behavior specific to realloc

``PLMT_GET_USEDMEM``
--------------------

Type: Integer/Constant

One of the constants used by public function ``plMTMemAmnt``. It tells the
function to return the current tracker memory usage

``PLMT_GET_MAXMEM``
-------------------

Type: Integer/Constant

One of the constants used by public function ``plMTMemAmnt``. It tells the
function to return the currently set maximum allocation size

``PLMT_SET_MAXMEM``
-------------------

Type: Integer/Constant

One of the constants used by public function ``plMTMemAmnt``. It tells the
function to set the new maximum allocation size
