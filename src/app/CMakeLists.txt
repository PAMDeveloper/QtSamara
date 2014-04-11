INCLUDE_DIRECTORIES(
  ${Samara_BINARY_DIR}/src
  ${Samara_SOURCE_DIR}/src
  ${Boost_INCLUDE_DIRS})

LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

ADD_EXECUTABLE(samara main.cpp)
SET_TARGET_PROPERTIES(samara PROPERTIES ${SAMARA_APP_PROPERTIES})

TARGET_LINK_LIBRARIES(samara samara-kernel samara-models-samara
  samara-models-meteo samara-utils ${Boost_LIBRARIES} pqxx)

INSTALL(TARGETS samara DESTINATION bin)
