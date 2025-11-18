add_library( app_base INTERFACE )
target_include_directories( app_base
    INTERFACE
    
    )
target_link_libraries( app_base
    INTERFACE
    puma_static
    )

# add apps
APP_TARGET(app_log app_log.cc app_base)
