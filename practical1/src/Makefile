all: 
	mkdir -p build ; cd build/ && cmake \
	-DGLM_INCLUDE_DIRS="./../extlib/glm-0.9.7.1" \
	-DGLEW_INCLUDE_DIRS="./../extlib/glew-1.13.0/include" \
	-DGLEW_LIBRARIES="./../extlib/glew-1.13.0/lib/libGLEW.so" \
	-DSFML_INCLUDE_DIRS="./../extlib/sfml-2.3.2/include/" \
	-DSFML_SYSTEM_LIBRARIES="./../extlib/sfml-2.3.2/build/lib/libsfml-system.so" \
	-DSFML_WINDOW_LIBRARIES="./../extlib/sfml-2.3.2/build/lib/libsfml-window.so" \
	-DSFML_GRAPHICS_LIBRARIES="./../extlib/sfml-2.3.2/build/lib/libsfml-graphics.so" \
	-DFREETYPE_INCLUDE_DIRS="./../extlib/freetype-2.6.2/include" \
	-DFREETYPE_LIBRARIES="./../extlib/freetype-2.6.2/build/libfreetype.so" \
	.. && make
