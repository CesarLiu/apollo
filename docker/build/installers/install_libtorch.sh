#!/usr/bin/env bash

###############################################################################
# Copyright 2020 The Apollo Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###############################################################################

set -e

CURR_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
. ${CURR_DIR}/installer_base.sh

# TODO(build): Docs on how to build libtorch on Jetson boards
# References:
#   https://github.com/ApolloAuto/apollo/blob/pre6/docker/build/installers/install_libtorch.sh
#   https://github.com/dusty-nv/jetson-containers/blob/master/Dockerfile.pytorch
#   https://github.com/pytorch/pytorch#nvidia-jetson-platforms
#
# Following content describes how to build libtorch source on jetson tx2:
# 1. Downloading libtorch source: git clone --recursive --branch v1.11.0 http://github.com/pytorch/pytorch
# 2. install py deps: pip3 install --no-cache-dir PyYAML typing
# 3. set env : export TORCH_CUDA_ARCH_LIST="3.5;5.0;5.2;6.1;6.2" && export USE_QNNPACK=0 && export USE_PYTORCH_QNNPACK=0 && export PYTORCH_BUILD_NUMBER=1 && export BUILD_CAFFE2=1 && export USE_NCCL=0 && export PYTORCH_BUILD_VERSION=1.11.0  # without the leading 'v', e.g. 1.3.0 for PyTorch v1.3.0
# 4. set env for cpu support: export USE_CUDA=0
# (4.1 set env for cpu support: export USE_CUDA=1)
# 5. python3 setup.py install
# (5.1 python3 setup.py install)
# 6. mkdir libtorch_cpu && cp -r include libtorch_cpu/ && cp -r lib libtorch_cpu/ && sudo mv libtorch_cpu /usr/local/
# (6.1 mkdir libtorch_gpu && cp -r include libtorch_gpu/ && cp -r lib libtorch_gpu/ && sudo mv libtorch_gpu /usr/local/)
# Attention check which python3 version you seee, here I use python3.7

bash ${CURR_DIR}/install_mkl.sh

TARGET_ARCH="$(uname -m)"

sudo apt update && sudo apt install -y libopenblas-base libopenmpi-dev libomp-dev

pip3 install Cython

##============================================================##
pushd pytorch
  export USE_QNNPACK=0 
  export USE_PYTORCH_QNNPACK=0 
  export PYTORCH_BUILD_NUMBER=1 
  export BUILD_CAFFE2=1 
  export USE_NCCL=0 
  export PYTORCH_BUILD_VERSION=1.11.0
  # libtorch_cpu
  export USE_CUDA=0
  python3 setup.py install
  mkdir -p /usr/local/libtorch_cpu
  cp -r build/lib.linux-x86_64-3.7/torch/lib /usr/local/libtorch_cpu/
  cp -r build/lib.linux-x86_64-3.7/torch/include /usr/local/libtorch_cpu/
  ok "Successfully installed libtorch_cpu ${VERSION}"

  python3 setup.py clean
  # libtorch_gpu
  export USE_CUDA=1
  export TORCH_CUDA_ARCH_LIST="3.5;5.0;5.2;6.1;6.2"

  python3 setup.py install
  mkdir -p /usr/local/libtorch_gpu
  cp -r build/lib.linux-x86_64-3.7/torch/lib /usr/local/libtorch_gpu/
  cp -r build/lib.linux-x86_64-3.7/torch/include /usr/local/libtorch_gpu/
  ok "Successfully installed libtorch_gpu ${VERSION}"

popd
rm -fr pytorch

# # libtorch_cpu

# if [[ "${TARGET_ARCH}" == "x86_64" ]]; then
#     # https://download.pytorch.org/libtorch/cpu/libtorch-cxx11-abi-shared-with-deps-1.5.0%2Bcpu.zip
#     VERSION="1.7.0-2"
#     CHECKSUM="02fd4f30e97ce8911ef933d0516660892392e95e6768b50f591f4727f6224390"
#     PKG_NAME="libtorch_cpu-${VERSION}-linux-${TARGET_ARCH}.tar.gz"
#     DOWNLOAD_LINK="https://apollo-system.cdn.bcebos.com/archive/6.0/${PKG_NAME}"
# elif [[ "${TARGET_ARCH}" == "aarch64" ]]; then
#     VERSION="1.11.0"
#     CHECKSUM="7faae6caad3c7175070263a0767732c0be9a92be25f9fb022aebe14a4cd2d092"
#     PKG_NAME="libtorch_cpu-${VERSION}-linux-${TARGET_ARCH}.tar.gz"
#     DOWNLOAD_LINK="https://apollo-pkg-beta.cdn.bcebos.com/archive/${PKG_NAME}"
# else
#     error "libtorch for ${TARGET_ARCH} not ready. Exiting..."
#     exit 1
# fi


# download_if_not_cached "${PKG_NAME}" "${CHECKSUM}" "${DOWNLOAD_LINK}"

# tar xzf "${PKG_NAME}"
# mv libtorch_cpu /usr/local/libtorch_cpu
# rm -f "${PKG_NAME}"
# ok "Successfully installed libtorch_cpu ${VERSION}"

# ##============================================================##
# # libtorch_gpu
# if [[ "${TARGET_ARCH}" == "x86_64" ]]; then
#     VERSION="1.7.0-2"
#     CHECKSUM="b64977ca4a13ab41599bac8a846e8782c67ded8d562fdf437f0e606cd5a3b588"
#     PKG_NAME="libtorch_gpu-${VERSION}-cu111-linux-x86_64.tar.gz"
#     DOWNLOAD_LINK="https://apollo-system.cdn.bcebos.com/archive/6.0/${PKG_NAME}"
# else # AArch64
#     VERSION="1.11.0"
#     PKG_NAME="libtorch_gpu-${VERSION}-linux-${TARGET_ARCH}.tar.gz"
#     CHECKSUM="661346303cafc832ef2d37b734ee718c85cdcf5ab21b72b13ef453cb40f13f86"
#     DOWNLOAD_LINK="https://apollo-pkg-beta.cdn.bcebos.com/archive/${PKG_NAME}"
# fi


# download_if_not_cached "${PKG_NAME}" "${CHECKSUM}" "${DOWNLOAD_LINK}"

# tar xzf "${PKG_NAME}"
# mv libtorch_gpu /usr/local/libtorch_gpu

# # Cleanup
# rm -f "${PKG_NAME}"
# ok "Successfully installed libtorch_gpu ${VERSION}"
