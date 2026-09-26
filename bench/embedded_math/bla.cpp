#include <BasicLinearAlgebra.h>

#include <cmath>

using Vec2 = BLA::Matrix<2, 1, float>;
using Vec3 = BLA::Matrix<3, 1, float>;
using Vec4 = BLA::Matrix<4, 1, float>;
using Mat3 = BLA::Matrix<3, 3, float>;
using Mat4 = BLA::Matrix<4, 4, float>;

volatile float gInput = 1.25F;
volatile float gSink = 0.0F;

static Vec3 Normalize(const Vec3& v) {
  const float invLength = 1.0F / std::sqrt(BLA::DotProduct(v, v));
  Vec3 out;
  out(0) = v(0) * invLength;
  out(1) = v(1) * invLength;
  out(2) = v(2) * invLength;
  return out;
}

static Mat4 Identity4() {
  Mat4 out;
  out.Fill(0.0F);
  for (int i = 0; i < 4; ++i) out(i, i) = 1.0F;
  return out;
}

static Mat4 Translation(const Vec3& v) {
  Mat4 out = Identity4();
  out(0, 3) = v(0);
  out(1, 3) = v(1);
  out(2, 3) = v(2);
  return out;
}

static Mat4 Scale(float x, float y, float z) {
  Mat4 out = Identity4();
  out(0, 0) = x;
  out(1, 1) = y;
  out(2, 2) = z;
  return out;
}

static Mat4 Rotation(float angle, const Vec3& axis) {
  const Vec3 a = Normalize(axis);
  const float c = std::cos(angle);
  const float sn = std::sin(angle);
  const float t = 1.0F - c;
  const float x = a(0);
  const float y = a(1);
  const float z = a(2);

  Mat4 out = Identity4();
  out(0, 0) = c + x * x * t;
  out(0, 1) = x * y * t - z * sn;
  out(0, 2) = x * z * t + y * sn;
  out(1, 0) = y * x * t + z * sn;
  out(1, 1) = c + y * y * t;
  out(1, 2) = y * z * t - x * sn;
  out(2, 0) = z * x * t - y * sn;
  out(2, 1) = z * y * t + x * sn;
  out(2, 2) = c + z * z * t;
  return out;
}

static Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
  Vec3 forward;
  for (int i = 0; i < 3; ++i) forward(i) = center(i) - eye(i);
  forward = Normalize(forward);
  const Vec3 side = Normalize(BLA::CrossProduct(forward, up));
  const Vec3 correctedUp = BLA::CrossProduct(side, forward);

  Mat4 out = Identity4();
  out(0, 0) = side(0);
  out(0, 1) = side(1);
  out(0, 2) = side(2);
  out(0, 3) = -BLA::DotProduct(side, eye);
  out(1, 0) = correctedUp(0);
  out(1, 1) = correctedUp(1);
  out(1, 2) = correctedUp(2);
  out(1, 3) = -BLA::DotProduct(correctedUp, eye);
  out(2, 0) = -forward(0);
  out(2, 1) = -forward(1);
  out(2, 2) = -forward(2);
  out(2, 3) = BLA::DotProduct(forward, eye);
  return out;
}

static Mat4 Perspective(float fovy, float aspect, float nearPlane, float farPlane) {
  const float f = 1.0F / std::tan(fovy * 0.5F);
  Mat4 out;
  out.Fill(0.0F);
  out(0, 0) = f / aspect;
  out(1, 1) = f;
  out(2, 2) = farPlane / (nearPlane - farPlane);
  out(2, 3) = (farPlane * nearPlane) / (nearPlane - farPlane);
  out(3, 2) = -1.0F;
  return out;
}

int main() {
  const float s = gInput;

  Vec2 uv;
  uv(0) = s;
  uv(1) = s + 0.25F;
  Vec3 a;
  a(0) = s + 0.1F;
  a(1) = 2.0F;
  a(2) = 3.0F;
  Vec3 b;
  b(0) = 4.0F;
  b(1) = s + 0.2F;
  b(2) = 6.0F;
  const Vec3 n = Normalize(BLA::CrossProduct(a, b));
  const float d = BLA::DotProduct(a, b);

  Mat3 m3;
  m3(0, 0) = 1.0F + s * 0.01F; m3(0, 1) = 0.4F; m3(0, 2) = 0.7F;
  m3(1, 0) = 0.2F; m3(1, 1) = 1.5F; m3(1, 2) = 0.8F;
  m3(2, 0) = 0.3F; m3(2, 1) = 0.6F; m3(2, 2) = 2.0F;
  const Vec3 mv3 = m3 * a;
  const Mat3 inv3 = BLA::Inverse(m3);

  Vec3 translation;
  translation(0) = 1.0F; translation(1) = 2.0F; translation(2) = s;
  Vec3 axis;
  axis(0) = 1.0F; axis(1) = 2.0F; axis(2) = 3.0F;
  const Mat4 model = Translation(translation) * Rotation(0.4F + s * 0.01F, axis) * Scale(1.2F, 0.8F, 1.1F);

  Vec3 eye;
  eye(0) = 2.0F; eye(1) = 3.0F; eye(2) = 4.0F + s * 0.01F;
  Vec3 center;
  center.Fill(0.0F);
  Vec3 up;
  up(0) = 0.0F; up(1) = 1.0F; up(2) = 0.0F;
  const Mat4 view = LookAt(eye, center, up);
  const Mat4 projection = Perspective(1.0471975512F + s * 0.001F, 1.6F, 0.1F, 100.0F);
  const Mat4 mvp = projection * view * model;
  Vec4 position;
  position(0) = a(0); position(1) = a(1); position(2) = a(2); position(3) = 1.0F;
  const Vec4 clip = mvp * position;

  gSink = uv(0) + uv(1) + n(0) + n(1) + d + mv3(0) + inv3(0, 0) + clip(0) + clip(1) + clip(2) + clip(3);
  return gSink == 0.0F ? 1 : 0;
}
