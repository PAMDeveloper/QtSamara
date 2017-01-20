ADD_SUBDIRECTORY(model)
ADD_SUBDIRECTORY(utils)

add_library(samara-lib SHARED lib.hpp lib.cpp)

set_target_properties(samara-lib PROPERTIES
  VERSION 0
  OUTPUT_NAME "samara-${SAMARA_VERSION_SHORT}"
  CLEAN_DIRECT_OUTPUT 1)

target_link_libraries(samara-lib samara-kernel samara-observer samara-utils
  samara-models-samara samara-models-meteo ${Boost_LIBRARIES})

install(TARGETS samara-lib RUNTIME DESTINATION bin LIBRARY DESTINATION
  lib ARCHIVE DESTINATION lib)
