*********************************************
``pl32-memory`` function and type definitions
*********************************************

This directory contains all function, macro and type definitions within
``pl32-memory.h`` (public) and ``pl32-memory.c`` (internal), with this file
containing a list of links to them

Public Definitions (``pl32-memory.h``)
--------------------------------------

Macros and Types
================

* Macros_ (All of the macros will be in one file)
* `Simple Typedefs`_ (Stuff such as ``byte_t`` are typedef to basic types built into C99, so they will be in the same file)
* |plmt_t|_ (Technically private, as it's an opaque struct, but it is declared in the headers)
* |plarray_t|_

Functions
=========

* |plMTInit|_
* |plMTStop|_
* |plMTMemAmnt|_
* |plMTAlloc|_
* |plMTAllocE|_
* |plMTCalloc|_
* |plMTRealloc|_
* |plMTFree|_
* |plMTFreeArray|_

Private Definitions (``pl32-memory.c``)
---------------------------------------

Macros and Types
================

* |plptr_t|_
* |plmt_t|_ (Technically public, as it's declared in the headers, but the full definition is in ``pl32-memory.c``)

Functions
=========

* |plMTError|_
* |plMTManage|_


.. |plptr_t| replace:: ``plptr_t``
.. |plmt_t| replace:: ``plmt_t``
.. |plarray_t| replace:: ``plarray_t``
.. |plMTError| replace:: ``plMTError``
.. |plMTInit| replace:: ``plMTInit``
.. |plMTStop| replace:: ``plMTStop``
.. |plMTManage| replace:: ``plMTManage``
.. |plMTMemAmnt| replace:: ``plMTMemAmnt``
.. |plMTAlloc| replace:: ``plMTAlloc``
.. |plMTAllocE| replace:: ``plMTAllocE``
.. |plMTCalloc| replace:: ``plMTCalloc``
.. |plMTRealloc| replace:: ``plMTRealloc``
.. |plMTFree| replace:: ``plMTFree``
.. |plMTFreeArray| replace:: ``plMTFreeArray``

.. _Macros: macros.rst
.. _`Simple Typedefs`: typedefs.rst
.. _`plptr_t`: plpointer.rst
.. _`plmt_t`: plmt.rst
.. _`plarray_t`: plarray.rst
.. _plMTError: plmterror.rst
.. _plMTInit: plmtinit.rst
.. _plMTStop: plmtstop.rst
.. _plMTManage: plmtmanage.rst
.. _plMTMemAmnt: plmtmemamnt.rst
.. _plMTAlloc: plmtalloc.rst
.. _plMTAllocE: plmtalloc.rst
.. _plMTCalloc: plmtalloc.rst
.. _plMTRealloc: plmtalloc.rst
.. _plMTFree: plmtalloc.rst
.. _plMTFreeArray: plmtfreearray.rst
