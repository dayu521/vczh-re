#include "CharClass.h"

//CharClass

CharClass::CharClass()
{
	Start=0;
	End=0;
	Number=0;
}

CharClass::~CharClass()
{
}

bool CharClass::operator==(CharClass& Temp)//不管Number的…
{
	if (Type==Temp.Type && Start==Temp.Start && End==Temp.End)
		return true;
	else return false;
}

bool CharClass::IsCharGroup()
{
	if (Start==End)
	{
		Type=Char;
		return false;
	}
	else return true;
}

//CharClassLink

CharClassLink::CharClassLink()
{
	Size=4;
	CharClass* Temp_CharClass;
	Temp_CharClass=new CharClass;
	Temp_CharClass->Start=L'n';
	Temp_CharClass->End=Temp_CharClass->Start;
	Temp_CharClass->Type=Transferred;
	CharLink.AddLast()->Data=Temp_CharClass;
	Temp_CharClass=new CharClass;
	Temp_CharClass->Start=L'r';
	Temp_CharClass->End=Temp_CharClass->Start;
	Temp_CharClass->Type=Transferred;
	CharLink.AddLast()->Data=Temp_CharClass;
	Temp_CharClass=new CharClass;
	Temp_CharClass->Start=L't';
	Temp_CharClass->End=Temp_CharClass->Start;
	Temp_CharClass->Type=Transferred;
	CharLink.AddLast()->Data=Temp_CharClass;
	Temp_CharClass=new CharClass;
	Temp_CharClass->Start=(wchar_t)0;
	Temp_CharClass->End=(wchar_t)65535;
	Temp_CharClass->Type=CharGroup;
	CharLink.AddLast()->Data=Temp_CharClass;
}

void CharClassLink::CutIn(CharClass Temp)
{
	Node<CharClass*>* TempNode_CharLink=CharLink.Tail;
	while (TempNode_CharLink)
	{
		if (Temp.Type==Char)
		{
			if (*(TempNode_CharLink->Data)==Temp)
				return;
			else if (TempNode_CharLink->Data->Type==Char)
			{
				if (TempNode_CharLink->Data->Start<Temp.Start)
				{
					CharClass* Temp_CharClass=new CharClass;
					Temp_CharClass->Start=Temp.Start;
					Temp_CharClass->End=Temp.End;
					Temp_CharClass->Type=Temp.Type;
					CharLink.AddFirst()->Data=Temp_CharClass;
					return;
				}
			}
			else if (TempNode_CharLink->Data->Type==CharGroup)
			{
				if (Temp.Start>TempNode_CharLink->Data->Start)
				{
					if (Temp.Start==TempNode_CharLink->Data->End)//5->[1-5]=[1-4]5
					{
						//5
						CharClass* Temp_CharClass=new CharClass;
						Temp_CharClass->Start=Temp.Start;
						Temp_CharClass->End=Temp_CharClass->Start;
						Temp_CharClass->Type=Temp.Type;
						CharLink.AddNext(TempNode_CharLink)->Data=Temp_CharClass;

						//[1-4]
						--(TempNode_CharLink->Data->End);
						TempNode_CharLink->Data->IsCharGroup();
						return;	
					}
					else//5->[1-9]=[1-4]5[6-9]
					{
						//5
						CharClass* Temp_CharClass=new CharClass;
						Temp_CharClass->Start=Temp.Start;
						Temp_CharClass->End=Temp_CharClass->Start;
						Temp_CharClass->Type=Temp.Type;
						CharLink.AddPrev(TempNode_CharLink)->Data=Temp_CharClass;

						//[1-4]
						Temp_CharClass=new CharClass;
						Temp_CharClass->Start=TempNode_CharLink->Data->Start;
						Temp_CharClass->End=Temp.Start-1;
						Temp_CharClass->Type=CharGroup;
						Temp_CharClass->IsCharGroup();
						CharLink.AddPrev(TempNode_CharLink->Prev)->Data=Temp_CharClass;

						//[6-9]
						TempNode_CharLink->Data->Start=Temp.Start+1;
						TempNode_CharLink->Data->IsCharGroup();
						return;
					}
				}
				else if (Temp.Start==TempNode_CharLink->Data->Start)//5->[5-9]=5[6-9]					
				{
					//5
					CharClass* Temp_CharClass=new CharClass;
					Temp_CharClass->Start=Temp.Start;
					Temp_CharClass->End=Temp_CharClass->Start;
					Temp_CharClass->Type=Temp.Type;
					CharLink.AddPrev(TempNode_CharLink)->Data=Temp_CharClass;

					//[6-9]
					++(TempNode_CharLink->Data->Start);	
					TempNode_CharLink->Data->IsCharGroup();
					return;
				}
			}
			else
			{
				//出错，本来得有一个全集可以拆分的，CharLink初始化出错
			}
		}
		else if (Temp.Type==CharGroup)
		{
			if (TempNode_CharLink->Data->Type==Char)
			{
				if (Temp.End>TempNode_CharLink->Data->Start)
				{
					if (Temp.Start==TempNode_CharLink->Data->Start)//[3-6]->3=3[4-6]
					{
						CharClass* Temp_CharClass=new CharClass;
						Temp_CharClass->Start=Temp.Start+1;
						Temp_CharClass->End=Temp.End;
						Temp_CharClass->Type=CharGroup;
						Temp_CharClass->IsCharGroup();
						CharLink.AddNext(TempNode_CharLink)->Data=Temp_CharClass;
						return;
					}
					else if (Temp.Start<TempNode_CharLink->Data->Start)//[1-8]->5=[0-4]5[6-9]
					{
						//[6-9]
						CharClass* Temp_CharClass=new CharClass;
						Temp_CharClass->Start=TempNode_CharLink->Data->Start+1;
						Temp_CharClass->End=Temp.End;
						Temp_CharClass->Type=CharGroup;
						Temp_CharClass->IsCharGroup();
						CharLink.AddNext(TempNode_CharLink)->Data=Temp_CharClass;

						//改Temp
						Temp.End=TempNode_CharLink->Data->Start-1;
						Temp.IsCharGroup();
					}
				}
				else if (Temp.End==TempNode_CharLink->Data->Start)//[3-6]->6=[3-5]6
				{
					//改Temp
					--Temp.End;
					Temp.IsCharGroup();
				}
			}
			else if (TempNode_CharLink->Data->Type==CharGroup)
			{
				if (Temp==*(TempNode_CharLink->Data))
					return;
				else if (Temp.End>TempNode_CharLink->Data->Start)
				{
					if (Temp.Start==TempNode_CharLink->Data->Start)
					{
						if (Temp.End<TempNode_CharLink->Data->End)//[1-6]->[1-9]=[1-6][7-9]
						{
							//[1-6]
							CharClass* Temp_CharClass=new CharClass;
							Temp_CharClass->Start=Temp.Start;
							Temp_CharClass->End=Temp.End;
							Temp_CharClass->Type=CharGroup;
							Temp_CharClass->IsCharGroup();
							CharLink.AddPrev(TempNode_CharLink)->Data=Temp_CharClass;
								
							//[7-9]
							TempNode_CharLink->Data->Start=Temp.End+1;
							TempNode_CharLink->Data->IsCharGroup();
							return;
						}
						else if (Temp.End>TempNode_CharLink->Data->End)//[1-6]->[1-3]=[1-3][4-6]
						{
							//[4-6]
							CharClass* Temp_CharClass=new CharClass;
							Temp_CharClass->Start=TempNode_CharLink->Data->End+1;
							Temp_CharClass->End=Temp.End;
							Temp_CharClass->Type=CharGroup;
							Temp_CharClass->IsCharGroup();
							CharLink.AddNext(TempNode_CharLink)->Data=Temp_CharClass;
							return;
						}
					}
					else if (Temp.Start>TempNode_CharLink->Data->Start)
					{
						if (Temp.End==TempNode_CharLink->Data->End)//[4-6]->[0-6]=[0-3][4-6]
						{
							//[4-6]
							CharClass* Temp_CharClass=new CharClass;
							Temp_CharClass->Start=Temp.Start;
							Temp_CharClass->End=Temp.End;
							Temp_CharClass->Type=CharGroup;
							Temp_CharClass->IsCharGroup();
							CharLink.AddNext(TempNode_CharLink)->Data=Temp_CharClass;

							//[0-3]
							TempNode_CharLink->Data->End=Temp.Start-1;
							TempNode_CharLink->Data->IsCharGroup();
							return;
						}
						else if (Temp.End<TempNode_CharLink->Data->End)//[4-6]->[0-9]=[0-3][4-6][7-9]
						{
							//[4-6]
							CharClass* Temp_CharClass=new CharClass;
							Temp_CharClass->Start=Temp.Start;
							Temp_CharClass->End=Temp.End;
							Temp_CharClass->Type=CharGroup;
							Temp_CharClass->IsCharGroup();
							CharLink.AddPrev(TempNode_CharLink)->Data=Temp_CharClass;

							//[0-3]
							Temp_CharClass=new CharClass;
							Temp_CharClass->Start=TempNode_CharLink->Data->Start;
							Temp_CharClass->End=Temp.Start-1;
							Temp_CharClass->Type=CharGroup;
							Temp_CharClass->IsCharGroup();
							CharLink.AddPrev(TempNode_CharLink->Prev)->Data=Temp_CharClass;

							//[7-9]
							TempNode_CharLink->Data->Start=Temp.End+1;
							TempNode_CharLink->Data->IsCharGroup();
							return;
						}
						else if (Temp.End>TempNode_CharLink->Data->End)//[4-9]->[0-6]=[0-3][4-6][7-9]
						{
							//[7-9]
							CharClass* Temp_CharClass=new CharClass;
							Temp_CharClass->Start=TempNode_CharLink->Data->End+1;
							Temp_CharClass->End=Temp.End;
							Temp_CharClass->Type=CharGroup;
							Temp_CharClass->IsCharGroup();
							CharLink.AddNext(TempNode_CharLink)->Data=Temp_CharClass;

							//[0-3]
							Temp_CharClass=new CharClass;
							Temp_CharClass->Start=TempNode_CharLink->Data->Start;
							Temp_CharClass->End=Temp.Start-1;
							Temp_CharClass->Type=CharGroup;
							Temp_CharClass->IsCharGroup();
							CharLink.AddPrev(TempNode_CharLink)->Data=Temp_CharClass;
								
							//[4-6]
							TempNode_CharLink->Data->Start=Temp.Start;
							TempNode_CharLink->Data->IsCharGroup();
							return;
						}
					}
					else if (Temp.Start<TempNode_CharLink->Data->Start)
					{
						if (Temp.End==TempNode_CharLink->Data->End)//[0-6]->[3-6]=[0-2][3-6]
						{
							//改Temp
							Temp.End=TempNode_CharLink->Data->Start-1;
							Temp.IsCharGroup();
						}
						else if (Temp.End<TempNode_CharLink->Data->End)//[0-6]->[4-9]=[0-3][4-6][7-9]
						{
							//[7-9]
							CharClass* Temp_CharClass=new CharClass;
							Temp_CharClass->Start=Temp.End+1;
							Temp_CharClass->End=TempNode_CharLink->Data->End;
							Temp_CharClass->Type=CharGroup;
							Temp_CharClass->IsCharGroup();
							CharLink.AddNext(TempNode_CharLink)->Data=Temp_CharClass;

							//[4-6]
							TempNode_CharLink->Data->End=Temp.End;
							TempNode_CharLink->Data->IsCharGroup();

							//改Temp
							Temp.End=TempNode_CharLink->Data->Start-1;
							Temp.IsCharGroup();
						}
						else if (Temp.End>TempNode_CharLink->Data->End)//[0-9]->[3-6]=[0-2][3-6][7-9]
						{
							//[7-9]
							CharClass* Temp_CharClass=new CharClass;
							Temp_CharClass->Start=TempNode_CharLink->Data->End+1;
							Temp_CharClass->End=Temp.End;
							Temp_CharClass->Type=CharGroup;
							Temp_CharClass->IsCharGroup();
							CharLink.AddNext(TempNode_CharLink)->Data=Temp_CharClass;

							//改Temp
							Temp.End=TempNode_CharLink->Data->Start-1;
							Temp.IsCharGroup();
						}
					}
				}
				else if (Temp.End==TempNode_CharLink->Data->Start)//[0-3]->[3-6]=[0-2]3[4-6]
				{
					//[4-6]
					CharClass* Temp_CharClass=new CharClass;
					Temp_CharClass->Start=Temp.End+1;
					Temp_CharClass->End=TempNode_CharLink->Data->End;
					Temp_CharClass->Type=CharGroup;
					Temp_CharClass->IsCharGroup();
					CharLink.AddNext(TempNode_CharLink)->Data=Temp_CharClass;

					//3
					TempNode_CharLink->Data->End=Temp.End;
					TempNode_CharLink->Data->Type=Char;

					//改Temp
					--Temp.End;
					Temp.IsCharGroup();
				}
			}
			else
			{
				//出错，插不进去，CharLink初始化出错
			}
		}
		else
		{
			//出错，要插入的东西不是字符，不是字符集(转义的东西不需要插入)
		}
		TempNode_CharLink=TempNode_CharLink->Prev;
	}
}

void CharClassLink::GetCharClass(TreeNode<CharType>* Temp_GrammarTree)
{
	if (Temp_GrammarTree)
	{
		if (Temp_GrammarTree->Left)
			GetCharClass(Temp_GrammarTree->Left);
		if (Temp_GrammarTree->Right)
			GetCharClass(Temp_GrammarTree->Right);
		CharClass Temp_CutIn;
		if (Temp_GrammarTree->Data.Type==Char)
		{
			Temp_CutIn.Start=Temp_GrammarTree->Data.Data;
			Temp_CutIn.End=Temp_CutIn.Start;
			Temp_CutIn.Type=Char;
			CutIn(Temp_CutIn);
		}
		else if (Temp_GrammarTree->Data.Type==CharGroup)
		{
			if (Temp_GrammarTree!=Temp_GrammarTree->Up->Left)
				return;
			else
			{
				Temp_CutIn.Start=Temp_GrammarTree->Data.Data;
				Temp_CutIn.End=Temp_GrammarTree->Up->Right->Data.Data;
				Temp_CutIn.Type=CharGroup;
				CutIn(Temp_CutIn);
			}
		}
	}
}

void CharClassLink::SetCharNumber(GrammarTree& Expression)
{
	GetCharClass(Expression.Data.TreeHead);
	int Count=0;
	Node<CharClass*>* TempNode_CharLink=CharLink.Head;
	while (TempNode_CharLink)
	{
		TempNode_CharLink->Data->Number=++Count;
		TempNode_CharLink=TempNode_CharLink->Next;
	}
	Size=Count;
}

void CharClassLink::Print()
{
	Node<CharClass*>* TempNode_CharLink=CharLink.Head;
	while (TempNode_CharLink)
	{
		switch(TempNode_CharLink->Data->Type)
		{
			case Char:
				wcout<<L"Char:		"<<TempNode_CharLink->Data->Start<<L"\tNumber: "<<TempNode_CharLink->Data->Number<<endl;
				break;
			case CharGroup:	
				wcout<<L"CharGroup:	"<<TempNode_CharLink->Data->Start<<L"-"<<TempNode_CharLink->Data->End<<L"\tNumber: "<<TempNode_CharLink->Data->Number<<endl;
				break;
			case Transferred:
				wcout<<L"Transferred:	"<<TempNode_CharLink->Data->Start<<L"\tNumber: "<<TempNode_CharLink->Data->Number<<endl;
				break;
		}
		TempNode_CharLink=TempNode_CharLink->Next;
	}
}

void CharClassLink::Clear()
{
	Node<CharClass*>* TempNode_CharLink=CharLink.Head;
	while (TempNode_CharLink)
	{
		delete TempNode_CharLink->Data;
		TempNode_CharLink=TempNode_CharLink->Next;
	}
	CharLink.Clear();
}

void CharClassLink::GetCharNumber(CharClass& Temp_CharClass, Set<int>& CharNumber)
{
	Node<CharClass*>* TempNode_CharLink=CharLink.Head;
	while (TempNode_CharLink)
	{
		if (TempNode_CharLink->Data->Type==Char || TempNode_CharLink->Data->Type==CharGroup)
		{
			if (Temp_CharClass.Type==CharGroup)
			{
				if (Temp_CharClass.Start<=TempNode_CharLink->Data->Start && Temp_CharClass.End>=TempNode_CharLink->Data->End)
					CharNumber.Add(TempNode_CharLink->Data->Number);
			}
			else if (Temp_CharClass.Type==Char)
			{
				if (Temp_CharClass==(*(TempNode_CharLink->Data)))
					CharNumber.Add(TempNode_CharLink->Data->Number);
			}
		}
		else if (TempNode_CharLink->Data->Type==Transferred && Temp_CharClass.Type==Transferred)
		{
			if (Temp_CharClass.Start==TempNode_CharLink->Data->Start)
				CharNumber.Add(TempNode_CharLink->Data->Number);
		}
		TempNode_CharLink=TempNode_CharLink->Next;
	}
}

int CharClassLink::GetCharNumber(const CharType& Input)
{
	Node<CharClass*>* TempNode_CharLink=CharLink.GetHead();
	while (TempNode_CharLink)
	{
		if (Input.Type==Char)
		{
			if (TempNode_CharLink->Data->Type==Char)
			{
				if (TempNode_CharLink->Data->Start==Input.Data)
					return TempNode_CharLink->Data->Number;
			}
			else if (TempNode_CharLink->Data->Type==CharGroup)
			{
				if (TempNode_CharLink->Data->Start<=Input.Data && Input.Data<=TempNode_CharLink->Data->End)
					return TempNode_CharLink->Data->Number;
			}
		}
		else if (Input.Type==Transferred)
		{
			if (TempNode_CharLink->Data->Type==Transferred) 
			{
				if (TempNode_CharLink->Data->Start==Input.Data)
					return TempNode_CharLink->Data->Number;
			}
		}
		TempNode_CharLink=TempNode_CharLink->Next;
	}
	return -1;
}

CharClassLink::~CharClassLink()
{
	Node<CharClass*>* TempNode_CharLink=CharLink.GetHead();
	while (TempNode_CharLink)
	{
		delete TempNode_CharLink->Data;
		TempNode_CharLink=TempNode_CharLink->Next;
	}
}
