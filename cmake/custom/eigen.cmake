set(EIGEN3_ROOT ${PROJECT_SOURCE_DIR}/external/eigen3)
set(EIGEN3_INCLUDE_DIR ${EIGEN3_ROOT}/include/eigen3)
install(DIRECTORY ${PROJECT_SOURCE_DIR}/external/eigen3 DESTINATION ${PROJECT_BINARY_DIR}/external/include)
message(STATUS "Eigen 3.2.0 is located here: " ${PROJECT_BINARY_DIR}/external/include/eigen3)
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})

# FIXME
#if(ENABLE_EIGEN_MKL)
#   message(STATUS "ENABLE_EIGEN_MKL option requires Intel MKL 10.3")
#   message(STATUS "   Be sure you have read http://eigen.tuxfamily.org/dox/TopicUsingIntelMKL.html")
#   include(ConfigMath)
#   set(EIGEN_USE_MKL_ALL ON)
#endif()