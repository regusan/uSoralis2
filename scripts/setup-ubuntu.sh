#!/usr/bin/env bash
set -euo pipefail

readonly LLVM_VERSION=22
readonly LLVM_KEY_URL="https://apt.llvm.org/llvm-snapshot.gpg.key"
readonly LLVM_REPOSITORY="deb https://apt.llvm.org/noble/ llvm-toolchain-noble-${LLVM_VERSION} main"
readonly LLVM_KEY_PATH="/etc/apt/trusted.gpg.d/apt.llvm.org.asc"
readonly LLVM_LIST_PATH="/etc/apt/sources.list.d/llvm${LLVM_VERSION}.list"
readonly SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
readonly REPOSITORY_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

if [[ ! -r /etc/os-release ]]; then
  echo "エラー: /etc/os-releaseを読み込めません。Ubuntu 24.04で実行してください。" >&2
  exit 1
fi

# Ubuntu 24.04 / WSL2 Ubuntu 24.04を基準環境とする。
source /etc/os-release
if [[ "${ID:-}" != "ubuntu" || "${VERSION_CODENAME:-}" != "noble" ]]; then
  echo "エラー: このスクリプトはUbuntu 24.04 (noble)専用です。" >&2
  exit 1
fi

sudo apt-get update
sudo apt-get install --no-install-recommends -y \
  build-essential \
  ca-certificates \
  cmake \
  ninja-build \
  wget

if [[ ! -f "${LLVM_KEY_PATH}" ]]; then
  wget -qO- "${LLVM_KEY_URL}" \
    | sudo tee "${LLVM_KEY_PATH}" >/dev/null
fi

if [[ ! -f "${LLVM_LIST_PATH}" ]] || \
   ! grep -Fxq "${LLVM_REPOSITORY}" "${LLVM_LIST_PATH}"; then
  echo "${LLVM_REPOSITORY}" \
    | sudo tee "${LLVM_LIST_PATH}" >/dev/null
fi

sudo apt-get update
sudo apt-get install --no-install-recommends -y \
  "clangd-${LLVM_VERSION}" \
  "clang-format-${LLVM_VERSION}" \
  "clang-tidy-${LLVM_VERSION}"

git -C "${REPOSITORY_DIR}" submodule update --init --recursive

bash "${SCRIPT_DIR}/check-env.sh"
