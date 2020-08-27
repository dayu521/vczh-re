#ifndef DFA_H
#define DFA_H

#include <iostream>
#include <stdlib.h>
#include "CharClass.h"
#include "StatusEdge.h"
#include "Nfa.h"

using namespace std;

typedef StatusBase<DfaStatusNumber, EdgeMatchContent> DfaStatusBase;
typedef DfaStatusBase::EdgeBase DfaEdgeBase;
typedef Status<DfaStatusBase, DfaEdgeBase> DfaStatus;
typedef DfaStatus::Edge DfaEdge;

class Dfa
{
public:
	DfaStatus Start;
	Set<DfaStatusBase*> UnsettledStatus;
	Set<DfaStatusBase*> ExisentStatus;

	Dfa();
	Dfa(Nfa& NfaGraph);
	DfaStatusBase* NfaStatusBaseToDfa(NfaStatusBase* Object);
	void FindCorrespondingNfaStatus(Set<int>& StatusNumber, Nfa& NfaGraph, Link<NfaStatusBase*>& Result);
	DfaStatusBase* IsExisentStatus(Set<int>& StatusNumber, bool Final);//如果状态不存在就加进UnsettledStatus、ExisentStatus
	void GetDfa(Nfa& NfaGraph);
	void Print();
};

#endif
