
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
  git clone https://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@github.com/ilhamfzri/WidyaWakeWordCPPModel.git
  cd WidyaWakeWordCPPModel
  ```
 2. Edit CMakeList.txt
   ```sh
  target_link_libraries("demo" ${REQUIRED_LIBS} 
                    /home/hamz/WidyaWakeWordCPPModel/portaudio/install/lib/libportaudio.a //set this to your portaudio directory
                    -lpthread
                    -lasound
                    -lblas 
                    )
  ```
 4. Build for x64 Linux
  ```sh
  sudo cp prebuilt/x86/libtensorflowlite.so lib/
  mkdir build
  cd build
  cmake ..
  make
  ```
  
  5. Build for armv7 Linux
  ```sh
  sudo cp prebuilt/armv7/libtensorflowlite.so lib/
  mkdir build
  cd build
  cmake ..
  make
  ```
