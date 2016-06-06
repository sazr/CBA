#include "Component.h"

// Class Property Implementation //


// Static Function Implementation //


// Function Implementation //

Component::Component(const std::weak_ptr<IApp>& app) : app(app) {
	printf("abc\n");
    generateGUID(gUid);
}

Component::~Component() {
#pragma message("TODO: how to ensure Component::terminate() is called upon destruction/falling out of scope")
    //terminate(NULL_ARGS);
}

bool Component::operator<(const Component& other) const {
    return compareGUID(gUid, other.gUid);
}

//template<typename T, typename... Params>
//std::shared_ptr<T> Component::addComponent(Params... params) 
//{
//	auto cmp = Component::create<T>(params...);
//    auto res = components.emplace(cmp->gUid, cmp);
//
//	return res.first->second;
//
//	/*return res.second;
//    if (!res.second) {
//            GUID gUid;
//            getNullGUID(&gUid);
//            return gUid;
//    }
//
//    return cmp->gUid;*/
//    //return res.first->second->gUid;
//}
//
//template<typename T>
//std::shared_ptr<T> Component::getComponent(const GUID& gUid) /*const*/ {
//    if (components.find(gUid) == components.end())
//        return nullptr; // std::make_shared<T>();
//
//    return std::static_pointer_cast<T>(components[gUid]);
//}
//
//template<typename T>
//Status Component::registerEventLambda(int evtId, std::function<Status(const IEventArgs& evtArgs) > func) {
//    auto sApp = app.lock();
//    if (!sApp)
//        return S_UNDEFINED_ERROR;
//
//    sApp->registerForEvent(evtId, EventDelegate(func, gUid));
//    return S_SUCCESS;
//}
//
//template<typename T>
//Status Component::registerEvent(const int& evtId, Status(T::*func) (const IEventArgs& evtArgs)) {
//    auto sApp = app.lock();
//    if (!sApp)
//        return S_UNDEFINED_ERROR;
//
//    auto res = std::bind(func, (T*)this, std::placeholders::_1);
//    sApp->registerForEvent(evtId, EventDelegate(res, gUid));
//    return S_SUCCESS;
//}