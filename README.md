# k_tree
Library for making tree data structure with arbitrary nubmer of children per node. Inspired by [tree.hh](https://github.com/kpeeters/tree.hh)

# integration
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
