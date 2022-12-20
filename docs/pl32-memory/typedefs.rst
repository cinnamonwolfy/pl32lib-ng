********************************
``pl32-memory``: Simple Typedefs
********************************

These typedefs are basically aliases to built-in C99 types created for code
clarity. Here are all of them:

``byte_t``
----------

Type: ASCII character, UTF-8 character block, 8-bit integer

An 8-bit integer meant to represent arbitrary data. It is typedef'd to ``char``

``string_t``
------------

Type: String (technically also a byte pointer, or an array of bytes)

A string of 8-bit characters/integers. It is typedef's to ``char*``

``memptr_t``
------------

Type: Pointer to memory block

A pointer to the beginning of a block of memory. It it typedef'd to ``void*``


When to use ``byte_t*`` or ``string_t``
---------------------------------------

As you have probably already figured out, these types are not necessary to use
pl32lib-ng. You can just ignore ``byte_t``, ``string_t`` and ``memptr_t``, and
just use ``char``, ``char*`` and ``void*``. In fact, two of these types are
redundant with each other (specifically ``byte_t*`` and ``string_t``),
functionally speaking. The sole purpose of these typedefs are for better code
readability and understandability. There may not be a functional difference
between ``byte_t*`` and ``string_t``, but there is a difference in intent

``byte_t*`` or ``byte_t[]`` should be used to signify that it is a binary data
buffer. While some binary data can be valid ASCII characters or a UTF-8
charblock, it is intended to be an array of 8-bit integers storing binary data

``string_t`` should be used for strings of text, ASCII or UTF-8. While it could
be used as a buffer for binary data, it's primary intent is to be a string of
text