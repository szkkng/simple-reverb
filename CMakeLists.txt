cmake_minimum_required(VERSION 3.15)
project(SimpleReverb VERSION 1.0.0)
add_subdirectory(External/JUCE)

juce_add_plugin(SimpleReverb
    VERSION "1.0.0"
    COMPANY_NAME "Suzuki Kengo"
    COMPANY_WEBSITE "https://suzuki-kengo.dev"
    COMPANY_EMAIL "zukkesukke23@gmail.com"
    PLUGIN_MANUFACTURER_CODE "Szkn"
    PLUGIN_CODE "Srvb"
    FORMATS "VST3" "AU" "Standalone"
    VST3_CATEGORIES "Fx"
    AU_MAIN_TYPE "kAudioUnitType_Effect"
    PLUGIN_NAME "Simple Reverb"
    PRODUCT_NAME "SimpleReverb")

add_subdirectory(Resources)
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
