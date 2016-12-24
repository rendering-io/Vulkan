##
# ext/CMakeLists.txt
#

# Add ExternalProject support.
include(ExternalProject)

if(${BUILD_UNITTESTS})
set(GTEST_BUILD_DIR
    ${CMAKE_CURRENT_BINARY_DIR}/GoogleTest-prefix/src/GoogleTest-build) 
set(GTEST_LOCATION ${GTEST_BUILD_DIR}/googlemock/gtest/libgtest.a)
set(GMOCK_LOCATION ${GTEST_BUILD_DIR}/googlemock/libgmock.a)

# External project to fetch and build GoogleTest/GoogleMock
ExternalProject_Add(GoogleTest
                    GIT_REPOSITORY https://github.com/google/googletest.git
                    INSTALL_COMMAND ""
                    BUILD_BYPRODUCTS ${GTEST_LOCATION} ${GMOCK_LOCATION}
                                     ${GTEST_BUILD_DIR}/googlemock/include 
                    )

# Import the library files from our GoogleTest external project.
ExternalProject_Get_Property(GoogleTest source_dir)
add_library(GTestLibrary STATIC IMPORTED)
add_library(GMockLibrary STATIC IMPORTED)
add_dependencies(GTestLibrary GoogleTest)
add_dependencies(GMockLibrary GoogleTest)

set_target_properties(GTestLibrary PROPERTIES 
                      IMPORTED_LOCATION ${GTEST_LOCATION}
                      )
set_target_properties(GMockLibrary PROPERTIES
                      IMPORTED_LOCATION ${GMOCK_LOCATION}
                      )

# Setting INTERFACE_INCLUDE_DIRECTORIES on IMPORTED targets fails with due to
# non-existent directories when we use an external project, so we add interface 
# libraries which include the imported libraries on their linking lists.
add_library(GTest INTERFACE)
add_library(GMock INTERFACE)
set_target_properties(GTest PROPERTIES
                      INTERFACE_INCLUDE_DIRECTORIES ${source_dir}/googletest/include
                      INTERFACE_LINK_LIBRARIES "GTestLibrary;pthread"
                      )
set_target_properties(GMock PROPERTIES
                      INTERFACE_INCLUDE_DIRECTORIES ${source_dir}/googlemock/include
                      INTERFACE_LINK_LIBRARIES "GMockLibrary;GTest"
		      )
endif()

