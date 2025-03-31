# CrossNet
A simple cross platform networking library.

## What is it ?
It's a simple library, designed to work on Linux and Windows systems, for general purposes.
It provides bindings to interact with both operating systems sockets APIs with similar functioning, and same
function names.

## How does it work on several platforms ?
Each library is compiled with same functions names, with similar processing, but different background functions calls.
Thank you engineers for inventing preprocessor definitions.