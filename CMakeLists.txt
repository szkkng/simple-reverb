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
    AU_MAIN_TYPE "kAudioUnitType_Effect")

target_compile_features(SimpleReverb PUBLIC cxx_std_17)

target_compile_definitions(SimpleReverb PUBLIC
    JUCE_WEB_BROWSER=0  
    JUCE_VST3_CAN_REPLACE_VST2=0)

add_subdirectory(Resources)
add_subdirectory(Source)

target_link_libraries(SimpleReverb PUBLIC 
    juce::juce_audio_basics
    juce::juce_audio_devices
    juce::juce_audio_formats
    juce::juce_audio_plugin_client
    juce::juce_audio_processors
    juce::juce_audio_utils
    juce::juce_core
    juce::juce_data_structures
    juce::juce_dsp
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra)

juce_generate_juce_header(SimpleReverb)