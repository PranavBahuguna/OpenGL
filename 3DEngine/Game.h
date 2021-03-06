#pragma once

#include "BasicDefines.h"
#include "Camera.h"
#include "UiOverlay.h"
#include "Window.h"

#include <memory>

// Camera statics
static constexpr float CAMERA_MOVE_SPEED = 5.0f;
static constexpr float CAMERA_TURN_SPEED = DEG2RAD(60.0f);
static constexpr float CAMERA_ZOOM_SPEED = DEG2RAD(25.0f);
static constexpr float CAMERA_MIN_ANGLE = DEG2RAD(-180.f);
static constexpr float CAMERA_MAX_ANGLE = DEG2RAD(180.f);
static constexpr float CAMERA_MIN_FOV = DEG2RAD(10.0f);
static constexpr float CAMERA_MAX_FOV = DEG2RAD(120.0f);
static constexpr float CAMERA_NEAR_PLANE = 0.1f;
static constexpr float CAMERA_FAR_PLANE = 100.0f;

// Camera setup params
static constexpr auto CAMERA_SETUP_POS = glm::vec3(0.0f, 0.0f, 0.0f);
static constexpr float CAMERA_SETUP_PITCH = DEG2RAD(0.0f);
static constexpr float CAMERA_SETUP_YAW = DEG2RAD(180.0f);
static constexpr float CAMERA_SETUP_ROLL = DEG2RAD(0.0f);
static constexpr auto CAMERA_SETUP_ROTATION =
    glm::vec3(CAMERA_SETUP_PITCH, CAMERA_SETUP_YAW, CAMERA_SETUP_ROLL);
static constexpr float CAMERA_SETUP_FOV = DEG2RAD(45.0f);

// Window statics
static constexpr int WINDOW_WIDTH = 2000;
static constexpr int WINDOW_HEIGHT = 1500;
#ifdef _DEBUG
static constexpr bool USE_WINDOWED = true;
#else
static constexpr bool USE_WINDOWED = false;
#endif
static constexpr bool FULLSCREEN_WINDOWS = false;

// Game options
static constexpr float FPS_UPDATE_DELAY = 0.5f;
static constexpr size_t FPS_BUFFER_SIZE = 8;
static constexpr int MAX_LIGHTS = 8;
static constexpr bool USE_DEPTH_VISUALISATION = false;
static constexpr GLuint SHADOW_WIDTH = 4096;
static constexpr GLuint SHADOW_HEIGHT = 4096;

// Text options
static constexpr auto HUD_FONT = "Unreal";
static constexpr auto COLOR_SEAWEED = glm::vec4(0.0392f, 0.4941f, 0.549f, 1.0f);
static constexpr auto COLOR_RED = glm::vec4(0.651f, 0.1725f, 0.1686f, 1.0f);
static constexpr auto COLOR_GREEN = glm::vec4(0.1608f, 0.4314f, 0.0039f, 1.0f);
static constexpr auto COLOR_BLUE = glm::vec4(0.1961f, 0.3216f, 0.4824f, 1.0f);
static constexpr auto COLOR_YELLOW = glm::vec4(0.9922f, 0.80f, 0.051f, 1.0f);
static constexpr auto COLOR_VIOLET = glm::vec4(0.3569f, 0.0392f, 0.5686f, 1.0f);
static constexpr auto COLOR_GREY = glm::vec4(0.6667f, 0.6627f, 0.6784f, 1.0f);

class Game {
public:
  static void Init();
  static void Exit();
  static void ToggleSceneUpdateStatus();
  static void Update();

  static Camera &GetCamera();
  static Window &GetWindow();
  static UiOverlay &GetUiOverlay();
  static bool ShouldUpdateScene();

private:
  Game() {} // prevent construction of this class

  static std::unique_ptr<Camera> _camera;
  static std::unique_ptr<UiOverlay> _uiOverlay;
  static std::unique_ptr<Window> _window;

  static bool _updateScene;
};