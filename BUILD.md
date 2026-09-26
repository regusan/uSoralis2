# ビルド

現段階ではHost上のBMPサンプルのみを対象とする。PythonとPico SDKは不要。

## 対象環境

Ubuntu 24.04またはWSL2上のUbuntu 24.04を基準環境とする。

- C++23
- GCC / libstdc++で通常ビルド
- Clang 22の`clang-format`と`clang-tidy`を使用
- CMake + Ninja

## Ubuntu / WSLの環境構築

基本ツールを導入する。

```bash
sudo apt update
sudo apt install -y build-essential cmake ninja-build ca-certificates wget
```

Clang 22はLLVM公式APTリポジトリから導入する。

```bash
wget -qO- https://apt.llvm.org/llvm-snapshot.gpg.key \
  | sudo tee /etc/apt/trusted.gpg.d/apt.llvm.org.asc >/dev/null

echo "deb https://apt.llvm.org/noble/ llvm-toolchain-noble-22 main" \
  | sudo tee /etc/apt/sources.list.d/llvm22.list

sudo apt update
sudo apt install -y clang-format-22 clang-tidy-22
```

導入されたバージョンを確認する。

```bash
clang-format-22 --version
clang-tidy-22 --version
```

CMakeは`clang-format-22`と`clang-tidy-22`を優先して検出する。Clang 18向けに使用していたlibc++は不要。

## リポジトリ取得

```bash
git clone https://github.com/regusan/uSoralis2.git
cd uSoralis2
```

VS Codeを使用する場合は、このディレクトリをWSL側から開く。推奨拡張機能は`.vscode/extensions.json`に定義している。

## Debugビルドとテスト

```bash
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

## Releaseビルド

```bash
cmake --preset release
cmake --build --preset release
```

## Clangツール

フォーマットする。

```bash
cmake --build --preset debug --target format
```

フォーマット差分がないことを確認する。

```bash
cmake --build --preset debug --target format-check
```

`clang-tidy`を有効にしてビルドする。通常ビルドと同じlibstdc++を解析する。

```bash
cmake --preset lint
cmake --build --preset lint
```

## BMPサンプル

Debugビルド後に次を実行する。

```bash
./build/debug/example/bmp/usoralis-bmp output.bmp
```

256x256のグラデーションBMPが`output.bmp`へ出力される。
