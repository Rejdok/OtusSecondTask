cmake_minimum_required(VERSION 3.2)

project(ip_filter VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})

find_package(Boost COMPONENTS unit_test_framework REQUIRED)

add_executable(ip_filter main.cpp)

add_executable(test_ip test.cpp)

set_target_properties(
	ip_filter test_ip PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED ON
	COMPILE_OPTIONS -Wpedantic -Wall -Wextra
)

target_compile_definitions(
	test_ip
	PRIVATE "BOOST_TEST_DYN_LINK=1"
)

install(TARGETS ip_filter RUNTIME DESTINATION bin)

set(CPACK_GENERATOR "DEB")

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Rejdok")
set(CPAK_PACKAGE_CONTACT "lvbnu@yandex.ru")
include(CPack)


enable_testing()
add_test(test_version test_version)
