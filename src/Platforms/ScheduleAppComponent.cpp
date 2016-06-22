#include "ScheduleAppComponent.h"

// Class Property Implementation //


// Static Function Implementation //


// Function Implementation //
ScheduleAppComponent::ScheduleAppComponent(const std::weak_ptr<IApp>& app, const tstring taskName) 
	: Component(app), TASK_NAME(taskName)
{
	registerEvents();
}

ScheduleAppComponent::~ScheduleAppComponent()
{

}

Status ScheduleAppComponent::init(const IEventArgs& evtArgs)
{
	const WinEventArgs& args = static_cast<const WinEventArgs&>(evtArgs);
	
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
