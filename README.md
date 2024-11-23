# Eytzinger Binary Search

This is a C implementation of the Eytzinger layout for binary trees.  This
layout maps binary trees to arrays, similar to Heaps.

Binary search in Eytzinger arrays is cache friendly.  Once created, the arrays
cannot be modified anymore: implementing insert and delete functionality would
be too complex to be economical due to the way how the tree layout changes
depending on the size of the tree.

This implementation support creating arrays, searching, and forward and
backward iteration from a particular position in key order.
