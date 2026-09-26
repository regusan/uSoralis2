#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_RIGHT_HANDED

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

volatile float gInput = 1.25F;
volatile float gSink = 0.0F;

int main() {
  const float s = gInput;

  const glm::vec2 uv{s, s + 0.25F};
  const glm::vec3 a{s + 0.1F, 2.0F, 3.0F};
  const glm::vec3 b{4.0F, s + 0.2F, 6.0F};
  const glm::vec3 n = glm::normalize(glm::cross(a, b));
  const float d = glm::dot(a, b);

  const glm::mat3 m3{
      1.0F + s * 0.01F, 0.2F, 0.3F,
      0.4F, 1.5F, 0.6F,
      0.7F, 0.8F, 2.0F,
  };
  const glm::vec3 mv3 = m3 * a;
  const glm::mat3 inv3 = glm::inverse(m3);

  glm::mat4 model{1.0F};
  model = glm::translate(model, glm::vec3{1.0F, 2.0F, s});
  model = glm::rotate(model, 0.4F + s * 0.01F, glm::normalize(glm::vec3{1.0F, 2.0F, 3.0F}));
  model = glm::scale(model, glm::vec3{1.2F, 0.8F, 1.1F});

  const glm::mat4 view = glm::lookAtRH(
      glm::vec3{2.0F, 3.0F, 4.0F + s * 0.01F},
      glm::vec3{0.0F, 0.0F, 0.0F},
      glm::vec3{0.0F, 1.0F, 0.0F});
  const glm::mat4 projection = glm::perspectiveRH_ZO(1.0471975512F + s * 0.001F, 1.6F, 0.1F, 100.0F);
  const glm::mat4 mvp = projection * view * model;
  const glm::vec4 clip = mvp * glm::vec4{a, 1.0F};

  gSink = uv.x + uv.y + n.x + n.y + d + mv3.x + inv3[0][0] + clip.x + clip.y + clip.z + clip.w;
  return gSink == 0.0F ? 1 : 0;
}
