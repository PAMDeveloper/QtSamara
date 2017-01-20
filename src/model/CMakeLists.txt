CONFIGURE_FILE(version.hpp.in ${Samara_BINARY_DIR}/src/samara/version.hpp)

ADD_SUBDIRECTORY(kernel)
ADD_SUBDIRECTORY(models)
ADD_SUBDIRECTORY(observer)
