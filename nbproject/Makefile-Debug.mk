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
	${OBJECTDIR}/N2kDeviceList.o \
	${OBJECTDIR}/N2kGroupFunction.o \
	${OBJECTDIR}/N2kGroupFunctionDefaultHandlers.o \
	${OBJECTDIR}/N2kMessages.o \
	${OBJECTDIR}/N2kMsg.o \
	${OBJECTDIR}/N2kStream.o \
	${OBJECTDIR}/NMEA2000.o \
	${OBJECTDIR}/NMEA2000_SocketCAN.o \
	${OBJECTDIR}/TrilliumBatteryInterface.o \
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
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/battmonpi

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/battmonpi: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/battmonpi ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/N2kDeviceList.o: N2kDeviceList.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/N2kDeviceList.o N2kDeviceList.cpp

${OBJECTDIR}/N2kGroupFunction.o: N2kGroupFunction.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/N2kGroupFunction.o N2kGroupFunction.cpp

${OBJECTDIR}/N2kGroupFunctionDefaultHandlers.o: N2kGroupFunctionDefaultHandlers.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/N2kGroupFunctionDefaultHandlers.o N2kGroupFunctionDefaultHandlers.cpp

${OBJECTDIR}/N2kMessages.o: N2kMessages.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/N2kMessages.o N2kMessages.cpp

${OBJECTDIR}/N2kMsg.o: N2kMsg.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/N2kMsg.o N2kMsg.cpp

${OBJECTDIR}/N2kStream.o: N2kStream.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/N2kStream.o N2kStream.cpp

${OBJECTDIR}/NMEA2000.o: NMEA2000.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/NMEA2000.o NMEA2000.cpp

${OBJECTDIR}/NMEA2000_SocketCAN.o: NMEA2000_SocketCAN.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/NMEA2000_SocketCAN.o NMEA2000_SocketCAN.cpp

${OBJECTDIR}/TrilliumBatteryInterface.o: TrilliumBatteryInterface.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/TrilliumBatteryInterface.o TrilliumBatteryInterface.cpp

${OBJECTDIR}/main.o: main.cpp
	${MKDIR} -p ${OBJECTDIR}
	${RM} "$@.d"
	$(COMPILE.cc) -g -MMD -MP -MF "$@.d" -o ${OBJECTDIR}/main.o main.cpp

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
