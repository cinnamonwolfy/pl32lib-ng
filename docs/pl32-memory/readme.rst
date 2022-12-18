*********************************************
``pl32-memory`` function and type definitions
*********************************************

This directory contains all function, macro and type definitions within 
``pl32-memory.h`` (public) and ``pl32-memory.c`` (internal), with this file
containing a list of links to them

Public Definitions (``pl32-memory.h``)
--------------------------------------

* Macros_ (All of the macros will be in one file)
* `Simple Typedefs`_ (Stuff such as ``byte_t`` are typedef to basic types built into C99, so they will be in the same file)
* |plmt_t|_ (Technically private, as it's an opaque struct, but it is defined in the headers)
* |plarray_t|_

S

* |plMTInit|_
* |plMTStop|_
* |plMTMemAmnt|_
* |plMTAlloc|_
* |plMTAllocE|_
* |plMTCalloc|_
* |plMTRealloc|_
* |plMTFree|_
* |plMTFreeArray|_



.. | replace:: ``
.. _Macros: macros.rst
.. _`Simple Typedefs`: typedefs.rst
.. _`plmt_t`: plmt.rst
.. _`plarray_t`: plarray.rst
.. _plMTInit: plmtinit.rst
.. _plMTStop: plmtstop.rst
.. _plMTMemAmnt: plmtmemamnt.rst
.. _plMTAlloc: plmtalloc.rst
.. _plMTAllocE: plmtalloc.rst
.. _plMTCalloc: plmtcalloc.rst
.. _plMTRealloc: plmtrealloc.rst
.. _plMTFree: plmtfree.rst
.. _plMTFreeArray: plmtfreearray.rst
