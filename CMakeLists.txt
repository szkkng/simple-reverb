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

cmake_minimum_required(VERSION 3.15)
project(SimpleReverb VERSION 1.0.0)
add_subdirectory(External/JUCE)

set(formats "VST3" "Standalone")
if (APPLE)
    list(APPEND formats "AU")
endif()

juce_add_plugin(SimpleReverb
    VERSION "1.0.0"
    COMPANY_NAME "Suzuki Kengo"
    COMPANY_WEBSITE "https://suzuki-kengo.dev"
    COMPANY_EMAIL "zukkesukke23@gmail.com"
    PLUGIN_MANUFACTURER_CODE "Szkn"
    PLUGIN_CODE "Srvb"
    FORMATS ${formats}
    VST3_CATEGORIES "Fx"
    AU_MAIN_TYPE "kAudioUnitType_Effect"
    PLUGIN_NAME "Simple Reverb"
    PRODUCT_NAME "SimpleReverb")

add_subdirectory(Source)

juce_generate_juce_header(SimpleReverb)

target_compile_features(SimpleReverb PUBLIC cxx_std_17)

target_compile_definitions(SimpleReverb
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(SimpleReverb
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)
