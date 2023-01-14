*********************************************
``pl32-file`` function and type definitions
*********************************************

This directory contains all function, macro and type definitions within
``pl32-file.h`` (public) and ``pl32-file.c`` (internal), with this file
containing a list of links to them

Public Definitions (``pl32-file.h``)
--------------------------------------

Macros and Types
================

* |plfile_t|_ (Technically private, as it's an opaque struct, but it is declared in the headers)

Functions
=========

* |plFOpen|_
* |plFToP|_
* |plFClose|_
* |plFRead|_
* |plFWrite|_
* |plFPutC|_
* |plFGetC|_
* |plFPuts|_
* |plFGets|_
* |plFSeek|_
* |plFTell|_
* |plFPToFile|_
* |plFCat|_

Private Definitions (``pl32-file.c``)
---------------------------------------

Macros and Types
================

* |plfile_t|_ (Technically public, as it's declared in the headers, but the full definition is in ``pl32-file.c``)

.. |plfile_t| replace:: ``plfile_t``
.. |plFOpen| replace:: ``plFOpen``
.. |plFToP| replace:: ``plFToP``
.. |plFClose| replace:: ``plFClose``
.. |plFRead| replace:: ``plFRead``
.. |plFWrite| replace:: ``plFWrite``
.. |plFPutC| replace:: ``plFPutC``
.. |plFGetC| replace:: ``plFGetC``
.. |plFPuts| replace:: ``plFPuts``
.. |plFGets| replace:: ``plFGets``
.. |plFSeek| replace:: ``plFSeek``
.. |plFTell| replace:: ``plFTell``
.. |plFPToFile| replace:: ``plFPToFile``
.. |plFCat| replace:: ``plFCat``

.. _`plfile_t`: plfile.rst
.. _plFOpen: plfopen.rst
.. _plFToP: plftop.rst
.. _plFClose: plfclose.rst
.. _plFRead: plfread.rst
.. _plFWrite: plfwrite.rst
.. _plFPutC: plfputc.rst
.. _plFGetC: plfgetc.rst
.. _plFPuts: plfputs.rst
.. _plFGets: plfgets.rst
.. _plFSeek: plfseek.rst
.. _plFTell: plftell.rst
.. _plFPToFile: plfptofile.rst
.. _plFCat: plfcat.rst