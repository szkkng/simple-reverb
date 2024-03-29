# ==============================================================================
#
#  Copyright 2021, 2022 Suzuki Kengo
# 
#  Simple Reverb is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
# 
#  Simple Reverb is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU General Public License for more details.
# 
#  You should have received a copy of the GNU General Public License
#  along with Simple Reverb. If not, see <http://www.gnu.org/licenses/>.
# 
# ==============================================================================

cmake_minimum_required(VERSION 3.21)

set(PROJECT_NAME "SimpleReverb")
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64" CACHE STRING "Build architectures for macOS")
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.11" CACHE STRING "Minimum macOS version required")

project("${PROJECT_NAME}" VERSION 1.0.0)

set(formats "VST3")

if(APPLE)
    list(APPEND formats "AU")
endif()

add_subdirectory(External/JUCE)

juce_add_plugin("${PROJECT_NAME}"
    VERSION "1.0.0"
    COMPANY_NAME "Suzuki Kengo"
    COMPANY_WEBSITE "https://suzuki-kengo.dev"
    COMPANY_EMAIL "zukkesukke23@gmail.com"
    PLUGIN_MANUFACTURER_CODE "Szkn"
    PLUGIN_CODE "Srvb"
    FORMATS ${formats}
    COPY_PLUGIN_AFTER_BUILD TRUE
    IS_SYNTH FALSE     
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    PRODUCT_NAME "${PROJECT_NAME}"
    PLUGIN_NAME "Simple Reverb")

juce_generate_juce_header("${PROJECT_NAME}")

add_subdirectory(Source)

target_compile_features("${PROJECT_NAME}" PUBLIC cxx_std_20)

target_compile_definitions("${PROJECT_NAME}"
    PUBLIC
        JUCE_DISPLAY_SPLASH_SCREEN=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_sources("${PROJECT_NAME}"
    PRIVATE
        Resources/FuturaMedium.cpp)

juce_add_binary_data(BinaryData
    SOURCES
        Resources/FreezeIcon.svg)

target_link_libraries("${PROJECT_NAME}"
    PRIVATE
        BinaryData
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
