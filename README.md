# k_tree
![C/C++ CI](https://github.com/tort-dla-psa/k_tree/workflows/C/C++%20CI/badge.svg)

Library for making tree data structure with arbitrary nubmer of children per node. Inspired by [tree.hh](https://github.com/kpeeters/tree.hh)

# Integration
It's header-only library due to reasons, so basically all you need to do is:
```sh
#in your project dir
git submodule add https://github.com/tort-dla-psa/k_tree.git submodules/k_tree
```
```cmake
#in CMakeLists.txt
include_directories(submodules/k_tree/include)
```
```c++
//in your code
#include "k_tree/k_tree.hpp"
```
# Usage

There're already a good examples in [tests](tests) directory.
```c++
k_tree::tree<int> t; //create empty tree
auto it = t.set_root(0); //set it's root (obviously) and save depth-first iterator to it
t.insert_left(it, 1); 
it = t.insert_right(it, 2);
t.append_child(it, 3);
t.prepend_child(it, 4);
/* this will create following tree:
   1-0-2
      / \
      4-3
*/
```

# Contributors
Just me yet
