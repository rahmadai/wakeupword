
<!-- PROJECT LOGO -->
<br />
<p align="center">
  <a href="https://github.com/ilhamfzri/widya-wicara-diarization">
    <img src="images/logo.png" alt="Logo" width="80" height="80">
  </a>

  <h3 align="center">Widya WakeWord CPP</h3>

  <p align="center">
<!--     Audio to Text Automation Transciption -->
    <br />
<!--     <a href="https://github.com/ilhamfzri/widya-wicara-diarization"><strong>Explore the docs »</strong></a> -->
    <br />
    <br />
<!--     <a href="https://colab.research.google.com/drive/13NL3jdqpWEKPUdREq6OGKb91MCQfcQfy?usp=sharing">View Demo</a> -->
    <!-- ·
    <a href="https://github.com/ilhamfzri/widya-wicara-diarization">Report Bug</a>
    ·
    <a href="https://github.com/ilhamfzri/widya-wicara-diarization">Request Feature</a> -->
  </p>
</p>

<!-- GETTING STARTED -->
## Getting Started
### Prerequisites
If you already have CMake 3.17+, skip this step.
 1. Remove CMake Old Version <3.17 (follow this step if you have cmake old version)
  ```sh
  sudo apt remove --purge cmake
  hash -r
  ```
 3. Install CMake 3.17+ 
   ```sh
  sudo apt install build-essential libssl-dev
  wget https://github.com/Kitware/CMake/releases/download/v3.20.2/cmake-3.20.2.tar.gz
  tar -zxvf cmake-3.20.2.tar.gz
  cd cmake-3.20.2
  ./bootstrap
  make 
  sudo make install 
  ```
  
  ### Build 
 1. Clone repo
  ```sh
  git clone https://github.com/rahmadai/wakeupword.git
  cd wakeupword
  ```

 2. Download cross compile
 ```sh
 curl -LO https://publishing-ie-linaro-org.s3.amazonaws.com/releases/components/toolchain/binaries/6.4-2017.11/arm-linux-gnueabihf/gcc-linaro-6.4.1-2017.11-x86_64_arm-linux-gnueabihf.tar.xz?Signature=VfyvQEjKz4mWRhKcIS4Soq3KbfA%3D&Expires=1636427417&AWSAccessKeyId=AKIAIELXV2RYNAHFUP7A -o toolchain.tar.gz
 tar xzf toolchain.tar.gz -C ${HOME}/toolchains
 
 ```
 3. Build for armv7
  ```sh
  mkdir build
  cd build
  ARMCC_FLAGS="-march=armv7-a -mfpu=vfp -funsafe-math-optimizations"
  ARMCC_PREFIX=${HOME}/toolchains/gcc-linaro-6.4.1-2017.11-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-
  cmake -DCMAKE_C_COMPILER=${ARMCC_PREFIX}gcc \
  -DCMAKE_CXX_COMPILER=${ARMCC_PREFIX}g++ \
  -DCMAKE_C_FLAGS="${ARMCC_FLAGS}" \
  -DCMAKE_CXX_FLAGS="${ARMCC_FLAGS}" \
  -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON \
  -DCMAKE_SYSTEM_NAME=Linux \
  -DCMAKE_SYSTEM_PROCESSOR=armv7 \
  -DCMAKE_BUILD_TYPE=Debug \
  ../
  make
  ```
