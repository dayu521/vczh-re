#ifndef LYTWSTRING_H
#define LYTWSTRING_H

#define _CRT_SECURE_NO_DEPRECATE
#include <iostream>
#include <string.h>
#include <wchar.h>
#include <math.h>
#include <iomanip> 

using namespace std;

class LytWString
{
protected:
	int Length;
	wchar_t* String;
public:
	LytWString();
	LytWString(const wchar_t& Temp);
	LytWString(const wchar_t* Temp);
	LytWString(const LytWString& Temp);
	LytWString operator=(const wchar_t* Temp);
	LytWString operator=(const LytWString& Temp);
	bool operator==(const LytWString& Temp)const;
    bool operator!=(const LytWString& Temp)const;
    bool operator>(const LytWString& Temp)const;
    bool operator<(const LytWString& Temp)const;
    bool operator>=(const LytWString& Temp)const;
    bool operator<=(const LytWString& Temp)const;
	LytWString operator+(const LytWString& Temp)const;
	LytWString operator+(const wchar_t* Temp)const;
	LytWString operator++();
	LytWString operator++(int);
	LytWString operator--();
	LytWString operator--(int);
	//LytWString operator*();
	friend LytWString operator+(const wchar_t* TempLeft, const LytWString& TempRight);
	friend wostream& operator<<(wostream& Output, const LytWString& Temp);
	friend wistream& operator>>(wistream& Input, LytWString& Temp);
	friend LytWString Wchar_tToLytWString(const wchar_t& Temp);
	LytWString Sub(const int Index, const int Count)const;
	void Insert(const int Index, const LytWString Temp);
	void Delete(int Index, int Count);
	LytWString ToUpper()const;
	LytWString ToLower()const;
	LytWString Left(const int Count)const; 
	LytWString Right(const int Count)const;
	LytWString TrimLeft()const;
	LytWString TrimRight()const; 
	LytWString Trim()const;
	int Pos(const LytWString& Temp)const;
	int Replace(const LytWString& Find , const LytWString& Result);
	int Size()const;
	wchar_t& operator[](int Index);
	const wchar_t* Buffer()const;
	~LytWString();
};

#endif
