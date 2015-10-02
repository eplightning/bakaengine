#include "resources/resources.h"

#include "bakaglobal.h"

#include "game/core/registry.h"

using namespace Baka;

Resources::Resources() : sources_()
{
}

Resources::~Resources()
{
	for (auto &x : sources_) {
		delete x;
	}
}

bool Resources::Exists(const char *resource)
{
	auto it = sources_.rbegin();

	for (; it != sources_.rend(); ++it) {
		if ((*it)->Exists(resource))
			return true;
	}

	return false;
}

ResourceData Resources::GetData(const char *resource)
{
	ResourceData data;

	auto it = sources_.rbegin();

	for (; it != sources_.rend(); ++it) {
		if ((*it)->GetData(resource, &data))
			return data;
	}

	data.bytes = nullptr;
	data.size = 0;

	return data;
}

void Resources::RegisterSource(SourceBase *source)
{
	sources_.push_back(source);

	LOG_INFO("Resources: New source registered");
}

void Resources::UnregisterSource(SourceBase *source)
{
	auto it = sources_.begin();

	for (; it != sources_.end(); ++it) {
		if (*it == source) {
			sources_.erase(it);
			return;
		}
	}

	Registry::Instance()->GetLogger()->WriteLine("Resources: Source unregistered");
}