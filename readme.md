# C exam #

## Compile: ##
Compile with makefile.

<code> make </code> <br>
<code> /exam </code> <br>
<code> /exam treetest </code> <br>

The "treetest" param runs a test on the actual treestructure. <br>
Running without params will parse the file(keyvalues.txt) and insert the derived 
key-value pairs into the b_tree and perform some operations on it. <br>
It will also verify that it works as expected. See sourcecode in main.c for more details.


## Description: ##
Application parses key-value pairs from the file keyvalues.txt and inserts them into a b-tree.
The file is assumed to have the following format:  path.key = "string" or path.key = int
parser.c provides useful methods for the parsing and manipulation of values.
The actual data structure is a tree with a dynamic number of nodes.
The tree includes an inplace sorting mechanism thus all nodes in a given lvl will be sorted.(see tests in main.c)
The tree implementation can be found in b_tree.c.

### Details: ### 
Since the format is path.key = "value", a path might be composed of multiple strings. 
I.e string.en.greeting = "hello", has a path = string.en, where "string" and "en"
are both folders. <br>
To solve the path problem I use a char* path[](path=["string", "en", "greeting"]) 
when accessing a given node. 
<br>
See main.c and b_tree_test.c for more detail on how this is done.
<br>
The sorting and dynamic allocation mechanisms are described in the source code of
b_tree.c and are tested, in main.c and b_tree_test.c so I wont go into any further
details about them here. 

[exam test](PG3400%20Home%20Exam%202017.pdf)
