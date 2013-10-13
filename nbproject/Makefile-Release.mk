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
CND_PLATFORM=GNU-Linux-x86
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
	${OBJECTDIR}/minisat/Constraints.o \
	${OBJECTDIR}/minisat/Solver.o \
	${OBJECTDIR}/src/CNFUtils.o \
	${OBJECTDIR}/src/ClakeCompletion.o \
	${OBJECTDIR}/src/DependenceGraph.o \
	${OBJECTDIR}/src/NNFUtils.o \
	${OBJECTDIR}/src/Rule.o \
	${OBJECTDIR}/src/SATSolver.o \
	${OBJECTDIR}/src/Utils.o \
	${OBJECTDIR}/src/Vocabulary.o \
	${OBJECTDIR}/src/global.o \
	${OBJECTDIR}/src/lex.o \
	${OBJECTDIR}/src/main.o \
	${OBJECTDIR}/src/parse.o


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
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nlpsolver

${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nlpsolver: ${OBJECTFILES}
	${MKDIR} -p ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nlpsolver ${OBJECTFILES} ${LDLIBSOPTIONS}

${OBJECTDIR}/minisat/Constraints.o: minisat/Constraints.C 
	${MKDIR} -p ${OBJECTDIR}/minisat
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/minisat/Constraints.o minisat/Constraints.C

${OBJECTDIR}/minisat/Solver.o: minisat/Solver.C 
	${MKDIR} -p ${OBJECTDIR}/minisat
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/minisat/Solver.o minisat/Solver.C

${OBJECTDIR}/src/CNFUtils.o: src/CNFUtils.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/CNFUtils.o src/CNFUtils.cpp

${OBJECTDIR}/src/ClakeCompletion.o: src/ClakeCompletion.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/ClakeCompletion.o src/ClakeCompletion.cpp

${OBJECTDIR}/src/DependenceGraph.o: src/DependenceGraph.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/DependenceGraph.o src/DependenceGraph.cpp

${OBJECTDIR}/src/NNFUtils.o: src/NNFUtils.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/NNFUtils.o src/NNFUtils.cpp

${OBJECTDIR}/src/Rule.o: src/Rule.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Rule.o src/Rule.cpp

${OBJECTDIR}/src/SATSolver.o: src/SATSolver.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/SATSolver.o src/SATSolver.cpp

${OBJECTDIR}/src/Utils.o: src/Utils.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Utils.o src/Utils.cpp

${OBJECTDIR}/src/Vocabulary.o: src/Vocabulary.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/Vocabulary.o src/Vocabulary.cpp

${OBJECTDIR}/src/global.o: src/global.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/global.o src/global.cpp

${OBJECTDIR}/src/lex.o: src/lex.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/lex.o src/lex.cpp

${OBJECTDIR}/src/main.o: src/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/main.o src/main.cpp

${OBJECTDIR}/src/parse.o: src/parse.cpp 
	${MKDIR} -p ${OBJECTDIR}/src
	${RM} $@.d
	$(COMPILE.cc) -O2 -MMD -MP -MF $@.d -o ${OBJECTDIR}/src/parse.o src/parse.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}
	${RM} ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/nlpsolver

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
