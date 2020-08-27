#ifndef DFAMATRIX_H
#define DFAMATRIX_H

#include <iostream>
#include <stdlib.h>
#include "Matrix.h"
#include "Dfa.h"
#include "SomeDataType.h"

using namespace std;

class DfaMatrix
{
public:
	Matrix<int> Data;
	bool* Final; //长度为状态数量+1的数组，记录某个状态是否终结状态
	int StatusSize;
	int CharLinkSize;

	DfaMatrix();
	DfaMatrix(const int TempStatusSize,const int TempCharLinkSize);
	void New();
	void New(const int TempStatusSize,const int TempCharLinkSize);
	void GetDfaMatrix();
	void Print();
	~DfaMatrix();
};

#endif
