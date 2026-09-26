#define EIGEN_DONT_VECTORIZE
#define EIGEN_MAX_ALIGN_BYTES 0
#define EIGEN_MAX_STATIC_ALIGN_BYTES 0
#define EIGEN_NO_MALLOC

#include <ArduinoEigen/Eigen/Dense>
#include <ArduinoEigen/Eigen/Geometry>

#include <cmath>

using Vec2 = Eigen::Matrix<float, 2, 1>;
using Vec3 = Eigen::Matrix<float, 3, 1>;
using Vec4 = Eigen::Matrix<float, 4, 1>;
using Mat3 = Eigen::Matrix<float, 3, 3>;
using Mat4 = Eigen::Matrix<float, 4, 4>;

volatile float gInput = 1.25F;
volatile float gSink = 0.0F;

static Mat4 Translation(const Vec3& v) {
  Mat4 out = Mat4::Identity();
  out(0, 3) = v.x();
  out(1, 3) = v.y();
  out(2, 3) = v.z();
  return out;
}

static Mat4 Scale(float x, float y, float z) {
  Mat4 out = Mat4::Identity();
  out(0, 0) = x;
  out(1, 1) = y;
  out(2, 2) = z;
  return out;
}

static Mat4 Rotation(float angle, const Vec3& axis) {
  const Vec3 a = axis.normalized();
  const float c = std::cos(angle);
  const float sn = std::sin(angle);
  const float t = 1.0F - c;
  const float x = a.x();
  const float y = a.y();
  const float z = a.z();

  Mat4 out = Mat4::Identity();
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
  const Vec3 forward = (center - eye).normalized();
  const Vec3 side = forward.cross(up).normalized();
  const Vec3 correctedUp = side.cross(forward);

  Mat4 out = Mat4::Identity();
  out(0, 0) = side.x();
  out(0, 1) = side.y();
  out(0, 2) = side.z();
  out(0, 3) = -side.dot(eye);
  out(1, 0) = correctedUp.x();
  out(1, 1) = correctedUp.y();
  out(1, 2) = correctedUp.z();
  out(1, 3) = -correctedUp.dot(eye);
  out(2, 0) = -forward.x();
  out(2, 1) = -forward.y();
  out(2, 2) = -forward.z();
  out(2, 3) = forward.dot(eye);
  return out;
}

static Mat4 Perspective(float fovy, float aspect, float nearPlane, float farPlane) {
  const float f = 1.0F / std::tan(fovy * 0.5F);
  Mat4 out = Mat4::Zero();
  out(0, 0) = f / aspect;
  out(1, 1) = f;
  out(2, 2) = farPlane / (nearPlane - farPlane);
  out(2, 3) = (farPlane * nearPlane) / (nearPlane - farPlane);
  out(3, 2) = -1.0F;
  return out;
}

int main() {
  const float s = gInput;

  const Vec2 uv{s, s + 0.25F};
  const Vec3 a{s + 0.1F, 2.0F, 3.0F};
  const Vec3 b{4.0F, s + 0.2F, 6.0F};
  const Vec3 n = a.cross(b).normalized();
  const float d = a.dot(b);

  Mat3 m3;
  m3 << 1.0F + s * 0.01F, 0.4F, 0.7F,
        0.2F, 1.5F, 0.8F,
        0.3F, 0.6F, 2.0F;
  const Vec3 mv3 = m3 * a;
  const Mat3 inv3 = m3.inverse();

  const Vec3 translation{1.0F, 2.0F, s};
  const Vec3 axis{1.0F, 2.0F, 3.0F};
  const Mat4 model = Translation(translation) * Rotation(0.4F + s * 0.01F, axis) * Scale(1.2F, 0.8F, 1.1F);
  const Vec3 eye{2.0F, 3.0F, 4.0F + s * 0.01F};
  const Vec3 center{0.0F, 0.0F, 0.0F};
  const Vec3 up{0.0F, 1.0F, 0.0F};
  const Mat4 view = LookAt(eye, center, up);
  const Mat4 projection = Perspective(1.0471975512F + s * 0.001F, 1.6F, 0.1F, 100.0F);
  const Mat4 mvp = projection * view * model;
  const Vec4 clip = mvp * Vec4{a.x(), a.y(), a.z(), 1.0F};

  gSink = uv.x() + uv.y() + n.x() + n.y() + d + mv3.x() + inv3(0, 0) + clip.x() + clip.y() + clip.z() + clip.w();
  return gSink == 0.0F ? 1 : 0;
}
