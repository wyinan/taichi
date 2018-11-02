/**
 * @file      CmdArgs.h
 * @brief     
 * @details   
 * @author    WYiNan
 * @contact   81656/user/sangfor
 * @date      2014-06-24
 * @copyright Sangfor Technologies Co.,Ltd.
 */
#ifndef _CMD_ARGS_H_
#define _CMD_ARGS_H_
#include "TypeDef.h"

#ifdef DBG_COMMANDLINE
static const pchart g_argvt[] = {DBG_COMMANDLINE};
#	define CmdArgc _countof(g_argvt)
#	define CmdArgv g_argvt
#else
#	define CmdArgc __argc
#	define CmdArgv __targv
#endif

#define MAX_CMDLINE	MAX_PATH

#define IsArg0(arg)	(0 == _tcsicmp(arg, CmdArgs::GetArg(0)))

#define IsArg1(arg)	(0 == _tcsicmp(arg, CmdArgs::GetArg(1)))

#define IsArg2(arg)	(0 == _tcsicmp(arg, CmdArgs::GetArg(2)))

class CmdArgs
{
public:
	/**
     * @brief 获取参数个数.
     */
	static int GetArgCnt()
	{
		return CmdArgc;
	}
	/**
     * @brief 获取参数.
     */
	static const pchart GetArg(int nIndex, const pchart pDefVal = stremptyt)
	{
		if (nIndex >= 0 && nIndex < CmdArgc)
		{
			return CmdArgv[nIndex];
		}
		return pDefVal;
	}
};

#endif  /* _CMD_ARGS_H_ */