# astl-c
Astl programming language for C

## Synopsis

This is an implementation of the Astl programming language
for C. Astl is an easy to learn scripting language that is specialized
on attributed abstract syntax trees and control flow graphs.

Astl-C starts with one or more C sources, constructs an abstract
syntax tree for it, and executes then your script. Astl scripts
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
   }
}
```

A so-called attribution rule consists of a tree expression followed by a
block which is executed for each matching tree expression. Any number of
attribution rules can be specified within an _attribution rules_ section.
The tree expressions of the attribution rules are considered during an
implicitly executed traverse of the syntax tree in depth-first order (by
default in preorder but this can be changed per individual rule).

Tree expressions are set in parentheses. In their simplest form,
they specify an operator as string (`"if"` in the example above),
followed by tree expressions for the direct subnodes. An `"if"`
syntax node has two or three subnodes, the first represents the
condition, the second represents the statement to be executed
when the condition is true and the optional third subnode represents
the else-part. Variables can be introduced and bound to subnodes.
In this case, `cond` and `then` are bound to the first two subnodes
of the outer `"if"` node, and `if_stmt` to the `"if"` node itself.
These bound variables can be used within the following block. In
this example, `if_stmt` is passed to the built function `location`
which returns a string specifying the source location of the
entire if-statement.

Add a hash-bang-header to it, i.e. '#!', followed by the path to astl-c,
make it executable, and run it with all the source files:

```
$ misra-1410.ast example.c
if-else-chain without final else at example.c:6.4-10.4
$
```

## A more elaborate example

Occasionally, the initialization of a for loop variable
is missing. The gcc compiler delivers no warning and
if the loop variable is of an unsigned integer type
such an error may go unnoticed in cases as following
where the index variable is guaranteed to be within
the array index range within the body of the for loop:

```C
for (std::size_t i; i < sizeof(a)/sizeof(a[0]); ++i) {
   a[i] = i;
}
```

As _i_ is left uninitialized, it may be 0, positive but
within range, or out of range. In the latter two cases
the array is left partially or entirely uninitialized.

Astl-C represents a for loop with the exception of
the trivial case `for(;;)` always as a syntax tree
with the operator `"for"` and four operands representing
the initialization, the condition, the increment, and
the body of the loop. Following attribution rules would
match all for loops:

```
   ("for" stmt) -> {
      // trivial case
   }
   ("for" init condition increment stmt) -> {
      // regular case
   }
```

We are now focusing on cases where a loop variable
is declared within a for loop. The _init_ subtree
of the for loop is either an _expression_ or a
_declaration_. We are just interested in cases
where we have a declaration which in turn consists
of the _declaration specifiers_ (like `std::size_t`
in the example above) and an _init declarator list_
which can declare one or more variables. Each of the
variables within an _init declarator list_
is introduced by an _init declarator_ which consists
of a _declarator_ and optionally an _initializer_.
We are interested here in cases where we get
an _init declarator_ without an _initializer_ within
the init part of a for loop. This can be expressed
as follows:

```
   ("for"
         ("declaration"
            specifiers
            ("init_declarator_list"
               ("init_declarator"
                  declarator
               )
            )
         ) condition increment stmt) -> {
      println("for loop variable w/o initialization at ",
         location(declarator));
   }
```

Note that we specify the to be matched syntax subtree
only to the level of detail which is necessary.
We insist, for example, on a _declaration_
subnode within a _for_ node. However, we do not specify
how the first subnode of the declaration looks like.
Instead we let it be bound to the variable _specifiers_.
Next comes the _init declarator list_ along with an
_init declarator_ with just one _declarator_ without
an _initializer_. Here we do not go into further
detail regarding the _declarator_ as we just let the subnode
be bound to a variable of that name. Within the action
for matching subtrees we can access all bound variables
that are named within the tree expression.

As above, the predefined function _location_
delivers the source location range of the corresponding
syntax tree.

Unfortunately this tree expression does not work in the
general case as more than one variable can be declared
within a for loop:

```C
for (std::size_t i, j = 0; i < sizeof(a)/sizeof(a[0]); ++i, j += i) {
   a[i] = j;
}
```

Here we would like to issue a warning regarding _i_ but not for _j_.
To catch the individual cases it is best to reorganize our
tree expression where we want to catch cases of an _init declarator_
without _initializer_ within the initialization part of a for loop.
This is possible using contextual tree expressions:

```
   ("init_declarator" declarator) in
         ("for" here condition increment stmt) -> {
      println("for loop variable w/o initialization at ",
         location(declarator));
   }
```

Using the keyword **in** we can restrict matching to
cases where an _init declarator_ with just a _declarator_
appears within a _for_ node. But we must be careful here
as this could also match declarations within the body
of the for loop. Hence, we must make sure that contextual
matches are restricted to the initialization part of the
for loop. This is done using the keyword **here** which
tells where the subnode has to appear to match the
contextual condition.

The error messages for these warnings would be more helpful
if the actual loop variable is named where an initialization
is missing. We can do this by using a tree expression
that starts even deeper at the _direct declarator_:

```
("direct_declarator" ("identifier" varname)) in
   ("init_declarator" declarator) and in
      ("for" here condition increment stmt) -> {
   println("for loop variable ", varname,
      " w/o initialization at ", location(declarator));
}
```

Cascading contextual tree expressions match if the
first tree expression is embedded within a subnode matching
the next tree expression and if the latter subnode is
embedded within the next tree expression.

Syntax trees consist of regular operator nodes with optional
subnodes or tokens. Tokens are always embedded within
special subnodes which have no other role than to make
matching easier. In C, all identifiers are
embedded in subnodes of type _identifier_. Similarly,
integer constants are embedded in subnodes of type
_integer constant_, floating point constants in subnodes
of type _floating constant_ etc. In the example above
the actual token is bound to a variable named _varname_
which, like all variables bound to tokens, is of
string type and can be printed directly.

Unfortunately, this rule is still not good enough
as it provides warnings in perfectly legitimate cases.
Sometimes the exceptions are not obvious right from
the beginning but by testing a rule against real
world programs. When testing the rule above I came
across following case where the loop variable is
initialized within the condition of the for loop:

```C
for (char* line; (line = readline(fp)); free(line)) {
   // ...
}
```

This can no longer be easily handled by one single
tree expression. But several rules can cooperate
here through the use of attributions. The syntax
trees can be arbitrarily attributed in Astl. Each
variable that is bound to a node in the syntax tree
can be used to refer to attributes associated with
that particular node.

Here is a simple example that uses attributes:

```
   ("for" init condition increment stmt) as forloop -> pre {
      forloop.vars = {}; // candidates where we possibly issue a warning
   }
```

Tree expressions in parentheses which describe a
node of the syntax tree can be followed by the keyword
**as** and a variable name, causing the variable to
be bound to the matching node within the action.
In the example above, the variable named _forloop_ is bound
to the _for_ node.

Nodes within a syntax tree are
visited depth-first and each node is visited twice,
first in preorder, second in postorder after all the
subnodes were visited. Actions may be marked **pre** or
**post**, allowing them to be associated to the visit
in preorder or postorder, respectively.

Within the action we are free to set any attributes
related to a syntax tree node by qualifying attribute
names by the variable name bound to the node. In the
example above, we have an attribute named _vars_
which is refered to as _forloop.vars_ where _forloop_
is bound to the corresponding node. This variable will
be used in the following to keep track of all possible
candidates, i.e. all for loop variables that are left
uninitialized. We will then eliminate candidates
whenever we see an assignment to them within the
condition. Finally, we will print warnings for all
surviving candidates when we visit the _for_ node
in postorder.

Following rule catches all for loop variables without
initialization as before:

```
   ("direct_declarator" ("identifier" varname)) in
	 ("init_declarator" ("declarator" *)) and in
	 ("declaration" *) as decl and in
	 ("for" here condition increment stmt) as forloop -> post {
      forloop.vars{varname} = {
	 loc -> location(decl),
	 text -> gentext(decl)
      };
   }
```

As a warning at that point is premature we just keep record
of its location and, to make error messages more readable,
add the program text of the entire declaration. The predefined
function _gentext_ generates program text from the syntax tree
using the print rules which can be imported using `import printer;`
at the beginning of the Astl script. (Note that Astl does
not deliver the original program text beyond the level of
preserved tokens as syntax trees can be changed using
mutation rules in Astl.) The syntax for dictionaries in
Astl is similar to that of hashes in Perl, i.e. we can
collect key/value pairs within `{...}` and keys are
separated by the arrow symbol `->` from the value (Perl
uses `=>` for this delimiter).

Next we would like to eliminate all candidates that are
assigned to within the condition of the for loop:

```
   ("=" ("identifier" varname) rvalue) in
	 ("for" init here increment stmt) as forloop -> post {
      delete forloop.vars{varname}; // delete corresponding candidate
   }
```

Note that **delete** has no effect if the key does not exist.
Finally, we print warnings for the survivors:

```
   ("for" init condition increment stmt) as forloop -> post {
      /* print warnings for surviving candidates */
      foreach (varname, record) in (forloop.vars) {
	 println("for loop variable ", varname,
	    " is not initialized at ", record.loc, ": ", record.text);
      }
   }
```

Put together, this script delivers for

```
extern int f(int);
int main() {
   for (int i, j = 0, k; i = f(j); j += i) {
      // ...
   }
}
```

following error message:

```
for loop variable k is not initialized at badfor.c:3.8-24: int i, j = 0, k;
```

In the examples directory this script is to be found at
[_for-loop/check-for-loops.ast_](examples/for-loop/check-for-loops.ast).

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

To build it, you need _bison_ 3.x, a recent _g++_ with
C++14 support, GMP, _pcre2_, and _boost_ libraries:

```
(cd astl/astl && make)
(cd astl-c && make)
```

Documentation for Astl can also be obtained:

```
(cd astl/refman && make)
```

This requires TeX and some other tools.

Some examples are to be found in the _examples_ subdirectory.
