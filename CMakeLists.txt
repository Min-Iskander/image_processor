cmake_minimum_required(VERSION 3.20)

project(filters)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(image_processor image_processor.cpp factory.cpp args.cpp image.cpp bmp.cpp filters/crop.cpp filters/pixel.cpp filters/grayscale.cpp filters/negative.cpp filters/matrix.cpp filters/edge_detection.cpp filters/sharpening.cpp filters/gaussian_blur.cpp filters/pinch.cpp)