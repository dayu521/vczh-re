#ifndef NFA_H
#define NFA_H

#include <iostream>
#include <stdlib.h>
#include "StatusEdge.h"
#include "EpsilonNfa.h"

using namespace std;

typedef StatusBase<NfaStatusNumber, EdgeMatchContent> NfaStatusBase;
typedef NfaStatusBase::EdgeBase NfaEdgeBase;
typedef Status<NfaStatusBase, NfaEdgeBase> NfaStatus;
typedef NfaStatus::Edge NfaEdge;

class Nfa
{
public:
	NfaStatus Start;
	Link<NfaStatusBase*> AvailableStatus;

	Nfa();
	Nfa(EpsilonNfa& EpsilonNfaGraph);
	void GetAvailableStatus(EpsilonNfa& EpsilonNfaGraph);
	bool IsEpsilonEdge(NfaEdgeBase* Object);
	void GetEpsilonClosure(NfaStatusBase* Temp_AvailableStatus, Set<NfaStatusBase*>& EpsilonClosure);
	void InsertEdge();
	void RemoveUnnessaryData();
	void GetNfa(EpsilonNfa& EpsilonNfaGraph);
	void Print();

};

#endif
