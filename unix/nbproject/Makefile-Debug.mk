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
	${OBJECTDIR}/_ext/511e4115/CBA.o \
	${OBJECTDIR}/_ext/511e4115/Component.o \
	${OBJECTDIR}/_ext/511e4115/EventDelegate.o \
	${OBJECTDIR}/_ext/9a3897b4/IApp.o \
	${OBJECTDIR}/_ext/24a135c6/POSIXConsoleApp.o \
	${OBJECTDIR}/_ext/511e4115/Status.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=-luuid -std=c++11
CXXFLAGS=-luuid -std=c++11

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libunix.a

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libunix.a: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libunix.a
	${AR} -rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libunix.a ${OBJECTFILES} 
	$(RANLIB) ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libunix.a

${OBJECTDIR}/_ext/511e4115/CBA.o: ../src/CBA.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/511e4115
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e4115/CBA.o ../src/CBA.cpp

${OBJECTDIR}/_ext/511e4115/Component.o: ../src/Component.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/511e4115
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e4115/Component.o ../src/Component.cpp

${OBJECTDIR}/_ext/511e4115/EventDelegate.o: ../src/EventDelegate.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/511e4115
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e4115/EventDelegate.o ../src/EventDelegate.cpp

${OBJECTDIR}/_ext/9a3897b4/IApp.o: ../src/Interfaces/IApp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/9a3897b4
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/9a3897b4/IApp.o ../src/Interfaces/IApp.cpp

${OBJECTDIR}/_ext/24a135c6/POSIXConsoleApp.o: ../src/Platforms/POSIXConsoleApp.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/24a135c6
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/24a135c6/POSIXConsoleApp.o ../src/Platforms/POSIXConsoleApp.cpp

${OBJECTDIR}/_ext/511e4115/Status.o: ../src/Status.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/511e4115
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/_ext/511e4115/Status.o ../src/Status.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libunix.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
