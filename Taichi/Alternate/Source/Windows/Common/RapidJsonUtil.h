#pragma once
#include <map>
#include <rapidjson\document.h>


typedef std::map<CString, CString> CSCouple;


class CRapidJsonUtil
{
public:
	static BOOL LoadDocument(CString csFilePath, std::string& strContent,
		rapidjson::Document& document);

	static BOOL LoadMap(const rapidjson::Value& valObject, CSCouple& map);

};

