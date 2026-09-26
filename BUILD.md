# ビルド

現段階ではHost上のBMPサンプルのみを対象とする。PythonとPico SDKは不要。

## 対象環境

Ubuntu 24.04またはWSL2上のUbuntu 24.04を基準環境とする。

- C++23
- GCC / libstdc++で通常ビルド
- Clang 22の`clang-format`と`clang-tidy`を使用
- CMake + Ninja

## 初回セットアップ

Gitが未導入なら先にインストールする。

```bash
sudo apt update
sudo apt install -y git
```

リポジトリを取得する。

```bash
git clone https://github.com/regusan/uSoralis2.git
cd uSoralis2
```

開発環境は共通セットアップスクリプトで構築する。

```bash
bash scripts/setup-ubuntu.sh
```

このスクリプトはUbuntu 24.04を確認したうえで、GCC、CMake、Ninja、Clang 22の`clang-format` / `clang-tidy`を導入する。LLVM公式APTリポジトリの設定も自動で行う。同じスクリプトをGitHub Actionsでも使用する。

環境だけを再確認したい場合は次を実行する。

```bash
bash scripts/check-env.sh
```

`check-env.sh`は必要なツールとバージョンを表示し、C++23の`std::expected`が実際にコンパイルできることも確認する。

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
