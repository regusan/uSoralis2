#define TRM3D_NO_IOSTREAM
#include <TinyReguMath3D.hpp>

volatile float gInput = 1.25F;
volatile float gSink = 0.0F;

int main() {
  const float s = gInput;

  const trm3d::vec2f uv{s, s + 0.25F};
  const trm3d::vec3f a{s + 0.1F, 2.0F, 3.0F};
  const trm3d::vec3f b{4.0F, s + 0.2F, 6.0F};
  const trm3d::vec3f n = trm3d::normalize(trm3d::cross(a, b));
  const float d = trm3d::dot(a, b);

  const trm3d::mat3f m3{
      trm3d::vec3f{1.0F + s * 0.01F, 0.2F, 0.3F},
      trm3d::vec3f{0.4F, 1.5F, 0.6F},
      trm3d::vec3f{0.7F, 0.8F, 2.0F},
  };
  const trm3d::vec3f mv3 = m3 * a;
  const trm3d::mat3f inv3 = trm3d::inverse(m3);

  trm3d::mat4f model{1.0F};
  model = trm3d::translate(model, trm3d::vec3f{1.0F, 2.0F, s});
  model = trm3d::rotate(model, 0.4F + s * 0.01F, trm3d::normalize(trm3d::vec3f{1.0F, 2.0F, 3.0F}));
  model = trm3d::scale(model, trm3d::vec3f{1.2F, 0.8F, 1.1F});

  const trm3d::mat4f view = trm3d::lookAt(
      trm3d::vec3f{2.0F, 3.0F, 4.0F + s * 0.01F},
      trm3d::vec3f{0.0F, 0.0F, 0.0F},
      trm3d::vec3f{0.0F, 1.0F, 0.0F});
  const trm3d::mat4f projection = trm3d::perspective(1.0471975512F + s * 0.001F, 1.6F, 0.1F, 100.0F);
  const trm3d::mat4f mvp = projection * view * model;
  const trm3d::vec4f clip = mvp * trm3d::vec4f{a.x, a.y, a.z, 1.0F};

  gSink = uv.x + uv.y + n.x + n.y + d + mv3.x + inv3[0][0] + clip.x + clip.y + clip.z + clip.w;
  return gSink == 0.0F ? 1 : 0;
}
