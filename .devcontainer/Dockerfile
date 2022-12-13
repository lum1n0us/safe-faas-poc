ARG VARIANT=focal
FROM mcr.microsoft.com/devcontainers/base:${VARIANT}

# [Optional] Uncomment this section to install additional OS packages.
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
  && apt-get -y install --no-install-recommends build-essential libssl-dev

# Install sgx
RUN wget -qO - https://download.01.org/intel-sgx/sgx_repo/ubuntu/intel-sgx-deb.key | sudo apt-key add
RUN echo 'deb [arch=amd64] https://download.01.org/intel-sgx/sgx_repo/ubuntu focal main' | sudo tee /etc/apt/sources.list.d/intel-sgx.list
RUN apt-get update && apt-get install -y libsgx-epid libsgx-quote-ex libsgx-dcap-ql

WORKDIR /tmp
RUN wget https://download.01.org/intel-sgx/latest/linux-latest/distro/ubuntu20.04-server/sgx_linux_x64_sdk_2.17.100.3.bin
RUN chmod +x sgx_linux_x64_sdk_2.17.100.3.bin && ./sgx_linux_x64_sdk_2.17.100.3.bin --prefix /opt/intel/sgx

# Install go
WORKDIR /tmp
RUN wget https://go.dev/dl/go1.19.4.linux-amd64.tar.gz
RUN tar -C /usr/local -xzf go1.19.4.linux-amd64.tar.gz
RUN mkdir /go && chmod a+wx /go
ENV GOPATH=/go
ENV PATH=${PATH}:/usr/local/go/bin:${GOPATH}/bin

# Install ego
WORKDIR /tmp
RUN wget https://github.com/edgelesssys/ego/releases/download/v1.0.1/ego_1.0.1_amd64.deb
RUN apt install -y -f ./ego_1.0.1_amd64.deb

# Install tinygo
WORKDIR /tmp
RUN wget https://github.com/tinygo-org/tinygo/releases/download/v0.26.0/tinygo_0.26.0_amd64.deb
RUN dpkg -i tinygo_0.26.0_amd64.deb

# Install cmake
RUN wget --progress=dot:giga -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /usr/share/keyrings/kitware-archive-keyring.gpg > /dev/null \
  && echo 'deb [signed-by=/usr/share/keyrings/kitware-archive-keyring.gpg] https://apt.kitware.com/ubuntu/ bionic main' | tee /etc/apt/sources.list.d/kitware.list >/dev/null \
  && apt-get update \
  && rm /usr/share/keyrings/kitware-archive-keyring.gpg \
  && apt-get install -y kitware-archive-keyring --no-install-recommends \
  && apt-get install -y cmake --no-install-recommends

# Install clang and llvm
WORKDIR /etc/apt/apt.conf.d
RUN touch 99verfiy-peer.conf \
  && echo "Acquire { https::Verify-Peer false }" > 99verfiy-peer.conf
WORKDIR /tmp
RUN apt-get update \
  && apt-get install -y apt-transport-https apt-utils build-essential lsb-release wget software-properties-common gnupg
RUN wget https://apt.llvm.org/llvm.sh && chmod a+x ./llvm.sh
RUN ./llvm.sh 14 all

# Install wabt
#install wabt
ARG WABT_VER=1.0.31
RUN wget -c --progress=dot:giga https://github.com/WebAssembly/wabt/releases/download/${WABT_VER}/wabt-${WABT_VER}-ubuntu.tar.gz -P /opt \
  && tar xf /opt/wabt-${WABT_VER}-ubuntu.tar.gz -C /opt \
  && rm /opt/wabt-${WABT_VER}-ubuntu.tar.gz

# Install wasi-sdk
ARG WASI_SDK_VER=16
RUN wget -c --progress=dot:giga https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-${WASI_SDK_VER}/wasi-sdk-${WASI_SDK_VER}.0-linux.tar.gz -P /opt \
  && tar xf /opt/wasi-sdk-${WASI_SDK_VER}.0-linux.tar.gz -C /opt \
  && rm /opt/wasi-sdk-${WASI_SDK_VER}.0-linux.tar.gz

# set path and use clang by default
ENV PATH="/usr/lib/llvm-14/bin:/opt/wabt-${WABT_VER}/bin:${PATH}"
ENV CC=/usr/lib/llvm-14/bin/clang CXX=/usr/lib/llvm-14/bin/clang++
ENV SGX_SDK=/opt/intel/sgx/sgxsdk PATH=$PATH:$SGX_SDK/bin:$SGX_SDK/bin/x64 \
  PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$SGX_SDK/pkgconfig \
  LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SGX_SDK/sdk_libs

# clean apt cached
RUN apt-get clean -y && rm -rf /var/lib/apt/lists/*
