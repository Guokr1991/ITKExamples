get_filename_component(_ITKExamplesExternalData_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(${_ITKExamplesExternalData_DIR}/ExternalData.cmake)

if(NOT ExternalData_OBJECT_STORES)
  # Use ExternalData_OBJECT_STORES from environment as default.
  set(ExternalData_OBJECT_STORES_DEFAULT "")
  if(DEFINED "ENV{ExternalData_OBJECT_STORES}")
    file(TO_CMAKE_PATH "$ENV{ExternalData_OBJECT_STORES}" ExternalData_OBJECT_STORES_DEFAULT)
  endif()
endif()

set(ExternalData_OBJECT_STORES "${ExternalData_OBJECT_STORES_DEFAULT}" CACHE STRING
  "Semicolon-separated list of local directories holding data objects in the layout %(algo)/%(hash).")
mark_as_advanced(ExternalData_OBJECT_STORES)
if(NOT ExternalData_OBJECT_STORES)
  set(ExternalData_OBJECT_STORES "${CMAKE_BINARY_DIR}/ExternalData/Objects")
  file(MAKE_DIRECTORY "${ExternalData_OBJECT_STORES}")
endif()
list(APPEND ExternalData_OBJECT_STORES
  # Local data store populated by the ITK pre-commit hook
  "${CMAKE_SOURCE_DIR}/.ExternalData"
  )

set(ExternalData_URL_TEMPLATES "" CACHE STRING
  "Additional URL templates for the ExternalData CMake script to look for testing data. E.g.
file:///var/bigharddrive/%(algo)/%(hash)")
mark_as_advanced(ExternalData_URL_TEMPLATES)
list(APPEND ExternalData_URL_TEMPLATES
  # Data published on itk.org
  "https://www.itk.org/files/ExamplesData/%(algo)/%(hash)"
  "https://www.itk.org/files/ExternalData/%(algo)/%(hash)"

  # Data published on the Rackspace backup
  "http://c389985.r85.cf1.rackcdn.com/%(algo)/%(hash)"

  # Data published by MIDAS
  "http://midas3.kitware.com/midas/api/rest?method=midas.bitstream.download&checksum=%(hash)"
  )

# Tell ExternalData commands to transform raw files to content links.
# TODO: Condition this feature on presence of our pre-commit hook.
set(ExternalData_LINK_CONTENT MD5)
