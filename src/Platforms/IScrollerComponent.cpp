#include "IScrollerComponent.h"

// Class Property Implementation //


// Static Function Implementation //


// Function Implementation //
IScrollerComponent::IScrollerComponent(const std::weak_ptr<IApp>& app, STATE hwndId, ScrollDirection scrollDir)
	: Component(app), hwndId(hwndId), scrollDir(scrollDir), clientScrollPos(0), maxScrollPos(0)
{
	
}

IScrollerComponent::~IScrollerComponent()
{

}
