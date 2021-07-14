cmake_minimum_required(VERSION 3.15)

PROJECT(SimpleReverb
LANGUAGES CXX
VERSION 0.0.1
)

add_subdirectory(External/JUCE)

add_subdirectory(Source)