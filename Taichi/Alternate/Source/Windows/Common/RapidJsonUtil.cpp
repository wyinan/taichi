#include "stdafx.h"
#include "RapidJsonUtil.h"
#include <rapidjson\document.h>
#include <rapidjson\prettywriter.h>
#include <rapidjson\filewritestream.h>
#include <rapidjson\writer.h>
#include <fstream>
#include <io.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL CRapidJsonUtil::LoadDocument(CString csFilePath, std::string& strContent,
	rapidjson::Document& document)
{
	std::ifstream inStream((LPCTSTR)csFilePath, std::ios::in | std::ios::binary);
	if (!inStream)
	{
		return FALSE;
	}

	strContent.clear();
	inStream.seekg(0, std::ios::end);
	strContent.resize((unsigned int)inStream.tellg());
	inStream.seekg(0, std::ios::beg);
	inStream.read(&strContent[0], strContent.size());
	inStream.close();

	if (document.ParseInsitu<0>((char*)strContent.c_str()).HasParseError())
	{
		return FALSE;
	}

	if (!document.IsObject())
	{
		return FALSE;
	}

	return TRUE;
}


static std::wstring UTF8ToUnicode(const std::string & s)
{
	std::wstring strResult;
	int cchWideChar = MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, NULL, 0);
	wchar_t* buffer = new wchar_t[cchWideChar];
	::MultiByteToWideChar(CP_UTF8, 0, s.c_str(), -1, buffer, cchWideChar);
	strResult = buffer;
	delete[] buffer;
	return strResult;
}


BOOL CRapidJsonUtil::LoadMap(const rapidjson::Value& valObject, CSCouple& map)
{
	using namespace rapidjson;

	map.clear();
	for (Value::ConstMemberIterator itr = valObject.MemberBegin();
		itr != valObject.MemberEnd(); ++itr)
	{
		const char* pKey = itr->name.GetString();
		const char* pValue = itr->value.GetString();
		ASSERT(pKey != NULL && pValue != NULL);
#ifdef UNICODE
		std::wstring strKey = UTF8ToUnicode(pKey);
		std::wstring strValue = UTF8ToUnicode(pValue);
		map[strKey.c_str()] = strValue.c_str();
#else
		map[pKey] = pValue;
#endif
	}

	return TRUE;
}