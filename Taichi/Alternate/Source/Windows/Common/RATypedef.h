#pragma once
#include <map>
#include <set>
#include <vector>
#include <stack>
#include <regex>


#ifdef UNICODE
#define Regex	std::wregex
#define CMatch	std::wcmatch
#define SMatch	std::wsmatch
#else
#define Regex	std::regex
#define CMatch	std::cmatch
#define SMatch	std::smatch
#endif


typedef std::map<CString, CString> CSCouple;
typedef std::map<CString, int> CICouple;
typedef std::map<int, CString> CISCouple;
typedef std::map<CString, long> CLCouple;

typedef std::vector<CString> VCString;
typedef std::vector<VCString> VVCString;

typedef std::set<CString> CStringSet;

typedef std::vector<std::string> VString;

typedef std::map<CString, CStringSet> CSMapSet;

typedef std::stack<CString> CStringStack;

typedef std::map<int, int> CMapII;

typedef std::map<int, CStringSet> CIMapSet;

typedef std::map<CString, short> CSSUMap;