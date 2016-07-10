/*
Copyright (c) 2016 Sam Zielke-Ryner All rights reserved.

For job opportunities or to work together on projects please contact
myself via Github:   https://github.com/sazr

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

1. Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.

2. The source code, API or snippets cannot be used for commercial
purposes without written consent from the author.

THIS SOFTWARE IS PROVIDED ``AS IS''
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "ScheduleAppComponent.h"

// Class Property Implementation //


// Static Function Implementation //


// Function Implementation //
ScheduleAppComponent::ScheduleAppComponent(const std::weak_ptr<IApp>& app, const tstring taskName) 
	: Component(app), TASK_NAME(taskName)
{
	TCHAR username[MAX_PATH];
	DWORD username_len = MAX_PATH;
	GetUserName(username, &username_len);

	TASK_NAME += username;
	registerEvents();
}

ScheduleAppComponent::~ScheduleAppComponent()
{

}

Status ScheduleAppComponent::init(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	
	unregisterScheduledTask();

	// check if task already scheduled
	auto regTasks = EasyTaskScheduler::RetrieveScheduledTasks();
	bool isTaskRegistered = std::find_if(regTasks.begin(), regTasks.end(), [&](const tstring& taskName) {
		output(_T("Name: %s\n"), taskName.c_str());
		return taskName.find(TASK_NAME) == TASK_NAME.length() + 1;
	}) != regTasks.end();

	if (isTaskRegistered) {
		output(_T("already registered\n"));
		return S_SUCCESS;
	}

	TCHAR exePath[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL), exePath, MAX_PATH);

	output(_T("Task: %s, %s, %s\n"), TASK_NAME.c_str(), exePath, _T("2015-06-14T13:40:00"));
	auto res = EasyTaskScheduler::ScheduleTaskAtLogon(TASK_NAME, tstring(exePath), _T("2015-06-14T13:40:00"));
	//auto res = EasyTaskScheduler::ScheduleTaskAtStartup(TASK_NAME, tstring(exePath), _T("2015-06-14T13:40:00"));
	tstring resStr = EasyTaskScheduler::ErrorToString(res);
	output(_T("add res: %d, %s\n"), res, resStr);
	
	return S_SUCCESS;
}

Status ScheduleAppComponent::terminate(const IEventArgs& evtArgs)
{
	return S_SUCCESS;
}

Status ScheduleAppComponent::registerEvents()
{
	registerEvent(WM_CREATE, &ScheduleAppComponent::init);
	//registerEvent(WM_CLOSE, &ScheduleAppComponent::terminate);
	return S_SUCCESS;
}

Status ScheduleAppComponent::unregisterScheduledTask()
{
	auto res = EasyTaskScheduler::RemoveTask(TASK_NAME);
	output(_T("remove res: %d\n"), res); 
	return S_SUCCESS;
}
