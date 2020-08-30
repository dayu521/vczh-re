#ifndef STATUSEDGE_H
#define STATUSEDGE_H

#include <iostream>
#include <stdlib.h>
#include "Set.h"

using namespace std;

class EdgeMatchContent
{
public:
	Set<int> Data;

	EdgeMatchContent();
	void Add(const int MatchContent);
	void Add(Set<int>& MatchContent);
};

class NfaStatusNumber
{
public:
	static int StaticStatusNumber;
	int StatusNumber;

	NfaStatusNumber();
};

class DfaStatusNumber
{
public:
	Set<int> StatusNumber;
	static int StaticStatusNumber;
	int Number;

	DfaStatusNumber();
	void Add(const int Temp_StatusNumber);
};

template<typename StatusNumber, typename EdgeContent>
class StatusBase
{
public:
	class EdgeBase
	{
	public:
		EdgeContent Data;
		static Link<EdgeBase*> AllEdge;
		StatusBase* From;
		StatusBase* To;

		EdgeBase()
		{
			From=0;
			To=0;
		}

		static void ReleaseEdge()
		{
            Node<EdgeBase*>* Temp=AllEdge.Head;
			while (Temp)
			{
				delete Temp->Data;
				Temp=Temp->Next;
			}
		}
	};

	Link<EdgeBase*> InEdges;
	Link<EdgeBase*> OutEdges;
	static Link<StatusBase*> AllStatus;
	bool FinalStatus;
	StatusNumber Data;

	StatusBase()
	{
		FinalStatus=0;
	}

	static void ReleaseStatus()
	{
		Node<StatusBase*>* Temp=AllStatus.Head;
		while (Temp)
		{
			delete Temp->Data;
			Temp=Temp->Next;
		}
	}
};
template<typename StatusNumber, typename EdgeContent>
Link<StatusBase<StatusNumber,EdgeContent>*> StatusBase<StatusNumber,EdgeContent>::AllStatus;

template<typename StatusNumber, typename EdgeContent>
Link<typename StatusBase<StatusNumber,EdgeContent>::EdgeBase *> StatusBase<StatusNumber,EdgeContent>::EdgeBase::AllEdge;

template<typename StatusData, typename EdgeData>
class Status
{
public:
	class Edge
	{
	public:
		EdgeData* Data;

		Edge()
		{
			Data=0;
		}

		Edge(EdgeData* Object)
		{
			Data=Object;
		}

		void New()
		{
			Data=new EdgeData;
			EdgeData::AllEdge.AddLast()->Data=Data;
		}

		void Connect(Status& FromStatus, Status& ToStatus)
		{
			New();
			FromStatus.Data->OutEdges.AddLast()->Data=Data;
			ToStatus.Data->InEdges.AddLast()->Data=Data;
			Data->From=FromStatus.Data;
			Data->To=ToStatus.Data;
		}

		void Connect(StatusData* FromStatusBase, StatusData* ToStatusBase)
		{
			New();
			FromStatusBase->OutEdges.AddLast()->Data=Data;
			ToStatusBase->InEdges.AddLast()->Data=Data;
			Data->From=FromStatusBase;
			Data->To=ToStatusBase;
		}

		void CutOff()
		{
			Node<EdgeData*>* Temp=Data->From->OutEdges.Find(Data);
			Temp->Data=0;
			Data->From->OutEdges.Delete(Temp);

			Temp=Data->To->InEdges.Find(Data);
			Temp->Data=0;
			Data->To->InEdges.Delete(Temp);
		}

	};

	StatusData* Data;

	Status()
	{
		Data=0;
	}

	Status(StatusData* Object)
	{
		Data=Object;
	}

	void New()
	{
		Data=new StatusData;
		StatusData::AllStatus.AddLast()->Data=Data;
	}
};

#endif
