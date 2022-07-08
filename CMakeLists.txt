cmake_minimum_required (VERSION 3.11)

# --- Fetch FTXUI --------------------------------------------------------------
include(FetchContent)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/codereport/ftxui
  # GIT_TAG v3.0.0
)

FetchContent_GetProperties(ftxui)
if(NOT ftxui_POPULATED)
  FetchContent_Populate(ftxui)
  add_subdirectory(${ftxui_SOURCE_DIR} ${ftxui_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# --- Fetch utfcpp --------------------------------------------------------------

FetchContent_Declare(utfcpp
  GIT_REPOSITORY https://github.com/nemtrif/utfcpp
  GIT_TAG v3.2.1
)

FetchContent_GetProperties(utfcpp)
if(NOT utfcpp_POPULATED)
  FetchContent_Populate(utfcpp)
  add_subdirectory(${utfcpp_SOURCE_DIR} ${utfcpp_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# --- Fetch range-v3 ------------------------------------------------------------

FetchContent_Declare(range-v3
  GIT_REPOSITORY https://github.com/ericniebler/range-v3
  # GIT_TAG v0.12.0
)

FetchContent_GetProperties(range-v3)
if(NOT range-v3_POPULATED)
  FetchContent_Populate(range-v3)
  add_subdirectory(${range-v3_SOURCE_DIR} ${range-v3_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_MakeAvailable(range-v3)

# --- Fetch blackbird --------------------------------------------------------------

FetchContent_Declare(blackbird
  GIT_REPOSITORY https://github.com/codereport/blackbird
  GIT_TAG main
)

FetchContent_GetProperties(blackbird)
if(NOT blackbird_POPULATED)
  FetchContent_Populate(blackbird)
  add_subdirectory(${blackbird_SOURCE_DIR} ${blackbird_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# --- Fetch tl::expected -----------------------------------------------------------

FetchContent_Declare(expected
  GIT_REPOSITORY https://github.com/TartanLlama/expected
  GIT_TAG master
)

FetchContent_GetProperties(expected)
if(NOT expected_POPULATED)
  FetchContent_Populate(expected)
  add_subdirectory(${expected_SOURCE_DIR} ${expected_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

# ------------------------------------------------------------------------------

project(mex
  LANGUAGES CXX CUDA
  VERSION 1.0.0
)

if (NOT CMAKE_CUDA_COMPILER)
    set(CMAKE_CUDA_COMPILER "/usr/bin/nvcc")
endif()

add_executable(mex src/main.cpp)
set_property(TARGET mex PROPERTY CXX_STANDARD 20)
target_include_directories(mex PRIVATE src)
include_directories(${utfcpp_SOURCE_DIR}/source)
include_directories(${blackbird_SOURCE_DIR})
include_directories(${expected_SOURCE_DIR})

target_link_libraries(mex
  PRIVATE range-v3
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component # Not needed for this example.
)

if (EMSCRIPTEN) 
  string(APPEND CMAKE_CXX_FLAGS " -s USE_PTHREADS") 
  string(APPEND CMAKE_EXE_LINKER_FLAGS " -s ASYNCIFY") 
  string(APPEND CMAKE_EXE_LINKER_FLAGS " -s PROXY_TO_PTHREAD") 

  foreach(file "index.html" "run_webassembly.py")
    configure_file("src/${file}" ${file})
  endforeach(file)
endif() 
