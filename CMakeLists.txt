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

cmake_minimum_required(VERSION 3.25)
project(simple_reverb VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.13")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(formats "VST3" "Standalone")

if(APPLE)
    list(APPEND formats "AU")
endif()

add_subdirectory(lib/JUCE)

juce_add_plugin(simple_reverb
        COMPANY_NAME "Kengo"
        COMPANY_WEBSITE "https://kengo.dev"
        COMPANY_EMAIL "hi@kengo.dev"
        BUNDLE_ID "dev.kengo.${PROJECT_NAME}"
        PLUGIN_MANUFACTURER_CODE "Kngo"
        PLUGIN_CODE "Srvb"
        FORMATS ${formats}
        COPY_PLUGIN_AFTER_BUILD TRUE
        PLUGIN_NAME "Simple Reverb"
)

target_compile_features(simple_reverb PUBLIC cxx_std_20)

target_compile_definitions(simple_reverb PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)

target_sources(
    simple_reverb
    PRIVATE
        src/PluginEditor.cpp
        src/PluginProcessor.cpp
        src/ui/EditorContent.cpp
        src/ui/Dial.cpp
        src/ui/FreezeButton.cpp
        src/ui/EditorLnf.cpp
)

juce_add_binary_data(binary_data SOURCES
        res/FreezeIcon.svg
        res/UbuntuRegular.ttf
)

target_link_libraries(
    simple_reverb
    PRIVATE
        binary_data
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)
