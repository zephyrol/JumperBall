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
CND_CONF=Release
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
	${OBJECTDIR}/main.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lglm_shared

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/balljumper

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/balljumper: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/balljumper ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/coreClasses/Ball.o: coreClasses/Ball.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/Ball.o coreClasses/Ball.cpp

${OBJECTDIR}/coreClasses/Camera.o: coreClasses/Camera.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/Camera.o coreClasses/Camera.cpp

${OBJECTDIR}/coreClasses/ClassicalMechanics.o: coreClasses/ClassicalMechanics.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/ClassicalMechanics.o coreClasses/ClassicalMechanics.cpp

${OBJECTDIR}/coreClasses/Map.o: coreClasses/Map.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/Map.o coreClasses/Map.cpp

${OBJECTDIR}/coreClasses/blocks/BaseBlock.o: coreClasses/blocks/BaseBlock.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/BaseBlock.o coreClasses/blocks/BaseBlock.cpp

${OBJECTDIR}/coreClasses/blocks/Block.o: coreClasses/blocks/Block.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/Block.o coreClasses/blocks/Block.cpp

${OBJECTDIR}/coreClasses/blocks/BrittleBlock.o: coreClasses/blocks/BrittleBlock.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/BrittleBlock.o coreClasses/blocks/BrittleBlock.cpp

${OBJECTDIR}/coreClasses/blocks/FireBlock.o: coreClasses/blocks/FireBlock.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/FireBlock.o coreClasses/blocks/FireBlock.cpp

${OBJECTDIR}/coreClasses/blocks/IceBlock.o: coreClasses/blocks/IceBlock.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/IceBlock.o coreClasses/blocks/IceBlock.cpp

${OBJECTDIR}/coreClasses/blocks/SpicyBlock.o: coreClasses/blocks/SpicyBlock.cpp
	${MKDIR} -p ${OBJECTDIR}/coreClasses/blocks
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/coreClasses/blocks/SpicyBlock.o coreClasses/blocks/SpicyBlock.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -O2 -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

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
