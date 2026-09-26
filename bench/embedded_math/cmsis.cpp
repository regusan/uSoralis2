#include <arm_math.h>

#include <cmath>

struct Vec2 { float v[2]; };
struct Vec3 { float v[3]; };
struct Vec4 { float v[4]; };
struct Mat3 { float v[9]; };
struct Mat4 { float v[16]; };

volatile float gInput = 1.25F;
volatile float gSink = 0.0F;

static float Dot(const Vec3& a, const Vec3& b) {
  float out = 0.0F;
  arm_dot_prod_f32(a.v, b.v, 3, &out);
  return out;
}

static Vec3 Cross(const Vec3& a, const Vec3& b) {
  return {{
      a.v[1] * b.v[2] - a.v[2] * b.v[1],
      a.v[2] * b.v[0] - a.v[0] * b.v[2],
      a.v[0] * b.v[1] - a.v[1] * b.v[0],
  }};
}

static Vec3 Normalize(const Vec3& v) {
  const float invLength = 1.0F / std::sqrt(Dot(v, v));
  return {{v.v[0] * invLength, v.v[1] * invLength, v.v[2] * invLength}};
}

static Mat4 Identity4() {
  return {{
      1.0F, 0.0F, 0.0F, 0.0F,
      0.0F, 1.0F, 0.0F, 0.0F,
      0.0F, 0.0F, 1.0F, 0.0F,
      0.0F, 0.0F, 0.0F, 1.0F,
  }};
}

static Mat4 Multiply(const Mat4& a, const Mat4& b) {
  Mat4 out{};
  arm_matrix_instance_f32 ma{4, 4, const_cast<float*>(a.v)};
  arm_matrix_instance_f32 mb{4, 4, const_cast<float*>(b.v)};
  arm_matrix_instance_f32 mo{4, 4, out.v};
  arm_mat_mult_f32(&ma, &mb, &mo);
  return out;
}

static Vec4 Multiply(const Mat4& a, const Vec4& b) {
  Vec4 out{};
  arm_matrix_instance_f32 ma{4, 4, const_cast<float*>(a.v)};
  arm_matrix_instance_f32 mb{4, 1, const_cast<float*>(b.v)};
  arm_matrix_instance_f32 mo{4, 1, out.v};
  arm_mat_mult_f32(&ma, &mb, &mo);
  return out;
}

static Vec3 Multiply(const Mat3& a, const Vec3& b) {
  Vec3 out{};
  arm_matrix_instance_f32 ma{3, 3, const_cast<float*>(a.v)};
  arm_matrix_instance_f32 mb{3, 1, const_cast<float*>(b.v)};
  arm_matrix_instance_f32 mo{3, 1, out.v};
  arm_mat_mult_f32(&ma, &mb, &mo);
  return out;
}

static Mat3 Inverse(const Mat3& input) {
  Mat3 source = input;
  Mat3 out{};
  arm_matrix_instance_f32 src{3, 3, source.v};
  arm_matrix_instance_f32 dst{3, 3, out.v};
  arm_mat_inverse_f32(&src, &dst);
  return out;
}

static Mat4 Translation(const Vec3& v) {
  Mat4 out = Identity4();
  out.v[3] = v.v[0];
  out.v[7] = v.v[1];
  out.v[11] = v.v[2];
  return out;
}

static Mat4 Scale(float x, float y, float z) {
  Mat4 out = Identity4();
  out.v[0] = x;
  out.v[5] = y;
  out.v[10] = z;
  return out;
}

static Mat4 Rotation(float angle, const Vec3& axis) {
  const Vec3 a = Normalize(axis);
  const float c = std::cos(angle);
  const float sn = std::sin(angle);
  const float t = 1.0F - c;
  const float x = a.v[0];
  const float y = a.v[1];
  const float z = a.v[2];

  Mat4 out = Identity4();
  out.v[0] = c + x * x * t;
  out.v[1] = x * y * t - z * sn;
  out.v[2] = x * z * t + y * sn;
  out.v[4] = y * x * t + z * sn;
  out.v[5] = c + y * y * t;
  out.v[6] = y * z * t - x * sn;
  out.v[8] = z * x * t - y * sn;
  out.v[9] = z * y * t + x * sn;
  out.v[10] = c + z * z * t;
  return out;
}

static Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
  const Vec3 delta{{center.v[0] - eye.v[0], center.v[1] - eye.v[1], center.v[2] - eye.v[2]}};
  const Vec3 forward = Normalize(delta);
  const Vec3 side = Normalize(Cross(forward, up));
  const Vec3 correctedUp = Cross(side, forward);

  Mat4 out = Identity4();
  out.v[0] = side.v[0]; out.v[1] = side.v[1]; out.v[2] = side.v[2]; out.v[3] = -Dot(side, eye);
  out.v[4] = correctedUp.v[0]; out.v[5] = correctedUp.v[1]; out.v[6] = correctedUp.v[2]; out.v[7] = -Dot(correctedUp, eye);
  out.v[8] = -forward.v[0]; out.v[9] = -forward.v[1]; out.v[10] = -forward.v[2]; out.v[11] = Dot(forward, eye);
  return out;
}

static Mat4 Perspective(float fovy, float aspect, float nearPlane, float farPlane) {
  const float f = 1.0F / std::tan(fovy * 0.5F);
  Mat4 out{};
  out.v[0] = f / aspect;
  out.v[5] = f;
  out.v[10] = farPlane / (nearPlane - farPlane);
  out.v[11] = (farPlane * nearPlane) / (nearPlane - farPlane);
  out.v[14] = -1.0F;
  return out;
}

int main() {
  const float s = gInput;

  const Vec2 uv{{s, s + 0.25F}};
  const Vec3 a{{s + 0.1F, 2.0F, 3.0F}};
  const Vec3 b{{4.0F, s + 0.2F, 6.0F}};
  const Vec3 n = Normalize(Cross(a, b));
  const float d = Dot(a, b);

  const Mat3 m3{{
      1.0F + s * 0.01F, 0.4F, 0.7F,
      0.2F, 1.5F, 0.8F,
      0.3F, 0.6F, 2.0F,
  }};
  const Vec3 mv3 = Multiply(m3, a);
  const Mat3 inv3 = Inverse(m3);

  const Vec3 translation{{1.0F, 2.0F, s}};
  const Vec3 axis{{1.0F, 2.0F, 3.0F}};
  const Mat4 model = Multiply(Multiply(Translation(translation), Rotation(0.4F + s * 0.01F, axis)), Scale(1.2F, 0.8F, 1.1F));
  const Vec3 eye{{2.0F, 3.0F, 4.0F + s * 0.01F}};
  const Vec3 center{{0.0F, 0.0F, 0.0F}};
  const Vec3 up{{0.0F, 1.0F, 0.0F}};
  const Mat4 view = LookAt(eye, center, up);
  const Mat4 projection = Perspective(1.0471975512F + s * 0.001F, 1.6F, 0.1F, 100.0F);
  const Mat4 mvp = Multiply(Multiply(projection, view), model);
  const Vec4 clip = Multiply(mvp, Vec4{{a.v[0], a.v[1], a.v[2], 1.0F}});

  gSink = uv.v[0] + uv.v[1] + n.v[0] + n.v[1] + d + mv3.v[0] + inv3.v[0] + clip.v[0] + clip.v[1] + clip.v[2] + clip.v[3];
  return gSink == 0.0F ? 1 : 0;
}
