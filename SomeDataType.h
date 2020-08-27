#ifndef SOMEDATATYPE_H
#define SOMEDATATYPE_H

#include <iostream>
#include <stdlib.h>
#include "Link.h"
#include "Set.h"

using namespace std;

const int Char=0, CharGroup=1, Transferred=2, Operator=3;
//写成枚举类型

const int Epsilon=0;

struct CharType
{
	int Type;
	wchar_t Data;
};

#endif
