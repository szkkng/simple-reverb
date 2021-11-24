target_sources(SimpleReverb
    PRIVATE
        PluginEditor.cpp
        PluginProcessor.cpp
        LookAndFeel/CustomLookAndFeel.cpp
        Components/RotarySlider.cpp)

add_subdirectory(Components)
add_subdirectory(LookAndFeel)
