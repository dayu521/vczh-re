#include "StatusEdge.h"

typedef StatusBase<NfaStatusNumber, EdgeMatchContent> NfaStatusBase;
typedef NfaStatusBase::EdgeBase NfaEdgeBase;
typedef Status<NfaStatusBase, NfaEdgeBase> NfaStatus;
typedef NfaStatus::Edge NfaEdge;

template<>
Link<NfaStatusBase*> NfaStatusBase::AllStatus{};
template<>
Link<NfaEdgeBase*> NfaEdgeBase::AllEdge{};

typedef StatusBase<DfaStatusNumber, EdgeMatchContent> DfaStatusBase;
typedef DfaStatusBase::EdgeBase DfaEdgeBase;
typedef Status<DfaStatusBase, DfaEdgeBase> DfaStatus;
typedef DfaStatus::Edge DfaEdge;

template<>
Link<DfaStatusBase*> DfaStatusBase::AllStatus{};
template<>
Link<DfaEdgeBase*> DfaEdgeBase::AllEdge{};

//class EdgeMatchContent----------------------------

EdgeMatchContent::EdgeMatchContent()
{
}

void EdgeMatchContent::Add(const int MatchContent)
{
	Data.Add(MatchContent);
}

void EdgeMatchContent::Add(Set<int> &MatchContent)
{
	Data.Add(MatchContent);
}

//class NfaStatusNumber------------------------------

int NfaStatusNumber::StaticStatusNumber=0;

NfaStatusNumber::NfaStatusNumber()
{
	StatusNumber=++StaticStatusNumber;
}

//class DfaStatusNumber------------------------------

int DfaStatusNumber::StaticStatusNumber=0;

DfaStatusNumber::DfaStatusNumber()
{
	Number=++StaticStatusNumber;
}

void DfaStatusNumber::Add(const int Temp_StatusNumber)
{
	StatusNumber.Add(Temp_StatusNumber);
}



