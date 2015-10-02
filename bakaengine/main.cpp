#include <exception>

#include "bakaglobal.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "game/core/utils.h"
#include "game/core/registry.h"
#include "config/configini.h"
#include "logging/loggerfile.h"
#include "logging/loggernull.h"
#include "resources/resources.h"
#include "resources/sourcefilesystem.h"
#include "graphics/imagesupport.h"
#include "game/event/eventmanager.h"
#include "game/event/eventdispatcherstatic.h"
#include "game/core/actions.h"
#include "game/core/window.h"
#include "game/core/input.h"

#include "lostislands/core/gamemanager.h"

using namespace Baka;

#define BAKA_FIXED_STEP_FPS 60.0
#define BAKA_FIXED_STEP_MAX 5

int main(int argc, char *argv[])
{
	// engine core initialization
	Utils::PlatformInit(argc, argv);

	UniquePtr<Registry> registry(new Registry);
	Registry *reg = registry.get();

	reg->SetConfig(new ConfigIni("baka.ini"));
	Config *conf = reg->GetConfig();

	if (conf->GetBool("Logging", "Enabled", true)) {
		reg->SetLogger(new LoggerFile(conf->GetString("Logging", "Filename", "baka.log")));
	} else {
		reg->SetLogger(new LoggerNull);
	}

	Logger *logger = reg->GetLogger();
	logger->WriteLine("Baka Engine %s codename %s | Build Date: %s %s", ENGINE_VERSION, ENGINE_CODENAME, __DATE__, __TIME__);

	reg->SetResources(new Resources);
	Resources *res = reg->GetResources();
	res->RegisterSource(new SourceFilesystem("data/"));

	reg->SetEventManager(new EventManager);
	EventManager *ev = reg->GetEventManager();
	EventDispatcherStatic *evstatic = new EventDispatcherStatic;
	ev->RegisterDispatcher("static", evstatic);

	logger->WriteLine("Engine core succesfully initialized");

	// GLFW
	Window *wnd_obj = new Window(
		conf->GetInt("Screen", "ResolutionX", 800),
		conf->GetInt("Screen", "ResolutionY", 600),
		conf->GetBool("Screen", "Fullscreen", false),
		conf->GetBool("Screen", "VSync", true),
		ENGINE_DEFAULT_WINDOW_TITLE);

	reg->Set("window", wnd_obj);
	GLFWwindow *wnd = wnd_obj->GetWindow();

	if (!wnd_obj->GetStatus()) {
		logger->WriteLine("Couldn't initialize GLFW");
		return 1;
	}

	// GLEW init
	glewExperimental = GL_TRUE;
	GLenum status = glewInit();

	if (status != GLEW_OK) {
		logger->WriteLine("Couldn't initialize OpenGL Extension Wrangler, status code: %d", status);
		return 1;
	}

	if (!GLEW_VERSION_3_3) {
		logger->WriteLine("OpenGL 3.3 or newer is required");
		return 1;
	}

	logger->WriteLine("Window and OpenGL context creation finished succesfully");

	reg->SetImageSupport(new ImageSupport);
	reg->SetActions(new Actions);

	// input
	reg->SetInput(new Input(wnd));
	Input *input = reg->GetInput();
	input->SetDefaultBinding(conf->GetKeyMap("BindingDefault"));

	logger->WriteLine("Input system initialized, engine initialization done ...");

	// game
	try {
		LiGameManager gm;

		// main loop
		double prev_time = glfwGetTime();
		double current_time, dt;
		double accumulator = 0.0;
		int fixed_frames;

		while (!glfwWindowShouldClose(wnd)) {
			// glfw events
			glfwPollEvents();

			// delta time
			current_time = glfwGetTime();
			dt = current_time - prev_time;
			prev_time = current_time;

			// fixed step
			accumulator += dt;
			fixed_frames = 0;

			while (accumulator >= (1 / BAKA_FIXED_STEP_FPS) && fixed_frames < BAKA_FIXED_STEP_MAX) {
				accumulator -= (1 / BAKA_FIXED_STEP_FPS);
				fixed_frames++;
			}

			if (fixed_frames > 0)
				gm.FixedStep(1 / BAKA_FIXED_STEP_FPS * fixed_frames);

			// variable step
			gm.VariableStep(dt, accumulator);

			// swap buffers
			glfwSwapBuffers(wnd);
		}
	}
	catch (std::exception &e) {
		glfwSetWindowShouldClose(wnd, 1);
		Utils::DisplayCriticalError(e.what());
	}
	catch (std::exception *e) {
		glfwSetWindowShouldClose(wnd, 1);
		Utils::DisplayCriticalError(e->what());
	}

	return 0;
}