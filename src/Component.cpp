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

#include "Component.h"

// Class Property Implementation //


// Static Function Implementation //


// Function Implementation //

Component::Component(const std::weak_ptr<IApp>& app) : app(app) {
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