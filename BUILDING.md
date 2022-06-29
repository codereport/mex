# Build instructions:
~~~bash
mkdir build
cd build
cmake ..
make -j
cd ../target
./ftxui-starter
~~~

## Webassembly build:
~~~bash
mkdir build_emscripten && cd build_emscripten
emcmake cmake ..
make -j
./run_webassembly.py
(visit localhost:8000)
~~~

## Linux snap build:
Upload your game to github and visit https://snapcraft.io/build.
