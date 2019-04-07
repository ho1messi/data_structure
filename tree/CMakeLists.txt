AUX_SOURCE_DIRECTORY (./ SRC_TREE)
AUX_SOURCE_DIRECTORY (./ SRC_SEARCH_TREE)

INCLUDE_DIRECTORIES (./)

ADD_EXECUTABLE (binary_search_tree ./test/binarySearchTree.cpp)
ADD_EXECUTABLE (avl_tree ./test/avlTree.cpp)
ADD_EXECUTABLE (redBlackTree ./test/redBlackTree.cpp)
ADD_EXECUTABLE (bTree ./test/bTree.cpp)

