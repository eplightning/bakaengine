#ifndef GAME_CORE_REGISTRY_H_
#define GAME_CORE_REGISTRY_H_

#include <unordered_map>

#include "bakaglobal.h"

#include "game/event/eventmanager.h"
#include "config/config.h"
#include "logging/logger.h"
#include "resources/resources.h"
#include "graphics/imagesupport.h"
#include "game/core/actions.h"
#include "game/core/input.h"

namespace Baka
{
	class Registry {
	public:
		Registry();
		~Registry();

		Registry(const Registry &copy) = delete;
		void operator=(const Registry &copy) = delete;

		inline static Registry* Instance()
		{
			return instance_;
		}

		// generic setter/getter
		void* Get(const String &name) const;
		void Set(const String &name, void *object);

		// specific service setters/getters
		inline EventManager* GetEventManager() const
		{
			return obj_eventmanager_;
		}

		inline Config* GetConfig() const
		{
			return obj_mainconf_;
		}

		inline Logger* GetLogger() const
		{
			return obj_logger_;
		}

		inline Resources* GetResources() const
		{
			return obj_resources_;
		}

		inline ImageSupport* GetImageSupport() const
		{
			return obj_imgsupport_;
		}

		inline Actions* GetActions() const
		{
			return obj_actions_;
		}

		inline Input* GetInput() const
		{
			return obj_input_;
		}

		void SetEventManager(EventManager *object);
		void SetConfig(Config *object);
		void SetLogger(Logger *object);
		void SetResources(Resources *object);
		void SetImageSupport(ImageSupport *object);
		void SetActions(Actions *object);
		void SetInput(Input *object);

	private:
		static Registry *instance_;
		std::unordered_map<String, void*> objects_;

		EventManager *obj_eventmanager_;
		Config *obj_mainconf_;
		Logger *obj_logger_;
		Resources *obj_resources_;
		ImageSupport *obj_imgsupport_;
		Actions *obj_actions_;
		Input *obj_input_;
	};
} // namespace Baka

#endif