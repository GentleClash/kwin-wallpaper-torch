kcoreaddons_add_plugin(wallpapertorch INSTALL_NAMESPACE "kwin/effects/plugins/")
target_sources(wallpapertorch PRIVATE
    wallpapertorch.cpp
    main.cpp
)

set_property(TARGET wallpapertorch APPEND PROPERTY
    AUTOMOC_MACRO_NAMES "KWIN_EFFECT_FACTORY_SUPPORTED"
)

target_link_libraries(wallpapertorch
    KF6::CoreAddons
    KF6::I18n
    KWinX11::kwin
)

install(FILES $<TARGET_FILE:wallpapertorch>
    DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/qt6/plugins/kwin-x11/effects/plugins/)
