#include "Window.h"
#include "Timer.h"
#include "Renderer.h"
#include "Sphere.h"
#include "Quad.h"
#include "PlatformUtils.h"
#include "InputManager.h"

#include <cmath>

static constexpr auto WINDOW_WIDTH = 800;
static constexpr auto WINDOW_HEIGHT = 600;

int main(int argc, char** argv) {
	using namespace luna;
	Window window("Luna - Ray Traced Cities", WINDOW_WIDTH, WINDOW_HEIGHT, true, false);
	InputManager::initialize(window);
	auto& inputManager = InputManager::getInstance();
	Renderer renderer(WINDOW_WIDTH, WINDOW_HEIGHT);
	Timer timer;

	// Create the camera
	Camera camera { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } };

	// Create objects for the scene
	std::vector<std::unique_ptr<Object>> objects;
	objects.emplace_back(std::make_unique<Sphere>(glm::vec3(1.5f, 0.0f, 5.0f), 0.3f));
	objects.emplace_back(std::make_unique<Sphere>(glm::vec3(0.0f, 0.0f, 5.0f), 1.0f));
	objects.emplace_back(std::make_unique<Quad>(glm::vec3(0.0f, -1.5f, 5.0f), glm::vec3(0.0f, -1.0f, 0.0f), 3.0f));

	// Create lights for the scene
	std::vector<PointLight> lights;
	lights.emplace_back(
		glm::vec3(1.3f, 0.0f, 5.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		0.7f, 0.2f);

	Scene scene(camera, std::move(objects), lights);
	while (!window.shouldClose()) {
		timer.tick();
		window.pollEvents();
		renderer.clear();

		// Spin around the light
		auto time = PlatformUtils::getTime();
		auto x = std::sin(time) * 1.2f;
		auto y = -0.5f;
		auto z = (std::cos(time) * 1.2f) + 5.0f;
		scene.lights[0].position = { x, y, z };

		renderer.render(scene);
		window.swapBuffers();
	}
	return 0;
}