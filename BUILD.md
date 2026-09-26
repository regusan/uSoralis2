# ビルド

現段階ではHost上のBMPサンプルのみを対象とする。PythonとPico SDKは不要。

## Ubuntu / WSL

必要なツールを導入する。

```bash
sudo apt update
sudo apt install build-essential cmake ninja-build clang-format clang-tidy
```

Debugビルドとテストを実行する。

```bash
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
```

Releaseビルドは次の通り。

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

`clang-tidy`を有効にしてビルドする。

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
