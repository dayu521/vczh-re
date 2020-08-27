#ifndef GRAMMARTREE_H
#define GRAMMARTREE_H

#include <iostream>
#include <stdlib.h>
#include "LytWString.h"
#include "Tree.h"
#include "SomeDataType.h"

using namespace std;

// A = A '|' B
// A = B
// B = BC
// B = C
// C = C '+'
// C = C '*'
// C = C '?'
// C = '(' A ')'
// C = D
// D = char
// D = '\' char
// D = '[' E ']'
// D = "[^" E ']'
// E = EF
// E = F
// F = char '-' char
// F = char
// F = '\' char

class GrammarTree
{
public:
	Tree<CharType> Data;
	
	GrammarTree();
	GrammarTree(const wchar_t* Input);
	void GetGrammarTree();
	LytWString GetErrorMessage();
	bool IsError();

private:
	const wchar_t* Expression;
	LytWString ErrorMessage;

	void SetErrorMessage(const LytWString ErrorInformation);
	TreeNode<CharType>* GetA(TreeNode<CharType>* Current);
	TreeNode<CharType>* GetB(TreeNode<CharType>* Current);
	TreeNode<CharType>* GetC(TreeNode<CharType>* Current);
	TreeNode<CharType>* GetD(TreeNode<CharType>* Current);
	TreeNode<CharType>* GetE(TreeNode<CharType>* Current);
	TreeNode<CharType>* GetF(TreeNode<CharType>* Current);
};

#endif
