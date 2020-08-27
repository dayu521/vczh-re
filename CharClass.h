#ifndef CHARCLASS_H
#define CHARCLASS_H

#include <iostream>
#include <stdlib.h>
#include "LytWString.h"
#include "Link.h"
#include "Tree.h"
#include "SomeDataType.h"
#include "GrammarTree.h"

using namespace std;

class CharClass
{
public:
	wchar_t Start;
	wchar_t End;
	int Type;
	int Number;

	CharClass();
	~CharClass();
	bool operator==(CharClass& Temp);
	bool IsCharGroup();
};

class CharClassLink
{
public:
	Link<CharClass*> CharLink;
	int Size;

	CharClassLink();
	void CutIn(CharClass Temp);
	void GetCharClass(TreeNode<CharType>* Temp_GrammarTree);
	void SetCharNumber(GrammarTree& Expression);
	void Print();
	void Clear();
	void GetCharNumber(CharClass& Temp_CharClass, Set<int>& CharNumber);
	int GetCharNumber(const CharType& Input);
	~CharClassLink();
};

#endif
