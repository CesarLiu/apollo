FROM nvcr.io/nvidia/l4t-jetpack-apollo:r32.7.1 
ENV CUDA_LITE 10.2

ENV CUDA_VERSION 10.2.3

#RUN wget https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/sbsa/cuda-ubuntu2004.pin && \
#    sudo mv cuda-ubuntu2004.pin /etc/apt/preferences.d/cuda-repository-pin-600 && \
#    sudo apt-key adv --fetch-keys https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/sbsa/7fa2af80.pub \
#    && sudo add-apt-repository "deb https://developer.download.nvidia.com/compute/cuda/repos/ubuntu2004/sbsa/ /" && \
#    sudo apt-get update


ENV PATH /usr/local/cuda/bin:${PATH}

# nvidia-container-runtime
ENV NVIDIA_VISIBLE_DEVICES all
ENV NVIDIA_DRIVER_CAPABILITIES compute,utility
ENV NVIDIA_REQUIRE_CUDA "cuda>=${CUDA_LITE}"


ENV LIBRARY_PATH /usr/local/cuda/lib64/stubs

ENV CUDNN_VERSION 8.2.1

ENV TENSORRT_VERSION 8.2.1
 
COPY rcfiles /opt/apollo/rcfiles
COPY installers /opt/apollo/installers

# RUN apt update && DEBIAN_FRONTEND=noninteractive TZ="Europe/Germany" apt-get -y install tzdata

# RUN bash /opt/apollo/installers/install_minimal_environment.sh us 18.04
# RUN bash /opt/apollo/installers/install_bazel.sh
# RUN bash /opt/apollo/installers/install_cmake.sh build

# RUN bash /opt/apollo/installers/install_llvm_clang.sh
# RUN bash /opt/apollo/installers/install_cyber_deps.sh build
# RUN bash /opt/apollo/installers/install_qa_tools.sh
# RUN bash /opt/apollo/installers/install_visualizer_deps.sh build 18.04

# RUN bash /opt/apollo/installers/install_geo_adjustment.sh us

# RUN bash /opt/apollo/installers/install_modules_base.sh
# RUN bash /opt/apollo/installers/install_ordinary_modules.sh build
# RUN bash /opt/apollo/installers/install_drivers_deps.sh build
# RUN bash /opt/apollo/installers/install_dreamview_deps.sh us
# RUN bash /opt/apollo/installers/install_contrib_deps.sh build
# RUN bash /opt/apollo/installers/install_gpu_support.sh
# RUN bash /opt/apollo/installers/install_release_deps.sh
# RUN bash /opt/apollo/installers/install_tkinter.sh

# RUN bash /opt/apollo/installers/install_geo_adjustment.sh us
RUN rm -rf /opt/apollo/pcl-pcl-1.10.1/
RUN rm /opt/apollo/pcl-1.10.1.tar.gz
RUN bash /opt/apollo/installers/post_install.sh dev

RUN mkdir -p /opt/apollo/neo/data/log && chmod -R 777 /opt/apollo/neo

COPY rcfiles/setup.sh /opt/apollo/neo/   

RUN echo "source /opt/apollo/neo/setup.sh" >> /etc/skel/.bashrc

# RUN apt update && apt-get -y install nvidia-vpi
