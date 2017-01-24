INCLUDE_DIRECTORIES(
  ${CMAKE_SOURCE_DIR}/src ${ARTIS_INCLUDE_DIRS})

LINK_DIRECTORIES(${ARTIS_LIBRARY_DIRS})

SET(SAMARA_KERNEL_HPP AbstractAtomicModel.hpp AbstractModel.hpp
  AbstractCoupledModel.hpp Simulator.hpp Model.hpp)

SET(SAMARA_KERNEL_CPP Simulator.cpp Model.cpp)

ADD_LIBRARY(samara-kernel SHARED ${SAMARA_KERNEL_HPP} ${SAMARA_KERNEL_CPP})
TARGET_LINK_LIBRARIES(samara-kernel samara-models-samara)

INSTALL(FILES ${SAMARA_KERNEL_HPP} DESTINATION
  ${SAMARA_INCLUDE_DIRS}/model/kernel)

INSTALL(TARGETS samara-kernel DESTINATION lib)
