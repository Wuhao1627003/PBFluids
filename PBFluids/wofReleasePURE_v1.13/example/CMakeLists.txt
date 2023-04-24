cmake_minimum_required(VERSION 3.0)
project(example)

# * 1 *   Header directory "../include"
include_directories(${CMAKE_SOURCE_DIR}/../include/)

set(SEC "PURE")

# * 2 *   Add the executable for WIN and Linux
if(MSVC)
	### WINDOWS ###
	
	# Parallel compilation
	add_compile_options(/MP)
	# TOOLNAME v140, v141, v142 (VS2015, VS2017, VS2019)
	set(TOOLNAME "v${MSVC_TOOLSET_VERSION}")

	# Link directories
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../binaries/x64)
	set(TA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../binaries/lib_licensing_dynamic/windows/bin-windows/x64/)
	link_directories(${TA_DIR})
	
	# WOF library version
	set(WOFLIBNAME 
		debug "wof${SEC}_x64_${TOOLNAME}_Debug"
		optimized "wof${SEC}_x64_${TOOLNAME}_Release"
		)
	add_executable(wof_example main.cpp reconstruct.cpp remesh.cpp toCloud.cpp)
	set(OUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/)
	set_target_properties(wof_example PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${OUTDIR} )
	TARGET_LINK_LIBRARIES( 	wof_example ${WOFLIBNAME}  )							
	
	add_custom_command(	TARGET wof_example POST_BUILD  
				COMMAND echo CCConfiguration is $<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>
				COMMAND ${CMAKE_COMMAND} -E copy_if_different 
				"${CMAKE_CURRENT_SOURCE_DIR}/../binaries/x64/wof${SEC}_x64_${TOOLNAME}_$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>.dll" 
				${OUTDIR}/$<$<CONFIG:Release>:Release>$<$<CONFIG:Debug>:Debug>
				
				) 

	
else()
	### LINUX ###
	link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../binaries/lib_ubuntu22.04_x86_64)
	#link_directories(${CMAKE_CURRENT_SOURCE_DIR}/../binaries/lib_licensing_dynamic/linux/bin-linux/x64/)
	add_executable(wof_example main.cpp reconstruct.cpp remesh.cpp toCloud.cpp)
	#TARGET_LINK_LIBRARIES( 	wof_example wof${SEC} pthread TurboActivate )
	TARGET_LINK_LIBRARIES( 	wof_example wof${SEC} pthread  )
endif()

