**********
pl32lib-ng
**********

``pl32lib-ng`` is a rewrite of pl32lib that has been relicensed under the less
restrictive MPL-2.0 license. ``pl32lib`` was a library I had written to make C
development less error-prone and more thread-safe. This documentation will only
contain definitions and examples for the base library

Components
##########

* ``pl32-memory``: Tracks dynamic memory allocations. It was the first module
implemented and it's also the module every other module depends on

* ``pl32-file``: Wrapper around the ``fopen`` suite of Standard C functions
that allow for file-in-memory creation

* ``pl32-token``: Family of tokenizers that can be used as the foundation for
any custom parser
