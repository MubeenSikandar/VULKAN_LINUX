#pragma once
#include "lve_gameobject.hpp"

namespace lve {
class KeyboardMovementController {
public:
  struct KeyMapping {
    int moveLeft = GLFW_KEY_A;
    int moveRight = GLFW_KEY_D;
    int moveForward = GLFW_KEY_W;
    int moveBackward = GLFW_KEY_S;
    int moveUp = GLFW_KEY_E;
    int moveDown = GLFW_KEY_Q;
    int lookLeft = GLFW_KEY_LEFT;
    int lookRight = GLFW_KEY_RIGHT;
    int lookUp = GLFW_KEY_UP;
    int lookDown = GLFW_KEY_DOWN;
  };

  void moveInPlaneXZ(GLFWwindow *window, float deltaTime,
                     LveGameObject &gameObject) const;

  KeyMapping keys{};
  float movementSpeed{3.0f};
  float lookSpeed{1.0f};
};
} // namespace lve
