###############################################
# define shared object files
###############################################

add_library(Gradient_loop_functions MODULE FaultToleranceLoopFunctions.h
		FaultToleranceLoopFunctions.cpp
		FaultToleranceQTUserFunctions.h
		FaultToleranceQTUserFunctions.cpp)

add_library(Gradient_controller     SHARED FaultToleranceController.h
        FaultToleranceController.cpp
		FaultToleranceLoopFunctions.h
		FaultToleranceLoopFunctions.cpp
		MoveTo.cpp
		MoveTo.h
		Movement.h
		CollisionAvoidance.cpp
		CollisionAvoidance.h
		MovementVector.cpp
		MovementVector.h
		Delay.cpp
		Delay.h
		WaypointUtil.cpp
		WaypointUtil.h CollisionAvoidance.cpp CollisionAvoidance.h)

add_library(Spiri_controller SHARED
		SpiriController.h
		SpiriController.cpp
		MoveToPosition.cpp
		MoveToPosition.h
		MovementVector.cpp
		MovementVector.h
		Movement.h
		Delay.cpp
		Delay.h
		)

###############################################
# link shared object files to dependencies
###############################################

target_link_libraries(Gradient_controller)

target_link_libraries(Gradient_loop_functions
                      Gradient_controller
                      argos3core_simulator
		              argos3plugin_simulator_spiri
                      argos3plugin_simulator_entities
                      argos3plugin_simulator_footbot
                      argos3plugin_simulator_genericrobot
                      argos3plugin_simulator_qtopengl
                      ${QT_LIBRARIES} ${GLUT_LIBRARY} ${OPENGL_LIBRARY})