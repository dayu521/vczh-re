#ifndef EPSILONNFA_H
#define EPSILONNFA_H

#include <iostream>
#include <stdlib.h>
#include "Tree.h"
#include "CharClass.h"
#include "StatusEdge.h"

using namespace std;

typedef StatusBase<NfaStatusNumber, EdgeMatchContent> NfaStatusBase;
typedef NfaStatusBase::EdgeBase NfaEdgeBase;
typedef Status<NfaStatusBase, NfaEdgeBase> NfaStatus;
typedef NfaStatus::Edge NfaEdge;

class EpsilonNfa
{
public:
	NfaStatus Start;
	NfaStatus End;

	EpsilonNfa();
	EpsilonNfa(const EpsilonNfa& Object);
	void Reverse(TreeNode<CharType>* Temp_TreeNode, CharClassLink& CharLink, Set<int>& CharNumber);
	EpsilonNfa GetEpsilonNfa(TreeNode<CharType>* Temp_TreeNode, CharClassLink& CharLink);
	static void RemoveUnnessaryData();
	void Print();
};

#endif
