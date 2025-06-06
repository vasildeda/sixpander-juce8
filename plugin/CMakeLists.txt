juce_add_plugin(Sixpander
    COMPANY_NAME Vasilovo
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    PLUGIN_MANUFACTURER_CODE VSLV
    PLUGIN_CODE SXPN
    FORMATS AU VST3 Standalone
    COPY_PLUGIN_AFTER_BUILD TRUE
)

target_sources(Sixpander
    PRIVATE src/Sixpander.cpp src/SixpanderEditor.cpp src/MeterComponent.cpp src/RotaryKnob.cpp
)

include_directories(Sixpander PRIVATE include)

# Copy assets directory to build directory
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(Sixpander
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

target_compile_definitions(Sixpander
    PUBLIC
        JUCE_VST3_CAN_REPLACE_VST2=0
)
