#include "lve_model.hpp"
#include <memory>

namespace lve {

struct Transform2dComponent {
    glm::vec2 translation{};

    glm::mat2 mat2() { return glm::mat2(1.0f); }
};

class LveGameObject {
  public:
    using id_t = unsigned int;

    static LveGameObject createGameObject() {
        static id_t currentId = 0;
        return LveGameObject{currentId++};
    }

    LveGameObject(const LveGameObject&) = delete;
    LveGameObject& operator=(const LveGameObject&) = delete;
    LveGameObject(LveGameObject&&) noexcept = default;
    LveGameObject& operator=(LveGameObject&&) noexcept = default;

    id_t getId() const { return id; }
    std::shared_ptr<LveModel> model{};
    glm::vec3 color{};
    Transform2dComponent transform2d{};

  private:
    LveGameObject(id_t objId) : id(objId) {}

    id_t id;
};
} // namespace lve
