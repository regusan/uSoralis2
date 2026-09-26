volatile float gInput = 1.25F;
volatile float gSink = 0.0F;

int main() {
  const float value = gInput;
  gSink = value * 1.01F + 0.5F;
  return gSink < 0.0F ? 1 : 0;
}
