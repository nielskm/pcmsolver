if(CMAKE_Fortran_COMPILER_ID MATCHES GNU)
  include(${CMAKE_CURRENT_LIST_DIR}/GNU/Fortran.cmake)
elseif(CMAKE_Fortran_COMPILER_ID MATCHES Intel)
  include(${CMAKE_CURRENT_LIST_DIR}/Intel/Fortran.cmake)
else()
  message(WARNING "Fortran compiler ${CMAKE_Fortran_COMPILER} never tested")
endif()
