add_library( test_base INTERFACE )
target_include_directories( test_base
    INTERFACE
    
    )
target_link_libraries( test_base
    INTERFACE
    doctest::doctest
    puma_static
    )

# add tests
TEST_TARGET(test_Puma test_puma.cc test_base)
