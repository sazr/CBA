#ifndef CBA_COMPONENT_H
#define CBA_COMPONENT_H

#include "CBA.h"
#include "GUID.h"
#include "Status.h"
#include "EventDelegate.h"
#include "Interfaces/IApp.h"

class IApp;

class Component
{
public:
	GUID gUid;

	template<typename T, typename... Params>
	static std::shared_ptr<T> create(Params... params)
	{
		return std::shared_ptr<T>(new T(params...));
	}

	virtual ~Component();
	Component(const Component& other) = delete;
	Component(Component&& other) = delete;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) = delete;

	bool operator< (const Component& other) const;

	template<typename T, typename... Params>
	std::shared_ptr<T> addComponent(Params... params)
	{
		auto cmp = Component::create<T>(params...);
		auto res = components.emplace(cmp->gUid, cmp);

		if (!res.second)
			return std::shared_ptr<T>();

		return cmp;
	}

	template<typename T>
	std::shared_ptr<T> getComponent(const GUID& gUid) /*const*/ 
	{
		if (components.find(gUid) == components.end())
			return nullptr; // std::make_shared<T>();

		return std::static_pointer_cast<T>(components[gUid]);
	}

	template<typename T>
	Status registerEventLambda(int evtId, std::function<Status(const IEventArgs& evtArgs) > func) 
	{
		auto sApp = app.lock();
		if (!sApp)
			return S_UNDEFINED_ERROR;

		sApp->registerForEvent(evtId, EventDelegate(func, gUid));
		return S_SUCCESS;
	}
        
protected:
	std::weak_ptr<IApp> app;
	
	Component(const std::weak_ptr<IApp>& app);

	virtual Status init(const IEventArgs& evtArgs) = 0;
	virtual Status terminate(const IEventArgs& evtArgs) = 0;
	virtual Status registerEvents() = 0;

	template<typename T>
	Status registerEvent(const int& evtId, Status(T::*func) (const IEventArgs& evtArgs)) 
	{
		auto sApp = app.lock();
		if (!sApp)
			return S_UNDEFINED_ERROR;

		auto res = std::bind(func, (T*)this, std::placeholders::_1);
		sApp->registerForEvent(evtId, EventDelegate(res, gUid));
		return S_SUCCESS;
	}

private:
	std::unordered_map<GUID, std::shared_ptr<Component>> components;
};


namespace std
{
	template<>
	struct hash<Component>
	{
		std::size_t operator()(const Component& cmp) const
		{
			using std::size_t;
			using std::hash;

			return hash<GUID>()(cmp.gUid);
		}
	};
}

#endif // CBA_COMPONENT_H

