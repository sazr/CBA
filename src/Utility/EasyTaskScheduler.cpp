#include "EasyTaskScheduler.h"

/// Function Implementation ///

/*std::wstring EasyTaskScheduler::s2ws( const std::string& s )
{
    int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
	wchar_t* buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
	std::wstring r(buf);
	delete[] buf;
	
	return r;
}


std::wstring EasyTaskScheduler::StringToWString(const std::string& s)
{
	std::wstring temp(s.length(),L' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp; 
}


std::string EasyTaskScheduler::WStringToString(const std::wstring& s)
{
	std::string temp(s.length(), ' ');
	std::copy(s.begin(), s.end(), temp.begin());
	return temp; 
}*/


std::map <tstring, tstring> EasyTaskScheduler::GetEnvironmentVariablesEx()
{
   std::map <tstring, tstring> envVariables;
   TCHAR* environVar    = GetEnvironmentStrings();
   TCHAR* environVarCpy = environVar; // we cannot free the char* if its memory address has been altered so we need to make a copy to alter
   TCHAR* pos           = _tcschr( environVarCpy, _T('\0') );
   
   // Skip over the "=::=::\0" of the environVar string
   if ( pos != NULL ) { environVarCpy = ++pos; pos = _tcschr( environVarCpy, _T('\0') ); }
   else return envVariables;
   
   while ( true )
   {
       TCHAR* delim = _tcschr( environVarCpy, '=' );
       if ( delim == NULL )
            break;
            
       tstring variable = tstring( environVarCpy, _tcslen(environVarCpy)-_tcslen(delim) );
       tstring value    = tstring( ++delim );
       
       envVariables.insert( std::pair<tstring, tstring>(variable, value) );
       environVarCpy = ++pos;
       
       // find the "\0\0" that identifies the end of environVar
       if ( pos != NULL && *pos == 0 ) { break; }
       
       pos = _tcschr( environVarCpy, _T('\0') );
   }
   
   FreeEnvironmentStrings( environVar ); 
   return envVariables;       
}


bool EasyTaskScheduler::ValidateTimeStamp( tstring timeStamp )
{
    // Post:
	// Format should be YYYY-MM-DDTHH:MM:SS(+-)(timezone)

    std::size_t pos;
	TCHAR delims[] = {_T('-'), _T('-'), _T(':'), _T(':'), _T('\0')};
	int   expLen[] = {4, 2, 5, 2, 2};

	try
	{
		for (int i=0; i<5; i++)
		{
			pos             = timeStamp.find_first_of( delims[i] );
			tstring res     = timeStamp.substr( 0, pos );
			timeStamp       = timeStamp.substr( pos+1, std::string::npos );

			if ( res.length() != expLen[i] )
				return false;
		}

	}
	catch ( ... )
	{
		return false;
	}


	return true;

}


bool EasyTaskScheduler::FileExists( tstring filePath )
{
	// Post: 

	WIN32_FIND_DATA fileData;
	HANDLE          hFile;


	//LPCTSTR nFilePath = filePath.c_str();
    hFile = FindFirstFileEx( (LPCTSTR)filePath.c_str(), FindExInfoStandard, &fileData, 
		                     FindExSearchNameMatch, NULL, 0 );
         
    // Alternative:                            
    // HANDLE hFile = CreateFile( filePath.c_str(), GENERIC_READ, 0, NULL, 
    //                            OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );
    
    
	/*
	// Alternative: Check file exists & file type == .exe
	DWORD fileAttribs = GetFileAttributes( filePath.c_str() );

	// TODO: How to check a files type
	if ( !(fileAttribs & FILE_EXECUTABLE) )
	{
		return false;
	}
	*/

    if ( hFile == INVALID_HANDLE_VALUE ) 
    { 
        return false;
    }

	return true;
}


bool EasyTaskScheduler::VerifySuccess( HRESULT& hr )
{
	// Post:

	if ( FAILED(hr) )
	{
		//DynamicMemoryLog::GetInstance() -> ReleaseDynamicMemory();
		return false;
	}

	return true;
}


bool EasyTaskScheduler::InitialiseCOMSecurity( HRESULT& hr, ITaskService*& taskService )
{
	// Post: Initialise COM & COM security

	taskService = NULL;
	hr          = CoInitializeEx( NULL, COINIT_MULTITHREADED );

	if ( FAILED(hr) )
	{
		printf( "Failed to initialise hr: %x \n", hr );
		return false;
	}

	hr = CoInitializeSecurity( NULL, -1, NULL, NULL, RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
		                       RPC_C_IMP_LEVEL_IMPERSONATE, NULL, 0, NULL );

	if ( FAILED(hr) )
	{
		printf( "CoInitialiseSecurity Failed: %x \n", hr );
		CoUninitialize();
		return false;
	}
	
	hr = CoCreateInstance( CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER,
		                   IID_ITaskService, (void**) &taskService );

	if ( FAILED(hr) )
	{
		printf( "Failed to CoCreate an instance of the TaskService class: %x \n", hr );
		CoUninitialize();
		return false;
	}

	hr = taskService -> Connect( _variant_t(), _variant_t(), _variant_t(), _variant_t() );

	if ( FAILED(hr) )
	{
		printf( "ITaskService::Connect failed: %x \n", hr );
		taskService -> Release();
		CoUninitialize();
		return false;
	}

	return true;
}


bool EasyTaskScheduler::SetTaskEnabled( HRESULT& hr, ITrigger* trigger, bool enabled )
{
	// Post:

	hr = trigger -> put_Enabled( (VARIANT_BOOL) enabled );

	if ( !VerifySuccess(hr) )
	{
		printf( "Failed to enable task \n" );
		return false;
	}

	return true;
}


bool EasyTaskScheduler::SetTaskLogonName( HRESULT& hr, ILogonTrigger*& pLogonTrigger, const TCHAR userId[])
{
	
	hr = pLogonTrigger->put_UserId(_bstr_t(userId));
    
    if ( !VerifySuccess(hr) )
    {
        printf("\nCannot add user ID to logon trigger: %x", hr );
        return false;
    }

	return true;
}


bool EasyTaskScheduler::SetTaskLogonType( HRESULT& hr, ITaskDefinition* taskDef )
{
	// Post: 

	IPrincipal    *taskPrinciple = NULL;

	hr = taskDef -> get_Principal( &taskPrinciple );

	if ( !VerifySuccess(hr) )
	{
		printf( "Cannot get principal pointer \n" );
		return false;
	}

	//DynamicMemoryLog::GetInstance()->AddIObject( taskPrinciple );
	hr = taskPrinciple -> put_LogonType( TASK_LOGON_INTERACTIVE_TOKEN );

	if ( !VerifySuccess(hr) )
	{
		printf( "Cannot put principal info \n" );
		return false;
	}

	return true;
}


bool EasyTaskScheduler::SetTaskWaitTimeout( HRESULT& hr, ITaskSettings* taskSettings )
{
	// Post:
	// TODO: add parameter to set the wait timeout period

	IIdleSettings *idleSettings  = NULL;

	hr = taskSettings -> get_IdleSettings( &idleSettings );

	if ( !VerifySuccess(hr) )
	{
		printf( "Cannot get idle setting information: \n" );
		return false;
	}

	//DynamicMemoryLog::GetInstance()->AddIObject( idleSettings );
	hr = idleSettings -> put_WaitTimeout( _T("PT5M") );

	if ( !VerifySuccess(hr) )
	{
		printf( "Cannot put idle setting information \n" );
		return false;
	}

	return true;
}


bool EasyTaskScheduler::SetTaskDelayPeriod( HRESULT& hr, ILogonTrigger*& trigger, tstring taskDelay )
{
	// Post: 

	hr = trigger -> put_Delay( _bstr_t(taskDelay.c_str()) );
    
	if ( !VerifySuccess(hr) )
	{
		printf( "Cannot put delay for trigger \n" );
		return false;
	}

	return true;
}


bool EasyTaskScheduler::SetTaskDelayPeriod( HRESULT& hr, IBootTrigger*& trigger, tstring taskDelay )
{
	// Post: 

	hr = trigger -> put_Delay( _bstr_t(taskDelay.c_str()) );
    
	if ( !VerifySuccess(hr) )
	{
		printf( "Cannot put delay for trigger \n" );
		return false;
	}

	return true;
}


bool EasyTaskScheduler::SetTaskRepetitionPattern( HRESULT& hr, ITrigger*& trigger, tstring taskDuration, tstring taskInterval )
{
	// Post: 

	IRepetitionPattern *repPattern = NULL;

	hr = trigger -> get_Repetition( &repPattern );

	if ( !VerifySuccess(hr) )
	{
		printf( "Cannot set tasks repetition pattern \n" );
		return false;
	}

	//DynamicMemoryLog::GetInstance()->AddIObject( repPattern );

	hr = repPattern -> put_Duration( _bstr_t(taskDuration.c_str()) );
    
	if( !VerifySuccess(hr) )
    {
        printf( "Cannot put repetition duration \n" );
        return false;
    }

	// if task interval is undefined
	if ( taskInterval == _T("") )
		return false;

    hr = repPattern -> put_Interval( _bstr_t(taskInterval.c_str()) );

    if( FAILED(hr) )
    {
        printf( "Cannot put repetition interval \n" );
        return false;
    }   

	return true;
}


bool EasyTaskScheduler::SetTaskWeeklyInterval( HRESULT& hr, IWeeklyTrigger* trigger, short interval, short day )
{
	// Post:

	hr = trigger -> put_WeeksInterval( interval ); // if interval = 2, the task will run every 2nd week

	if ( !VerifySuccess(hr) )
	{
		printf( "Cannot set weekly interval \n" );
        return false;
	}
	
	hr = trigger -> put_DaysOfWeek( day );        // 1 = Sunday, 2 = Monday, 4, 8, 16, 32, 64

	if ( !VerifySuccess(hr) )
	{
		printf( "Cannot set IWeeklyTrigger DaysOfWeek member var \n" );
        return false;
	}

	return true;
}


bool EasyTaskScheduler::SetTaskMonthlyInterval( HRESULT& hr, IMonthlyDOWTrigger* trigger, short month, short week, short day )
{
	// Post:

	hr = trigger -> put_MonthsOfYear( month );

	if ( !VerifySuccess(hr) )
	{
		printf( "Failed to set MonthsOfYear member var \n" );
		return false;
	}

	hr = trigger -> put_WeeksOfMonth( week );

	if ( !VerifySuccess(hr) )
	{
		printf( "Failed to set MonthsOfYear member var \n" );
		return false;
	}

	hr = trigger -> put_DaysOfWeek( day );

	if ( !VerifySuccess(hr) )
	{
		printf( "Failed to set MonthsOfYear member var \n" );
		return false;
	}

	return true;
}


bool EasyTaskScheduler::PromptUserApproval()
{
	// Post:

	CREDUI_INFO cui;
    TCHAR pszName[CREDUI_MAX_USERNAME_LENGTH] = _T("");
    TCHAR pszPwd[CREDUI_MAX_PASSWORD_LENGTH]  = _T("");
    BOOL  fSave;
    DWORD dwErr;

    cui.cbSize     = sizeof(CREDUI_INFO);
    cui.hwndParent = NULL;
    //  Ensure that MessageText and CaptionText identify
    //  what credentials to use and which application requires them.
    cui.pszMessageText = TEXT("Account information for task registration:");
    cui.pszCaptionText = TEXT("Enter Account Information for Task Registration");
    cui.hbmBanner      = NULL;
    fSave              = FALSE;

    //  Create the UI asking for the credentials.
    dwErr = CredUIPromptForCredentials(
        &cui,                             //  CREDUI_INFO structure
        TEXT(""),                         //  Target for credentials
        NULL,                             //  Reserved
        0,                                //  Reason
        pszName,                          //  User name
        CREDUI_MAX_USERNAME_LENGTH,       //  Max number for user name
        pszPwd,                           //  Password
        CREDUI_MAX_PASSWORD_LENGTH,       //  Max number for password
        &fSave,                           //  State of save check box
        CREDUI_FLAGS_GENERIC_CREDENTIALS | //  Flags
        CREDUI_FLAGS_ALWAYS_SHOW_UI |
        CREDUI_FLAGS_DO_NOT_PERSIST);  

    if(dwErr)
    {
        printf( "Did not get credentials \n" ); 
		SecureZeroMemory(pszName, sizeof(pszName));
        SecureZeroMemory(pszPwd, sizeof(pszPwd));
		//DynamicMemoryLog::GetInstance() -> ReleaseDynamicMemory();
        return false;      
    }

	SecureZeroMemory(pszName, sizeof(pszName));
    SecureZeroMemory(pszPwd, sizeof(pszPwd));

	return true;
}


void EasyTaskScheduler::DefineTaskSpecificVars( TASK_TRIGGER_TYPE2 triggerType, ITrigger*& trigger, IID& triggerClass, TASK_LOGON_TYPE& logonType, TCHAR userID[] )
{
	// Post:

	DWORD username_len = MAX_PATH;
	GetUserName(userID, &username_len);

	switch ( triggerType )
	{
		case TASK_TRIGGER_LOGON:
		{
			trigger      = dynamic_cast<ILogonTrigger*>(trigger);
			triggerClass = IID_ILogonTrigger;                    //userID = _bstr_t( _wgetenv(_T("USERNAME")) );
			logonType    = TASK_LOGON_INTERACTIVE_TOKEN;
			/*_tdupenv_s( &userID, &len, _T("USERNAME") );
			printf( "TEST: %S\n", userID );*/
		}
		break;
		case TASK_TRIGGER_TIME:
		{
			trigger      = dynamic_cast<ITimeTrigger*>(trigger); // (ITimeTrigger*)trigger;
			triggerClass = IID_ITimeTrigger;                     // userID       = _bstr_t( _wdupenv_s(_T("USERNAME")) ); 
			logonType    = TASK_LOGON_INTERACTIVE_TOKEN;
			/*_tdupenv_s( &userID, &len, _T("USERNAME") );*/
		}
		break;
		case TASK_TRIGGER_BOOT:
		{
			trigger      = dynamic_cast<IBootTrigger*>(trigger); // (IBootTrigger*)trigger;
			triggerClass = IID_IBootTrigger;                     // userID       = _T("Local Service");
			logonType    = TASK_LOGON_SERVICE_ACCOUNT;
			/*_tdupenv_s( &userID, &len, _T("Local Service") );*/
		}
		break;
		case TASK_TRIGGER_DAILY:
		{
			trigger      = dynamic_cast<IDailyTrigger*>(trigger);
			triggerClass = IID_IDailyTrigger;                     // userID       = _bstr_t( _wdupenv_s(_T("USERNAME")) );
			logonType    = TASK_LOGON_INTERACTIVE_TOKEN;          // _variant_t(_bstr_t(pszName)), _variant_t(_bstr_t(pszPwd)), TASK_LOGON_PASSWORD
			/*_tdupenv_s( &userID, &len, _T("USERNAME") );*/
		}
		break;
		case TASK_TRIGGER_WEEKLY:
		{
			trigger      = dynamic_cast<IWeeklyTrigger*>(trigger);
			triggerClass = IID_IWeeklyTrigger;                     // userID       = _bstr_t( _wdupenv_s(_T("USERNAME")) ); 
			logonType    = TASK_LOGON_INTERACTIVE_TOKEN;
			/*_tdupenv_s( &userID, &len, _T("USERNAME") );*/
		}
		break;
		case TASK_TRIGGER_MONTHLY:
		{
			trigger      = dynamic_cast<IMonthlyDOWTrigger*>(trigger);
			triggerClass = IID_IMonthlyTrigger;                    // userID       = _bstr_t( _wdupenv_s(_T("USERNAME")) ); 
			logonType    = TASK_LOGON_INTERACTIVE_TOKEN;
			/*_tdupenv_s( &userID, &len, _T("USERNAME") );*/
		}
		break;
		default:
		{
			printf( "Invalid param value (triggerType) \n" );
			trigger      = dynamic_cast<IDailyTrigger*>(trigger);
			triggerClass = IID_IDailyTrigger;                     // userID       = _bstr_t( _wdupenv_s(_T("USERNAME")) );
			logonType    = TASK_LOGON_INTERACTIVE_TOKEN;          // _variant_t(_bstr_t(pszName)), _variant_t(_bstr_t(pszPwd)), TASK_LOGON_PASSWORD
			/*_tdupenv_s( &userID, &len, _T("USERNAME") );*/
		}
		break;
	}
}


EasyTaskScheduler::ScheduleStatus EasyTaskScheduler::RegisterTask( tstring taskName, tstring appPath, tstring timeStamp, TASK_TRIGGER_TYPE2 triggerType )
{
	// Post:

	ITaskService       *taskService    = NULL;
	ITaskFolder        *rootFolder     = NULL;
	ITaskDefinition    *taskDef        = NULL;
	IRegistrationInfo  *regInfo        = NULL;
	ITaskSettings      *taskSettings   = NULL;
	ITriggerCollection *triggerCol     = NULL;
	ITrigger           *mainTrigger    = NULL;
	IActionCollection  *actionCol      = NULL;
	IAction            *taskAction     = NULL;
	IExecAction        *execAction     = NULL;
	IRegisteredTask    *regTask        = NULL;
	ITrigger           *trigger        = NULL;
	HRESULT            hr              = NULL;
	IID                triggerClass    = IID_IUnknown;
	TCHAR             userID[MAX_PATH] = _T("");
	TASK_LOGON_TYPE    logonType       = TASK_LOGON_INTERACTIVE_TOKEN;
	VARIANT            varPassword;
	varPassword.vt                     = VT_EMPTY;


	DefineTaskSpecificVars( triggerType, trigger, triggerClass, logonType, userID);

	// Return if appPath doesn't lead to an actual application
	if ( !FileExists( appPath ) )
	{
		outputStr( "appPath parameter Error: appPath does not lead to an actual executable \n" );
		return SS_EXE_PATH_ERROR;
	}

	// DEBUGGING
	output( _T("TEST1: %s\n"), userID );

	if ( !ValidateTimeStamp( timeStamp ) )
	{
		outputStr( "timeStamp parameter Error: timeStamp has invalid format \n" );
		return SS_TIMESTAMP_FORMAT_ERROR;
	}

	// DEBUGGING
	output( _T("TEST2: %s\n"), userID );

	if ( !InitialiseCOMSecurity( hr, taskService ) )
	{
		outputStr( "Function InitialiseCOMSecurity() Failed \n" );
		//DynamicMemoryLog::GetInstance() -> ReleaseDynamicMemory(); 
		return SS_COM_SECURITY_ERROR;
	}

	// Initialise common interface objects
	//DynamicMemoryLog::GetInstance() -> AddIObject( taskService );
	hr = taskService -> GetFolder( _bstr_t( _T("\\")), &rootFolder );

	if ( !VerifySuccess(hr) )
	{
		outputStr( "Failed to get root folder \n" );
		return SS_ROOT_FOLDER_ERROR;
	}

	// DEBUGGING
	output(_T("TEST3: %s\n"), userID);

	//DynamicMemoryLog::GetInstance() -> AddIObject( rootFolder );
	// Delete previous task (of same name) if it exists
	rootFolder -> DeleteTask( _bstr_t(taskName.c_str()), 0 );

	hr = taskService -> NewTask( 0, &taskDef );

	if ( !VerifySuccess(hr) )
	{
		outputStr( "Failed to CoCreate an instance of the TaskService class \n" );
		return SS_NEW_TASK_ERROR;
	}

	//DynamicMemoryLog::GetInstance() -> AddIObject( taskDef );
	hr = taskDef -> get_RegistrationInfo( &regInfo );

	if ( !VerifySuccess(hr) )
	{
		outputStr( "Failed to RegInfo identification pointer \n" );
		return SS_REGINFO_ERROR;
	}

	//DynamicMemoryLog::GetInstance() -> AddIObject( regInfo );
	hr = regInfo -> put_Author( _bstr_t(userID) );  // _wgetenv(_T("USERNAME")) );

	if ( !VerifySuccess(hr) )
	{
		outputStr( "Cannot put identification info \n" );
		return SS_SET_TASK_AUTHOR_ERROR;
	}

	hr = taskDef -> get_Settings( &taskSettings );

	if ( !VerifySuccess(hr) )
	{
		outputStr( "Cannot get settings pointer \n" );
		return SS_GET_TASK_SETTINGS_ERROR;
	}

	// NEW 2016 July
	taskSettings->put_Priority(HIGH_PRIORITY_CLASS);
	taskSettings->put_DisallowStartIfOnBatteries(VARIANT_BOOL(FALSE));
	// END NEW

	//DynamicMemoryLog::GetInstance() -> AddIObject( taskSettings );
	hr = taskSettings -> put_StartWhenAvailable( VARIANT_BOOL(true) );

	if ( !VerifySuccess(hr) )
	{
		outputStr( "Cannot put setting information \n" );
		return SS_TASK_START_ERROR;
	}
    
    hr = taskDef -> get_Triggers( &triggerCol );

	if( !VerifySuccess(hr) )
    {
        outputStr( "Cannot get trigger collection \n" );
		return SS_GET_TRIGGERS_ERROR;
    }

	//DynamicMemoryLog::GetInstance() -> AddIObject( triggerCol );
    //  Add the time trigger to the task.    
    hr = triggerCol -> Create( triggerType, &mainTrigger ); 

    if( !VerifySuccess(hr) )
    {
        outputStr( "Cannot create trigger \n" );
	    return SS_CREATE_TRIGGER_ERROR;
    }

	//DynamicMemoryLog::GetInstance() -> AddIObject( mainTrigger );
    hr = mainTrigger -> QueryInterface( (REFIID)triggerClass, (void**) &trigger );

    if( !VerifySuccess(hr) )
    {
        outputStr( "QueryInterface call failed for ITimeTrigger \n" );
		return SS_TRIGGER_PTR_ERROR;
    }

	//DynamicMemoryLog::GetInstance() -> AddIObject( trigger );
    hr = trigger -> put_Id( _bstr_t( _T("Trigger1") ) );     // TODO: add parameter to set trigger id
    
	if( FAILED(hr) )
        outputStr( "Cannot put trigger ID \n" );

    hr = trigger -> put_EndBoundary( _bstr_t(_T("2050-05-02T08:00:00")) );
    
	if( FAILED(hr) )
       outputStr( "Cannot put end boundary on trigger \n" );

    //  Set the task to start at a certain time. The time 
    //  format should be YYYY-MM-DDTHH:MM:SS(+-)(timezone).
    //  For example, the start boundary below
    //  is January 1st 2005 at 12:05
    hr = trigger -> put_StartBoundary( _bstr_t(timeStamp.c_str()) );   
    
	if( !VerifySuccess(hr) )
    {
        outputStr( "Cannot add start boundary to trigger \n" );
		return SS_SET_TASK_START_ERROR;
    }
    
    //  Add an action to the task. Get the task action collection pointer.
    hr = taskDef -> get_Actions( &actionCol );

    if( !VerifySuccess(hr) )
    {
        outputStr( "Cannot get Task collection pointer \n" );
		return SS_GET_ACTION_COLLECTION_ERROR;
    }
    
	//DynamicMemoryLog::GetInstance() -> AddIObject( actionCol );
    //  Create the action, specifying that it is an executable action.
    hr = actionCol -> Create( TASK_ACTION_EXEC, &taskAction );

    if( !VerifySuccess(hr) )
    {
        outputStr( "Cannot create the action \n" );
        return SS_SET_TASK_ACTION_ERROR;
    }

	//DynamicMemoryLog::GetInstance() -> AddIObject( taskAction );
    //  QI for the executable task pointer.
	hr = taskAction -> QueryInterface( IID_IExecAction, (void**) &execAction );
    
	if( !VerifySuccess(hr) )
    {
		output(_T("QueryInterface call failed for IExecAction: %x %X %u \n"), hr, hr, hr);
		return SS_GET_EXECUTABLE_PTR_ERROR;
    }

	//DynamicMemoryLog::GetInstance() -> AddIObject( execAction );
    //  Set the path of the executable to appPath
    hr = execAction -> put_Path( _bstr_t( appPath.c_str() ) );
	// NEW July
	tstring exeDir = appPath.substr(0, appPath.rfind(_T("\\")));
	execAction->put_WorkingDirectory( _bstr_t(exeDir.c_str()) );
	execAction->put_Arguments( _bstr_t( _T("/scheduledstart") ) );
	// END NEW

    if( !VerifySuccess(hr) )
    {
        outputStr( "Cannot put action path \n" );
		return SS_SET_EXECUTABLE_PATH_ERROR;
    }  


	// Define task specific actions & data
	switch ( triggerType )
	{
		case TASK_TRIGGER_LOGON:
		{
			bool setLogonUserSuccess = SetTaskLogonName(hr, (ILogonTrigger*&)trigger, userID);
			
			if ( !setLogonUserSuccess )
			{
				return SS_USER_LOGON_DETAILS_ERROR;
			}

			if ( !SetTaskDelayPeriod( hr, (ILogonTrigger*&) trigger, _T("PT30S") ) )
			{
				return SS_TASK_DELAY_ERROR;
			}

		}
		break;
		case TASK_TRIGGER_TIME:
		{
			bool setLogonSuccess   = SetTaskLogonType( hr, taskDef );
			bool setTimeoutSuccess = SetTaskWaitTimeout( hr, taskSettings );
			
			if ( !setLogonSuccess )
			{
				return SS_SET_LOGON_TYPE_ERROR;
			}
			
			if ( !setTimeoutSuccess )
			{
				return SS_SET_TIMEOUT_SUCCESS;
			}
		}
		break;
		case TASK_TRIGGER_BOOT:
		{
			if ( !SetTaskDelayPeriod( hr, (IBootTrigger*&) trigger, tstring(_T("PT30S")) ) )
			{
				return SS_TASK_DELAY_ERROR;
			}
		}
		break;
		case TASK_TRIGGER_DAILY:
		{
			bool setPatternSuccess = SetTaskRepetitionPattern( hr, trigger, tstring(_T("PT10M")), tstring(_T("PT1M")) ); // "PT1M", "PT1M" );
			//bool userApproval    = PromptUserApproval();

			if ( !setPatternSuccess )
			{
				return SS_SET_REPETITION_PATTERN_ERROR;
			}
		}
		break;
		case TASK_TRIGGER_WEEKLY:
		{
			bool enableSuccess      = SetTaskEnabled( hr, trigger, true );
			bool setIntervalSuccess = SetTaskWeeklyInterval( hr, (IWeeklyTrigger*&) trigger, 1, 16 );
			//bool setPatternSuccess  = SetTaskRepetitionPattern( hr, trigger, "PT10W", "" ); 

			if ( !setIntervalSuccess )
			{
				return SS_SET_INTERVAL_ERROR;
			}
			
			if ( !enableSuccess )
			{
				return SS_SET_TASK_ENABLE_ERROR;
			}
		}
		break;
		case TASK_TRIGGER_MONTHLY:
		{
			bool enableSuccess      = SetTaskEnabled( hr, trigger, true );
			bool setIntervalSuccess = SetTaskMonthlyInterval( hr, (IMonthlyDOWTrigger*&) trigger, 1, 16, 16 );

			if ( !setIntervalSuccess )
			{
				return SS_SET_INTERVAL_ERROR;
			}

			if ( !enableSuccess )
			{
				return SS_SET_TASK_ENABLE_ERROR;
			}
		}
		break;
		default:
		break;
	}

	VARIANT varUser;
	varUser.vt = VT_EMPTY;
	VARIANT varSddl;
	varSddl.vt = VT_EMPTY;

	//  Save the task in the root folder
    //hr = rootFolder -> RegisterTaskDefinition( _bstr_t( taskName.c_str() ), 
		//taskDef, TASK_CREATE_OR_UPDATE, 
		//varUser, //_variant_t(_bstr_t(userID)), 
		//varPassword, 
		//TASK_LOGON_NONE, 
		//varSddl, //_variant_t(_T("")),
		//&regTask);

	hr = rootFolder->RegisterTaskDefinition(
		_bstr_t(taskName.c_str()),
		taskDef,
		TASK_CREATE_OR_UPDATE,
		_variant_t(_bstr_t(userID)), //_variant_t(_T("Builtin\\Users")),
		varPassword,
		TASK_LOGON_NONE,
		_variant_t(_T("")),
		&regTask);

	// Debugging
	output(_T("A: %S\n"), taskName.c_str());
	output(_T("B: %S\n"), userID);
	TCHAR* a;
	size_t len;
	_tdupenv_s( &a, &len, _T("USERNAME") );
	output(_T("C: %S\n"), a);
	// end debugging

	if ( !VerifySuccess(hr) )
    {
        output( _T("Error saving the Task. HR= %x \n"), hr );
        return SS_REGISTER_TASK_ERROR;
    }
    
	//DynamicMemoryLog::GetInstance() -> AddIObject( regTask );
    outputStr( "Success! Task successfully registered \n" );

    // Clean up.
	//DynamicMemoryLog::GetInstance() -> ReleaseDynamicMemory();

    return SS_OK;
}


EasyTaskScheduler::ScheduleStatus EasyTaskScheduler::ScheduleTaskAtTime( tstring taskName, tstring appPath, tstring timeStamp )
{
    // Post: 
	// timeStamp: Format should be YYYY-MM-DDTHH:MM:SS(+-)(timezone)
	
	return RegisterTask( taskName, appPath, timeStamp, TASK_TRIGGER_TIME );
}


EasyTaskScheduler::ScheduleStatus EasyTaskScheduler::ScheduleTaskAtStartup( tstring taskName, tstring appPath, tstring timeStamp )
{
	// Post:
	// timeStamp: Format should be YYYY-MM-DDTHH:MM:SS(+-)(timezone)

	return RegisterTask( taskName, appPath, timeStamp, TASK_TRIGGER_BOOT );
}


EasyTaskScheduler::ScheduleStatus EasyTaskScheduler::ScheduleTaskAtLogon( tstring taskName, tstring appPath, tstring timeStamp )
{
    // Post: 
	// timeStamp: Format should be YYYY-MM-DDTHH:MM:SS(+-)(timezone)
	
	return RegisterTask( taskName, appPath, timeStamp, TASK_TRIGGER_LOGON );
}


EasyTaskScheduler::ScheduleStatus EasyTaskScheduler::ScheduleTaskDaily( tstring taskName, tstring appPath, tstring timeStamp )
{
    // Post: 
	// timeStamp: Format should be YYYY-MM-DDTHH:MM:SS(+-)(timezone)
	
	return RegisterTask( taskName, appPath, timeStamp, TASK_TRIGGER_DAILY );
}


EasyTaskScheduler::ScheduleStatus EasyTaskScheduler::ScheduleTaskWeekly( tstring taskName, tstring appPath, tstring timeStamp )
{
	// Post:
	// timeStamp: Format should be YYYY-MM-DDTHH:MM:SS(+-)(timezone)

	return RegisterTask( taskName, appPath, timeStamp, TASK_TRIGGER_WEEKLY );
}


EasyTaskScheduler::ScheduleStatus EasyTaskScheduler::ScheduleTaskMonthly( tstring taskName, tstring appPath, tstring timeStamp )
{
	// Post:
	// timeStamp: Format should be YYYY-MM-DDTHH:MM:SS(+-)(timezone)

	return RegisterTask( taskName, appPath, timeStamp, TASK_TRIGGER_MONTHLY );
}


bool EasyTaskScheduler::RemoveTask( tstring taskName )
{
	// Post: 

	ITaskService *taskService = NULL;
	ITaskFolder  *rootFolder  = NULL;
	HRESULT      hr           = NULL;
	_bstr_t bTaskName(taskName.c_str());


	if ( !InitialiseCOMSecurity( hr, taskService ) )
	{
		outputStr( "Function InitialiseCOMSecurity() Failed \n" );
		return false;
	}

	// Retrive root folder

	hr = taskService -> GetFolder( _bstr_t( _T("\\")), &rootFolder );
	taskService -> Release();

	if ( FAILED(hr) )
	{
		output(_T("Cannot get Root Folder pointer: %x \n"), hr);
		CoUninitialize();
		return false;
	}

	// Delete previous task (of same name) if it exists
	hr = rootFolder -> DeleteTask( bTaskName, 0 );

	if ( hr != S_OK )
	{
		//std::cout << " Failed to delete task \n";
	}
	//else std::cout << "Task deleted \n";

	rootFolder -> Release();
	CoUninitialize();

	return true;
}


bool EasyTaskScheduler::EditTask( tstring taskName )
{
	// Post:

	ITaskScheduler *taskSched  = NULL;
	ITask          *task       = NULL;
	HWND           parent      = NULL;
	HRESULT        hr          = NULL;
	DWORD          reserved    = 0;

	hr = CoInitialize( NULL );

	if ( SUCCEEDED(hr) )
	{
		hr = CoCreateInstance( CLSID_CTaskScheduler, NULL, CLSCTX_INPROC_SERVER,
			                   IID_ITaskScheduler, (void**) &taskSched );

		if ( FAILED(hr) )
		{
			outputStr( "Failed to create instance \n" );
			CoUninitialize();
			return false;
		}
	}
	else {
		outputStr( "Failed to initialise HRESULT \n" );
		return false;
	}

	hr = taskSched -> Activate( _bstr_t(taskName.c_str()), IID_ITask, (IUnknown**) &task );
	taskSched -> Release();

	if ( FAILED(hr) )
	{
		output(_T("Failed to get task %x \n"), hr);
		CoUninitialize();
		return false;
	}

	hr = task -> EditWorkItem( parent, reserved );
	task -> Release();

	if ( FAILED(hr) )
	{
		outputStr( "Failed to edit task \n" );
		CoUninitialize();
		return false;
	}

	CoUninitialize();
	return true;

}


std::vector <tstring> EasyTaskScheduler::RetrieveScheduledTasks()
{
    // Post: 

	HRESULT                   hr              = NULL;
	ITaskService              *taskService    = NULL;
	ITaskFolder               *rootFolder     = NULL;
	IRegisteredTaskCollection *taskCollection = NULL;
	LONG                      taskNum         = 0;
	TASK_STATE                taskState;
	std::vector <tstring> tasks;
	// std::vector <IRegisteredTask*> tasks;

	/// Initialise COM & COM security

	if ( !InitialiseCOMSecurity( hr, taskService ) )
	{
		outputStr( "Failed to initialise COM & COM security \n" );
		return tasks;
	}

	/// Retrieve tasks from task folder

	hr = taskService -> GetFolder( _bstr_t( _T("\\")), &rootFolder );
	taskService -> Release();

	if ( FAILED(hr) )
	{
		output(_T("Cannot get Root Folder pointer: %x \n"), hr);
		CoUninitialize();
		return tasks;
	}

	hr = rootFolder -> GetTasks( TASK_ENUM_HIDDEN, &taskCollection );
	rootFolder -> Release();

	if( FAILED(hr) )
    {
		output(_T("Cannot get the registered tasks.: %x \n"), hr);
        CoUninitialize();
        return tasks;
    }

	hr = taskCollection -> get_Count( &taskNum );

	if ( taskNum == 0 )
    {
        outputStr( "\nNo Tasks are currently running" );
        taskCollection -> Release();
        CoUninitialize();
        return tasks;
    }


	/// Iterate over returned tasks & store them
	
	for (LONG i=0; i<taskNum; i++)
	{
		//TaskInfo ti;
		tstring taskStr          = _T("");
		IRegisteredTask *regTask = NULL;
		hr = taskCollection -> get_Item( _variant_t(i+1), &regTask );
        
        if ( SUCCEEDED(hr) )
        {
			// Opportunity to store task objects
			// tasks.push_back( regTask );

            BSTR taskName = NULL;

            hr = regTask -> get_Name( &taskName );

            if( SUCCEEDED(hr) )
            {
                taskStr += "Task Name: " + _bstr_t( taskName ) + ", ";
                SysFreeString( taskName );

                hr = regTask -> get_State( &taskState );
                
				if ( SUCCEEDED (hr) )
				{
                    TCHAR buf[32];
					_itot_s( taskState, buf, 32, 10 );
					taskStr += _T("Task State: ") + tstring(buf);
				}
                else taskStr += _T("Task State: Failed to retrieve task state");

            }
            else taskStr += _T("Task Name: Failed to retrieve task name ");

            regTask -> Release();
			SysFreeString( taskName );
			tasks.push_back( taskStr );
        }
		else output(_T("\nCannot get the registered task item at index=%d: %x"), i + 1, hr);

    }


    taskCollection -> Release();
    CoUninitialize();
    
	return tasks;
}


std::vector <tstring> EasyTaskScheduler::RetrieveRunningTasks()
{
	// Post: 

	HRESULT                   hr            = NULL;
	ITaskService              *taskService  = NULL;
	IRunningTaskCollection    *runningTasks = NULL;
	LONG                      taskNum       = 0;
	TASK_STATE                taskState;
	std::vector <tstring> tasks;

	if ( !InitialiseCOMSecurity( hr, taskService ) )
	{
		outputStr( "Function InitialiseCOMSecurity() Failed \n" );
		return tasks;
	}

	/// Retrieve running tasks

	hr = taskService -> GetRunningTasks( TASK_ENUM_HIDDEN, &runningTasks );

	if( FAILED(hr) )
    {
		output(_T("Cannot get Root Folder pointer: %x \n"), hr);
        CoUninitialize();
        return tasks;
    }

	hr = runningTasks -> get_Count( &taskNum );

	if( taskNum == 0 )
    {
        outputStr( "\nNo Tasks are currently running" );
        runningTasks -> Release();
        CoUninitialize();
        return tasks;
    }

	/// Iterate over returned tasks & store them
	
	for (LONG i=0; i<taskNum; i++)
	{
		tstring taskStr   = _T("");
        IRunningTask* selTask = NULL;
        hr = runningTasks -> get_Item( _variant_t(i+1), &selTask );
        
        if( SUCCEEDED(hr) )
        {
            BSTR taskName = NULL;
            hr = selTask -> get_Name( &taskName );
            
			if( SUCCEEDED(hr) )
            {
                taskStr += "Task Name: " + _bstr_t( taskName ) + ", ";
                SysFreeString( taskName );

                hr = selTask -> get_State( &taskState );
                
				if ( SUCCEEDED (hr) )
				{
                    TCHAR* buf = NULL;
					size_t len = 0;
					_itot_s( taskState, buf, len, 10 );
					taskStr += _T("Task State: ") + tstring(buf);
				}
                else taskStr += _T("Task State: Failed to retrieve task state");

            }
            else taskStr += _T("Task Name: Failed to retrieve task Name, ");

			tasks.push_back( taskStr );
			SysFreeString( taskName );
            selTask->Release();
        }
		else output(_T("\nCannot get the registered task item at index=%d: %x"), i + 1, hr);

    }

    runningTasks -> Release();
    CoUninitialize();
    
	return tasks;
}


void EasyTaskScheduler::GetTimestamp( const DWORD Time, WCHAR DisplayString[] ) 
{
	// Post: Get a string that contains the time stamp of when the event 
    //       was generated

    ULONGLONG ullTimeStamp = 0;
    ULONGLONG SecsTo1970 = 116444736000000000;
    SYSTEMTIME st;
    FILETIME ft, ftLocal;

    #define MAX_TIMESTAMP_LEN 500001

    ullTimeStamp      = Int32x32To64(Time, 10000000) + SecsTo1970;
    ft.dwHighDateTime = (DWORD)((ullTimeStamp >> 32) & 0xFFFFFFFF);
    ft.dwLowDateTime  = (DWORD)(ullTimeStamp & 0xFFFFFFFF);
    
    FileTimeToLocalFileTime(&ft, &ftLocal);
    FileTimeToSystemTime(&ftLocal, &st);
    StringCchPrintf( DisplayString, MAX_TIMESTAMP_LEN, _T("%d/%d/%d %.2d:%.2d:%.2d"), 
                     st.wMonth, st.wDay, st.wYear, st.wHour, st.wMinute, st.wSecond);
}


tstring EasyTaskScheduler::ErrorToString( ScheduleStatus statusCode )
{
	tstring statusStr;

	switch ( statusCode )
	{
		case SS_OK:
		{
			statusStr = _T("Schedule Success");
		}
		break;
		case SS_EXE_PATH_ERROR:
		{
			statusStr = _T("Executable string is not a path to an actual executable");
		}
		break;
		case SS_TIMESTAMP_FORMAT_ERROR:
		{
			statusStr = _T("Timestamp string is incorrectly formatted");
		}
		break;
		case SS_COM_SECURITY_ERROR:
		{
			statusStr = _T("Failed to initialise the COM security");
		}
		break;
		case SS_ROOT_FOLDER_ERROR:
		{
			statusStr = _T("Failed to retireve the root folder");
		}
		break;
		case SS_NEW_TASK_ERROR:
		{
			statusStr = _T("Failed to create a new task object");
		}
		break;
		case SS_REGINFO_ERROR:
		{
			statusStr = _T("Failed to retrieve the tasks registered information");
		}
		break;
		case SS_SET_TASK_AUTHOR_ERROR:
		{
			statusStr = _T("Failed to set the tasks author");
		}
		break;
		case SS_GET_TASK_SETTINGS_ERROR:
		{
			statusStr = _T("Failed to retrieve the tasks settings");
		}
		break;
		case SS_TASK_START_ERROR:
		{
			statusStr = _T("Failed to set the tasks put_StartWhenAvailable() value");
		}
		break;
		case SS_GET_TRIGGERS_ERROR:
		{
			statusStr = _T("Failed to retrieve the trigger");
		}
		break;
		case SS_CREATE_TRIGGER_ERROR:
		{
			statusStr = _T("Failed to create the trigger object");
		}
		break;
		case SS_TRIGGER_PTR_ERROR:
		{
			statusStr = _T("Failed to retrive the trigger pointer from QueryInterface");
		}
		break;
		case SS_SET_TASK_START_ERROR:
		{
			statusStr = _T("Failed to set the tasks start timestamp");
		}
		break;
		case SS_GET_ACTION_COLLECTION_ERROR:
		{
			statusStr = _T("Failed to retrieve the triggers action collection pointer from QueryInterface");
		}
		break;
		case SS_SET_TASK_ACTION_ERROR:
		{
			statusStr = _T("Failed to set the tasks action type");
		}
		break;
		case SS_GET_EXECUTABLE_PTR_ERROR:
		{
			statusStr = _T("Failed to retrieve the executable pointer from QueryInterface");
		}
		break;
		case SS_SET_EXECUTABLE_PATH_ERROR:
		{
			statusStr = _T("Failed to set the tasks executable path");
		}
		break;
		case SS_USER_LOGON_DETAILS_ERROR:
		{
			statusStr = _T("Failed to set the tasks logon details");
		}
		break;
		case SS_TASK_DELAY_ERROR:
		{
			statusStr = _T("Failed to set the tasks delay period");
		}
		break;
		case SS_SET_LOGON_TYPE_ERROR:
		{
			statusStr = _T("Failed to set the tasks logon type");
		}
		break;
		case SS_SET_TIMEOUT_SUCCESS:
		{
			statusStr = _T("Failed to set the tasks timeout value");
		}
		break;
		case SS_SET_REPETITION_PATTERN_ERROR:
		{
			statusStr = _T("Failed to set tasks repetition pattern");
		}
		break;
		case SS_SET_INTERVAL_ERROR:
		{
			statusStr = _T("Failed to set tasks occurence interval");
		}
		break;
		case SS_SET_TASK_ENABLE_ERROR:
		{
			statusStr = _T("Failed to enable scheduled task");
		}
		break;
		case SS_REGISTER_TASK_ERROR:
		{
			statusStr = _T("Failed to Register Task");
		}
		break;
	    default:
		{
			statusStr = _T("Undefined ScheduleStatus Error");
		}
		break;
	}

	return statusStr;
}

