#ifndef EASYTASKSCHEDULER_H
#define EASYTASKSCHEDULER_H

#include "../CBA.h"
#include <windows.h>
#include <comdef.h>
#include <wincred.h>
#include <initguid.h>
#include <ole2.h>
#include <mstask.h>
#include <msterr.h>
#include <wchar.h>
#include <taskschd.h>
#include <string>
#include <vector>
#include <map>
#include <strsafe.h>

//#include "DynamicMemoryLog.h"

#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")
#pragma comment(lib, "credui.lib")

namespace EasyTaskScheduler
{
	/// Error Constants ///
	enum ScheduleStatus { SS_OK, SS_EXE_PATH_ERROR, SS_TIMESTAMP_FORMAT_ERROR, SS_COM_SECURITY_ERROR, SS_ROOT_FOLDER_ERROR,
	                      SS_NEW_TASK_ERROR, SS_REGINFO_ERROR, SS_SET_TASK_AUTHOR_ERROR, SS_GET_TASK_SETTINGS_ERROR, 
						  SS_TASK_START_ERROR, SS_GET_TRIGGERS_ERROR, SS_CREATE_TRIGGER_ERROR, SS_TRIGGER_PTR_ERROR,
	                      SS_SET_TASK_START_ERROR, SS_GET_ACTION_COLLECTION_ERROR, SS_SET_TASK_ACTION_ERROR, 
	                      SS_GET_EXECUTABLE_PTR_ERROR, SS_SET_EXECUTABLE_PATH_ERROR, SS_USER_LOGON_DETAILS_ERROR,
	                      SS_TASK_DELAY_ERROR, SS_SET_LOGON_TYPE_ERROR, SS_SET_TIMEOUT_SUCCESS, SS_SET_REPETITION_PATTERN_ERROR,
	                      SS_SET_INTERVAL_ERROR, SS_SET_TASK_ENABLE_ERROR, SS_REGISTER_TASK_ERROR
	                    };

	/// Functions ///

	bool                      VerifySuccess( HRESULT& hr );
	//bool                      VerifySuccess( HRESULT& hr, DynamicMemoryLog* dmLog );
	bool                      InitialiseCOMSecurity( HRESULT& hr, ITaskService*& taskService );
	void                      DefineTaskSpecificVars( TASK_TRIGGER_TYPE2 triggerType, ITrigger*& trigger, IID& triggerClass, TASK_LOGON_TYPE& logonType, TCHAR*& userID );
	bool                      SetTaskDelayPeriod( HRESULT& hr, IBootTrigger*& trigger, tstring taskDelay );
	bool                      SetTaskDelayPeriod( HRESULT& hr, ILogonTrigger*& trigger, tstring taskDelay );
	bool                      SetTaskLogonName( HRESULT& hr, ILogonTrigger*& pLogonTrigger, TCHAR* userID );
	bool                      SetTaskLogonType( HRESULT& hr, ITaskDefinition* taskDef );
	bool                      SetTaskWaitTimeout( HRESULT& hr, ITaskSettings* taskSettings );
	bool                      SetTaskRepetitionPattern( HRESULT& hr, ITrigger*& trigger, tstring taskDuration, tstring taskInterval );
	bool                      SetTaskWeeklyInterval( HRESULT& hr, IWeeklyTrigger* trigger, short interval, short day );
	bool                      SetTaskMonthlyInterval( HRESULT& hr, IMonthlyDOWTrigger* trigger, short month, short week, short day );
	bool                      SetTaskEnabled( HRESULT& hr, ITrigger* trigger, bool enabled );
	bool                      PromptUserApproval();
	ScheduleStatus            RegisterTask( tstring taskName, tstring appPath, tstring timeStamp, TASK_TRIGGER_TYPE2 triggerType );
	ScheduleStatus            ScheduleTaskAtTime( tstring taskName, tstring appPath, tstring timeStamp );
	ScheduleStatus            ScheduleTaskAtStartup( tstring taskName, tstring appPath, tstring timeStamp );
	ScheduleStatus            ScheduleTaskAtLogon( tstring taskName, tstring appPath, tstring timeStamp );
	ScheduleStatus            ScheduleTaskDaily( tstring taskName, tstring appPath, tstring timeStamp );
	ScheduleStatus            ScheduleTaskWeekly( tstring taskName, tstring appPath, tstring timeStamp );
	ScheduleStatus            ScheduleTaskMonthly( tstring taskName, tstring appPath, tstring timeStamp );
	bool                      RemoveTask( tstring taskName );
	bool                      EditTask( tstring taskName );
	std::vector <tstring>     RetrieveScheduledTasks();
	std::vector <tstring>     RetrieveRunningTasks();
	std::wstring              s2ws( const std::string& s );
	std::wstring              StringToWString(const std::string& s);
	std::string               WStringToString(const std::wstring& s);
	bool                      ValidateTimeStamp( tstring timeStamp );
	bool                      FileExists( tstring filePath );
	void                      GetTimestamp( const DWORD Time, WCHAR DisplayString[] );
	tstring                   ErrorToString( ScheduleStatus statusCode );
	std::map <tstring, tstring> GetEnvironmentVariablesEx();
}

#endif // EASYTASKSCHEDULER_H