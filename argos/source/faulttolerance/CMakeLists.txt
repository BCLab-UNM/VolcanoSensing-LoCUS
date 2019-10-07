###############################################
# define shared object files
###############################################

add_library(Gradient_loop_functions MODULE FaultToleranceLoopFunctions.h
		FaultToleranceLoopFunctions.cpp
		FaultToleranceQTUserFunctions.h
		FaultToleranceQTUserFunctions.cpp
		ThenMovement.cpp
		ThenMovement.h
		ReplaceWithHeir.cpp
		ReplaceWithHeir.h
		ParallelMovement.cpp
		ParallelMovement.h
		SimplePlume.cpp
		SimplePlume.h
		ColorUtil.cpp
		ColorUtil.h)

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
		WaitForChildren.cpp
		WaitForChildren.h
		EmptyMovement.h
        Finishable.h
		ReplaceWithHeir.cpp
		ReplaceWithHeir.h
		ParallelMovement.cpp
		ParallelMovement.h
		SwarmLocation.h
		ControllerBase.h
		SwarmManager.cpp
		SwarmManager.h
		SwarmLocationImpl.cpp
		SwarmLocationImpl.h
        BalanceMovement.cpp BalanceMovement.h
		SimplePlume.cpp
		SimplePlume.h
		ColorUtil.cpp
		ColorUtil.h
		PositionReading.h)

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