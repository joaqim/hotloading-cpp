file(GLOB PLUGINS *.cpp)

foreach(file ${PLUGINS})
	get_filename_component(NAME_NOEXT ${file} NAME_WE)
	add_library(${PROJECT_NAME_LOWER}_${NAME_NOEXT}_plugin MODULE ${file})
endforeach()

#set(lib${PROJECT_NAME_LOWER}_plugin_sources plugin.cpp)
#add_library(${PROJECT_NAME_LOWER}_plugin MODULE ${lib${PROJECT_NAME_LOWER}_plugin_sources})
