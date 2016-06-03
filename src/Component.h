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
	bool /*GUID*/ addComponent(Params... params);

	template<typename T>
	std::shared_ptr<T> getComponent(const GUID& gUid);

	template<typename T>
	Status registerEventLambda(int evtId, std::function<Status(const IEventArgs& evtArgs)> func);
        
protected:
	std::weak_ptr<IApp> app;
	
	Component(const std::weak_ptr<IApp>& app);

	virtual Status init(const IEventArgs& evtArgs) = 0;
	virtual Status terminate(const IEventArgs& evtArgs) = 0;
	virtual Status registerEvents() = 0;

	template<typename T>
	Status registerEvent(const int& evtId, Status (T::*func) (const IEventArgs& evtArgs));

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

