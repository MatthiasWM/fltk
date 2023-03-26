
find_package(PythonLibs 3 REQUIRED)

set (LANGBIND "Python")
set (LANGBIND_GROUP "Python Bindings")

set (LANGBIND_FILES
  bindings/python/Flpy.cxx
)

set (LANGBIND_HEADER_FILES
  bindings/python/Flpy.h
)

set (LANGBIND_INCLUDE_DIRS
  ${PYTHON_INCLUDE_DIRS}
)

set (LANGBIND_LIBRARIES
  ${PYTHON_LIBRARIES}
)

