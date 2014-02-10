#include <iostream>
using namespace std;
#include "tbd.h"


#include "machine.h"
#include "products.h"
#include "tests.h"
#include <set>
#include <vector>
using namespace std;

#define KeyType int 
#define ContainerType vector<KeyType> 
#include "methods.h"

#undef KeyType
#undef ContainerType
#define KeyType float 
#define ContainerType vector<KeyType> 
#include "methods.h"

#undef KeyType
#undef ContainerType
