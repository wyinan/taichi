#pragma once

#include <tchar.h>
#pragma warning(disable: 4244)
#include <string>
#pragma warning(default: 4244)
#include <cstring>
#include <fstream>
#include <vector>
#include <map>

/** 
 * @addtogroup stringflag
 * @brief �ַ�����ʶ��
 * @{
 */

//! ���ַ���
#define strempty	   ""
#define stremptyw	  L""
#define stremptyt	_T("")

//! ����λ��
#define strnpos		size_t(-1)	///< string::npos
//! ����λ��
#define strend		'\0'

//! �ַ��ֽ���
#define size_ch		sizeof(char)
#define size_chw	sizeof(charw)
#define size_cht	sizeof(chart)

//! ����������ֵ
#define size_end	sizeof(strend)
//! ���󳤶�ֵ
#define size_err	size_t(-1)

/** @} */

/** 
 * @addtogroup stringdef
 * @brief �ַ�������
 * @{
 */

typedef void* pvoid;

typedef wchar_t charw;

typedef char*	pchar;
typedef charw*	pcharw;

typedef const char*		cpchar;
typedef const wchar_t*	cpcharw;

#ifdef  UNICODE
typedef charw chart, *pchart;
typedef const charw* cpchart;
#else
typedef char  chart, *pchart;
typedef const char*  cpchart;
#endif

typedef std::string  string;
typedef std::wstring stringw;

#ifdef  UNICODE
typedef stringw stringt;
#else
typedef string  stringt;
#endif

#ifdef UNICODE
#define to_stringt std::to_wstring
#else
#define to_stringt std::to_string
#endif

/** @} */

/** 
 * @addtogroup array
 * @brief ���鶨��
 * @{
 */

typedef std::vector<char>  arrchar;
typedef std::vector<charw> arrcharw;
typedef std::vector<chart> arrchart;

/** @} */
