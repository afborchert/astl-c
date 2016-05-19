# astl-c
Astl programming language for C

This is currently in a development stage. If you want to clone it,
you should do this recursively:

'''
git clone --recursive https://github.com/afborchert/astl-c.git
'''

To build it, you need _bison_ 3.x, a recent _g++_ with C++14 support, GMP and
_boost_ libraries:

'''
(cd astl/astl && make)
(cd astl-c && make)
'''

Some examples are to be found in the _examples_ subdirectory.
