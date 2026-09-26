# ビルド

現段階ではHost上のBMPサンプルのみを対象とする。PythonとPico SDKは不要。

## 対象環境

Ubuntu 24.04またはWSL2上のUbuntu 24.04を基準環境とする。

- C++23
- GCC / libstdc++で通常ビルド
- Clang 22の`clangd`、`clang-format`、`clang-tidy`を使用
- CMake + Ninja
- GLM 1.0.3をGit submoduleとして使用

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

このスクリプトはUbuntu 24.04を確認したうえで、GCC、CMake、Ninja、Clang 22の`clangd` / `clang-format` / `clang-tidy`を導入する。LLVM公式APTリポジトリの設定に加えて、`third_party/glm` submoduleの初期化も自動で行う。同じスクリプトをGitHub Actionsでも使用する。

環境だけを再確認したい場合は次を実行する。

```bash
bash scripts/check-env.sh
```

`check-env.sh`は必要なツールとバージョンを表示し、C++23の`std::expected`が実際にコンパイルできることも確認する。

## VS Code / clangd

VS Codeを使用する場合は、このディレクトリをWSL側から開く。推奨拡張機能は`.vscode/extensions.json`に定義しており、C++の補完と診断にはclangdを使用する。

リポジトリ直下の`.clangd`で`build/debug/compile_commands.json`を参照するため、最初にDebug構成を生成する。

```bash
cmake --preset debug
```

VS Code設定では`/usr/bin/clangd-22`を明示している。Microsoft C/C++拡張もインストールしている場合は、IntelliSenseを無効にしてclangdとの二重診断を避ける。

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
