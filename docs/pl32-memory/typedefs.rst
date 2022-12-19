********************************
``pl32-memory``: Simple Typedefs
********************************

These typedefs are basically aliases to built-in C99 types for clarity. Here
are all of them:

``byte_t``
----------

Type: ASCII character, 8-bit integer

An 8-bit integer meant to represent arbitrary data. It is typedef'd to ``char``

``string_t``
------------

Type: String (technically a byte pointer, or an array of bytes)

A string of 8-bit characters/integers. It is typedef's to ``char*``

``memptr_t``
------------

Type: Pointer to memory block

A pointer to the beginning of a block of memory. It it typedef'd to ``void*``
