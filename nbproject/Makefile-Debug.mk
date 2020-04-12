#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=gfortran
AS=as

# Macros
CND_PLATFORM=GNU-Linux
CND_DLIB_EXT=so
CND_CONF=Debug
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/coreClasses/Ball.o \
	${OBJECTDIR}/coreClasses/ClassicalMechanics.o \
	${OBJECTDIR}/coreClasses/Map.o \
	${OBJECTDIR}/coreClasses/Types.o \
	${OBJECTDIR}/coreClasses/blocks/BaseBlock.o \
	${OBJECTDIR}/coreClasses/blocks/Block.o \
	${OBJECTDIR}/coreClasses/blocks/BrittleBlock.o \
	${OBJECTDIR}/coreClasses/blocks/FireBlock.o \
	${OBJECTDIR}/coreClasses/blocks/IceBlock.o \
	${OBJECTDIR}/coreClasses/blocks/JumpBlock.o \
	${OBJECTDIR}/coreClasses/blocks/SharpBlock.o \
	${OBJECTDIR}/coreClasses/objects/Bonus.o \
	${OBJECTDIR}/coreClasses/objects/Clock.o \
	${OBJECTDIR}/coreClasses/objects/Coin.o \
	${OBJECTDIR}/coreClasses/objects/Key.o \
	${OBJECTDIR}/coreClasses/objects/Object.o \
	${OBJECTDIR}/graphicsClasses/Camera.o \
	${OBJECTDIR}/graphicsClasses/FrameBuffer.o \
	${OBJECTDIR}/graphicsClasses/MeshComponent.o \
	${OBJECTDIR}/graphicsClasses/Rendering.o \
	${OBJECTDIR}/graphicsClasses/Shader.o \
	${OBJECTDIR}/graphicsClasses/ShaderProgram.o \
	${OBJECTDIR}/graphicsClasses/Star.o \
	${OBJECTDIR}/graphicsClasses/Utility.o \
	${OBJECTDIR}/graphicsClasses/animations/Animation.o \
	${OBJECTDIR}/graphicsClasses/animations/BallAnimation.o \
	${OBJECTDIR}/graphicsClasses/animations/ObjectAnimation.o \
	${OBJECTDIR}/graphicsClasses/geometry/Cube.o \
	${OBJECTDIR}/graphicsClasses/geometry/Cylinder.o \
	${OBJECTDIR}/graphicsClasses/geometry/GeometricShape.o \
	${OBJECTDIR}/graphicsClasses/geometry/Pyramid.o \
	${OBJECTDIR}/graphicsClasses/geometry/Quad.o \
	${OBJECTDIR}/graphicsClasses/geometry/Sphere.o \
	${OBJECTDIR}/graphicsClasses/testClass.o \
	${OBJECTDIR}/graphicsClasses/uniformBlocks/uniformBlock.o \
	${OBJECTDIR}/graphicsClasses/uniformBlocks/uniformLight.o \
	${OBJECTDIR}/libs/glad/glad.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-pedantic -Wall -Wextra -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wlogical-op
CXXFLAGS=-pedantic -Wall -Wextra -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wlogical-op

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/jumperball

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/jumperball: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/jumperball ${OBJECTFILES} ${LDLIBSOPTIONS} -lglfw -ldl

${OBJECTDIR}/coreClasses/Ball.o: coreClasses/Ball.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/Ball.o coreClasses/Ball.cpp

${OBJECTDIR}/coreClasses/ClassicalMechanics.o: coreClasses/ClassicalMechanics.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/ClassicalMechanics.o coreClasses/ClassicalMechanics.cpp

${OBJECTDIR}/coreClasses/Map.o: coreClasses/Map.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/Map.o coreClasses/Map.cpp

${OBJECTDIR}/coreClasses/Types.o: coreClasses/Types.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/Types.o coreClasses/Types.cpp

${OBJECTDIR}/coreClasses/blocks/BaseBlock.o: coreClasses/blocks/BaseBlock.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/BaseBlock.o coreClasses/blocks/BaseBlock.cpp

${OBJECTDIR}/coreClasses/blocks/Block.o: coreClasses/blocks/Block.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/Block.o coreClasses/blocks/Block.cpp

${OBJECTDIR}/coreClasses/blocks/BrittleBlock.o: coreClasses/blocks/BrittleBlock.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/BrittleBlock.o coreClasses/blocks/BrittleBlock.cpp

${OBJECTDIR}/coreClasses/blocks/FireBlock.o: coreClasses/blocks/FireBlock.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/FireBlock.o coreClasses/blocks/FireBlock.cpp

${OBJECTDIR}/coreClasses/blocks/IceBlock.o: coreClasses/blocks/IceBlock.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/IceBlock.o coreClasses/blocks/IceBlock.cpp

${OBJECTDIR}/coreClasses/blocks/JumpBlock.o: coreClasses/blocks/JumpBlock.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/JumpBlock.o coreClasses/blocks/JumpBlock.cpp

${OBJECTDIR}/coreClasses/blocks/SharpBlock.o: coreClasses/blocks/SharpBlock.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/SharpBlock.o coreClasses/blocks/SharpBlock.cpp

${OBJECTDIR}/coreClasses/objects/Bonus.o: coreClasses/objects/Bonus.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/objects
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/objects/Bonus.o coreClasses/objects/Bonus.cpp

${OBJECTDIR}/coreClasses/objects/Clock.o: coreClasses/objects/Clock.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/objects
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/objects/Clock.o coreClasses/objects/Clock.cpp

${OBJECTDIR}/coreClasses/objects/Coin.o: coreClasses/objects/Coin.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/objects
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/objects/Coin.o coreClasses/objects/Coin.cpp

${OBJECTDIR}/coreClasses/objects/Key.o: coreClasses/objects/Key.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/objects
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/objects/Key.o coreClasses/objects/Key.cpp

${OBJECTDIR}/coreClasses/objects/Object.o: coreClasses/objects/Object.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/objects
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/objects/Object.o coreClasses/objects/Object.cpp

${OBJECTDIR}/graphicsClasses/Camera.o: graphicsClasses/Camera.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/Camera.o graphicsClasses/Camera.cpp

${OBJECTDIR}/graphicsClasses/FrameBuffer.o: graphicsClasses/FrameBuffer.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/FrameBuffer.o graphicsClasses/FrameBuffer.cpp

${OBJECTDIR}/graphicsClasses/MeshComponent.o: graphicsClasses/MeshComponent.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/MeshComponent.o graphicsClasses/MeshComponent.cpp

${OBJECTDIR}/graphicsClasses/Rendering.o: graphicsClasses/Rendering.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/Rendering.o graphicsClasses/Rendering.cpp

${OBJECTDIR}/graphicsClasses/Shader.o: graphicsClasses/Shader.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/Shader.o graphicsClasses/Shader.cpp

${OBJECTDIR}/graphicsClasses/ShaderProgram.o: graphicsClasses/ShaderProgram.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/ShaderProgram.o graphicsClasses/ShaderProgram.cpp

${OBJECTDIR}/graphicsClasses/Star.o: graphicsClasses/Star.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/Star.o graphicsClasses/Star.cpp

${OBJECTDIR}/graphicsClasses/Utility.o: graphicsClasses/Utility.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/Utility.o graphicsClasses/Utility.cpp

${OBJECTDIR}/graphicsClasses/animations/Animation.o: graphicsClasses/animations/Animation.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses/animations
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/animations/Animation.o graphicsClasses/animations/Animation.cpp

${OBJECTDIR}/graphicsClasses/animations/BallAnimation.o: graphicsClasses/animations/BallAnimation.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses/animations
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/animations/BallAnimation.o graphicsClasses/animations/BallAnimation.cpp

${OBJECTDIR}/graphicsClasses/animations/ObjectAnimation.o: graphicsClasses/animations/ObjectAnimation.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses/animations
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/animations/ObjectAnimation.o graphicsClasses/animations/ObjectAnimation.cpp

${OBJECTDIR}/graphicsClasses/geometry/Cube.o: graphicsClasses/geometry/Cube.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses/geometry
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/geometry/Cube.o graphicsClasses/geometry/Cube.cpp

${OBJECTDIR}/graphicsClasses/geometry/Cylinder.o: graphicsClasses/geometry/Cylinder.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses/geometry
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/geometry/Cylinder.o graphicsClasses/geometry/Cylinder.cpp

${OBJECTDIR}/graphicsClasses/geometry/GeometricShape.o: graphicsClasses/geometry/GeometricShape.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses/geometry
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/geometry/GeometricShape.o graphicsClasses/geometry/GeometricShape.cpp

${OBJECTDIR}/graphicsClasses/geometry/Pyramid.o: graphicsClasses/geometry/Pyramid.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses/geometry
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/geometry/Pyramid.o graphicsClasses/geometry/Pyramid.cpp

${OBJECTDIR}/graphicsClasses/geometry/Quad.o: graphicsClasses/geometry/Quad.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses/geometry
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/geometry/Quad.o graphicsClasses/geometry/Quad.cpp

${OBJECTDIR}/graphicsClasses/geometry/Sphere.o: graphicsClasses/geometry/Sphere.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses/geometry
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/geometry/Sphere.o graphicsClasses/geometry/Sphere.cpp

${OBJECTDIR}/graphicsClasses/testClass.o: graphicsClasses/testClass.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/testClass.o graphicsClasses/testClass.cpp

${OBJECTDIR}/graphicsClasses/uniformBlocks/uniformBlock.o: graphicsClasses/uniformBlocks/uniformBlock.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses/uniformBlocks
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/uniformBlocks/uniformBlock.o graphicsClasses/uniformBlocks/uniformBlock.cpp

${OBJECTDIR}/graphicsClasses/uniformBlocks/uniformLight.o: graphicsClasses/uniformBlocks/uniformLight.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses/uniformBlocks
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/uniformBlocks/uniformLight.o graphicsClasses/uniformBlocks/uniformLight.cpp

${OBJECTDIR}/libs/glad/glad.o: libs/glad/glad.c
	${MKDIR} -p ${OBJECTDIR}/libs/glad
	${RM} "$@.d"
	$(COMPILE.c) -g -Ilibs -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/libs/glad/glad.o libs/glad/glad.c

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -Wall -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
