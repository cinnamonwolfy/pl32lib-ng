**********
pl32lib-ng
**********

``pl32lib-ng`` is a rewrite of pl32lib that has been relicensed under the less
restrictive MPL-2.0 license. ``pl32lib`` was a library I had written to make C
development less error-prone and more thread-safe. This documentation will only
contain definitions and examples for the base library

Components
----------

pl32-memory
===========

|pl32-memory|_ is a memory tracker module. It keeps track of all dynamic
memory allocation pointers and how much memory was allocated through it. The
trackers themselves are thread-specific, and shouldn't be used accross threads.

pl32-file
=========

|pl32-file|_ is a wrapper around the Standard C File Streams API that allows
for the creation of file-in-memory: a block of memory that gets treated like a
file stream

pl32-token
==========

|pl32-token|_ is a family of tokenizers that can be used to make custom
parsers or shell interpreters



.. |pl32-memory| replace:: ``pl32-memory``
.. |pl32-file| replace:: ``pl32-file``
.. |pl32-token| replace:: ``pl32-token``
.. _pl32-memory: pl32-memory
.. _pl32-file: pl32-file
.. _pl32-token: pl32-token
