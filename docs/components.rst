*********************
pl32lib-ng components
*********************

``pl32lib-ng`` consists of 3 core modules. These modules have a stable API and
are feature-complete. Any new modules made for pl32lib-ng will be built on top
of these 3 modules.

pl32-memory
###########

``pl32-memory`` is a memory tracker module. It keeps track of all dynamic
memory allocation pointers and how much memory was allocated through it. The
trackers themselves are thread-specific, and shouldn't be used accross threads.

pl32-file
#########

``pl32-file`` is a wrapper around the Standard C File Streams API that allows
for the creation of file-in-memory: a block of memory that gets treated like a
file stream

pl32-token
##########

``pl32-token`` is a family of tokenizers that can be used to make custom
parsers or shell interpreters
