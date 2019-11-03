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
	${OBJECTDIR}/coreClasses/Camera.o \
	${OBJECTDIR}/coreClasses/ClassicalMechanics.o \
	${OBJECTDIR}/coreClasses/Map.o \
	${OBJECTDIR}/coreClasses/blocks/BaseBlock.o \
	${OBJECTDIR}/coreClasses/blocks/Block.o \
	${OBJECTDIR}/coreClasses/blocks/BrittleBlock.o \
	${OBJECTDIR}/coreClasses/blocks/FireBlock.o \
	${OBJECTDIR}/coreClasses/blocks/IceBlock.o \
	${OBJECTDIR}/coreClasses/blocks/SpicyBlock.o \
	${OBJECTDIR}/graphicsClasses/Shader.o \
	${OBJECTDIR}/graphicsClasses/ShaderProgram.o \
	${OBJECTDIR}/graphicsClasses/Utility.o \
	${OBJECTDIR}/graphicsClasses/testClass.o \
	${OBJECTDIR}/libs/glad/glad.o \
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-Werror -pedantic -Wall -Wextra -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Weffc++ -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wlogical-op -Winline -Wformat=2 -Wformat-truncation=2 -Wmisleading-indentation
CXXFLAGS=-Werror -pedantic -Wall -Wextra -Woverloaded-virtual -Wfloat-equal -Wwrite-strings -Wpointer-arith -Wcast-qual -Wcast-align -Weffc++ -Wredundant-decls -Wdouble-promotion -Winit-self -Wswitch-default -Wswitch-enum -Wlogical-op -Winline -Wformat=2 -Wformat-truncation=2 -Wmisleading-indentation

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
	$(COMPILE.cc) -g -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/Ball.o coreClasses/Ball.cpp

${OBJECTDIR}/coreClasses/Camera.o: coreClasses/Camera.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/Camera.o coreClasses/Camera.cpp

${OBJECTDIR}/coreClasses/ClassicalMechanics.o: coreClasses/ClassicalMechanics.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/ClassicalMechanics.o coreClasses/ClassicalMechanics.cpp

${OBJECTDIR}/coreClasses/Map.o: coreClasses/Map.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/Map.o coreClasses/Map.cpp

${OBJECTDIR}/coreClasses/blocks/BaseBlock.o: coreClasses/blocks/BaseBlock.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -g -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/BaseBlock.o coreClasses/blocks/BaseBlock.cpp

${OBJECTDIR}/coreClasses/blocks/Block.o: coreClasses/blocks/Block.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -g -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/Block.o coreClasses/blocks/Block.cpp

${OBJECTDIR}/coreClasses/blocks/BrittleBlock.o: coreClasses/blocks/BrittleBlock.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -g -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/BrittleBlock.o coreClasses/blocks/BrittleBlock.cpp

${OBJECTDIR}/coreClasses/blocks/FireBlock.o: coreClasses/blocks/FireBlock.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -g -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/FireBlock.o coreClasses/blocks/FireBlock.cpp

${OBJECTDIR}/coreClasses/blocks/IceBlock.o: coreClasses/blocks/IceBlock.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -g -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/IceBlock.o coreClasses/blocks/IceBlock.cpp

${OBJECTDIR}/coreClasses/blocks/SpicyBlock.o: coreClasses/blocks/SpicyBlock.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -g -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/SpicyBlock.o coreClasses/blocks/SpicyBlock.cpp

${OBJECTDIR}/graphicsClasses/Shader.o: graphicsClasses/Shader.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/Shader.o graphicsClasses/Shader.cpp

${OBJECTDIR}/graphicsClasses/ShaderProgram.o: graphicsClasses/ShaderProgram.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/ShaderProgram.o graphicsClasses/ShaderProgram.cpp

${OBJECTDIR}/graphicsClasses/Utility.o: graphicsClasses/Utility.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/Utility.o graphicsClasses/Utility.cpp

${OBJECTDIR}/graphicsClasses/testClass.o: graphicsClasses/testClass.cpp
	${MKDIR} -p ${OBJECTDIR}/graphicsClasses
	${RM} "$@.d"
	$(COMPILE.cc) -g -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/graphicsClasses/testClass.o graphicsClasses/testClass.cpp

${OBJECTDIR}/libs/glad/glad.o: libs/glad/glad.c
	${MKDIR} -p ${OBJECTDIR}/libs/glad
	${RM} "$@.d"
	$(COMPILE.c) -g -Ilibs -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/libs/glad/glad.o libs/glad/glad.c

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -IcoreClasses -Ilibs -std=c++11 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

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
