
find_package(PythonLibs 3 REQUIRED)

set (LANGBIND "Python")
set (LANGBIND_GROUP "Python Bindings")

set_property(GLOBAL PROPERTY CXX_STANDARD 11)

set (LANGBIND_FILES
  bindings/python/Flpy.cxx
  bindings/python/Flpy_Fl.cxx
  bindings/python/Flpy_Widget.cxx
  bindings/python/Flpy_Group.cxx
  bindings/python/Flpy_Window.cxx
  bindings/python/Flpy_Button.cxx
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

