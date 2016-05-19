# astl-c
Astl programming language for C

## Synopsis

This is an implementation of the Astl programming language
for C. Astl is an easy to learn scripting language that is specialized
on attributed abstract syntax trees and control flow graphs.

Astl-C starts with one or C sources, constructs an abstract
syntax tree for it, and executes then your program. Astl scripts
in their simplest form just provide tree expressions and
associated actions which are executed for each matching subtree.

Take as example the Misra rule 14.10 which states that
in Misra-C all _if_ ... _else if_ constructs shall be terminated
with an _else_ clause. If you want to check your sources for
conformance, you can put the violating tree pattern in
a tree expression:

```
attribution rules {
   ("if" cond then ("if" cond2 then2)) as if_stmt -> {
      println("if-else-chain without final else at ", location(if_stmt));
   };
};
```

So-called attribution rules consist of a collection of
tree expressions followed by a block which is executed
for each matching tree expression. The tree expressions
are considered during an implicitly executed traverse of
the syntax tree in depth-first order (by default in
preorder but this can be changed per individual rule).

Add a hash-bang-header to it, i.e. '#!', followed by
the path to astl-c, make it executable, and run it with
all the source files:

```
$ misra-1410.ast example.c
if-else-chain without final else at example.c:13.4-5
$
```

## Warning

I worked on this project in 2009, suspended it, and resumed
it for a short period in 2011. I am now consolidating
it towards more recent versions of C++, bison and the
libraries the project uses.

## Downloading and building

If you want to clone it, you should do this recursively:

```
git clone --recursive https://github.com/afborchert/astl-c.git
```

To build it, you need _bison_ 3.x, a recent _g++_ with C++14 support, GMP and
_boost_ libraries:

```
(cd astl/astl && make)
(cd astl-c && make)
```

Some examples are to be found in the _examples_ subdirectory.
