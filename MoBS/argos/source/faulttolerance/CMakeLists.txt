###############################################
# define shared object files
###############################################

add_library(Gradient_loop_functions MODULE FaultToleranceLoopFunctions.h
		FaultToleranceLoopFunctions.cpp
		FaultToleranceQTUserFunctions.h
		FaultToleranceQTUserFunctions.cpp
		SimplePlume.cpp
		SimplePlume.h
		ColorUtil.cpp
		ColorUtil.h
		SpokeSpiralSearch.cpp
		SpokeSpiralSearch.h)

add_library(Spiri_controller SHARED
		SpiriController.h
		SpiriController.cpp
		MoveToPosition.cpp
		MoveToPosition.h
		MovementVector.cpp
		MovementVector.h
		Movement.h
		SimplePlume.cpp
		SimplePlume.h
		ColorUtil.cpp
		ColorUtil.h
		GasGradientDescentMovement.cpp
		GasGradientDescentMovement.h
		SpokeSpiralSearch.cpp
		SpokeSpiralSearch.h)

###############################################
# link shared object files to dependencies
###############################################

target_link_libraries(Gradient_loop_functions
		              Spiri_controller
                      argos3core_simulator
		              argos3plugin_simulator_spiri
                      argos3plugin_simulator_entities
                      argos3plugin_simulator_footbot
                      argos3plugin_simulator_genericrobot
                      argos3plugin_simulator_qtopengl
                      ${QT_LIBRARIES} ${GLUT_LIBRARY} ${OPENGL_LIBRARY})