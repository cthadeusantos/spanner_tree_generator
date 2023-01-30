#Main build setup of the project. 
#It is extended by ´run.mk` in each component directory. See those files for run examples.

#--Configuration--
.DEFAULT_GOAL := build

SRC_DIR := ./src
INSTANCE_DIR := ./instances
WORKSPACE_DIR := ./workspace
DOC_DIR := ./documents/doxygen
BUILD_DIR := ./build

RELEASE_BUILD_DIR := ${BUILD_DIR}/release
DEBUG_BUILD_DIR := ${BUILD_DIR}/debug

CXXFLAGS := --std=c++11 -pthread -MMD -MP
RELEASE_FLAGS := -O3 -DNDEBUG -fno-exceptions -fno-rtti 
DEBUG_FLAGS := -g

#INCLUDE := -I${EXTERNAL_DIR}/
#SRC_INCLUDE := -I ${EXTERNAL_DIR}/

#--Applications--
##SEQ-OLD := app_SEQ-OLD
##app_SEQ-OLD_FILES := old/main.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/old/code/ -name *.cpp))
##PAR-OLD := app_PAR-OLD
##app_PAR-OLD_FILES := old/main_paralelo.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/old/code/ -name *.cpp))
#BF-SEQ := app_BF-SEQ
#app_BF-SEQ_FILES := new/main_BF-SEQ.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))
##ALL_APP := ${SEQ-OLD} ${PAR-OLD} ${BF-SEQ}
#TADM-CREATE := app_CREATE
#app_CREATE_FILES := new/main_create_graphs.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))
#ALL_APP := ${SEQ-OLD} ${PAR-OLD} ${BF-SEQ} ${TADM-CREATE}

CREATE := app_CREATE
app_CREATE_FILES := new/main-create.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))

BF-ALL := app_BF-ALL
app_BF-ALL_FILES := new/brute-force/main-bf_all.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))

BF-SEQ := app_BF-SEQ
app_BF-SEQ_FILES := new/brute-force/main-bf.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))

BF-PAR := app_BF-PAR
app_BF-PAR_FILES := new/brute-force/main-bf_par_threads.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))

BF-DEGREE := app_BF-DEGREE
app_BF-DEGREE_FILES := new/brute-force/main-bf_par_max_degree.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))

BF-CYCLES_M1 := app_BF-CYCLES_M1
app_BF-CYCLES_M1_FILES := new/brute-force/main-bf_par_cycles_m1.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))

BF-CYCLES_M2 := app_BF-CYCLES_M2
app_BF-CYCLES_M2_FILES := new/brute-force/main-bf_par_cycles_m2.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))

BF-ARTICULATION := app_BF-ARTICULATION
app_BF-ARTICULATION_FILES := new/brute-force/main-bf_par_articulation.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))

HR-ALL := app_HR-ALL
app_HR-ALL_FILES := new/heuristic/main-hr_all.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))

HR-H1v1 := app_HR-H1v1
app_HR-H1v1_FILES := new/heuristic/main-hr_H1v1.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))

HR-H1v2 := app_HR-H1v2
app_HR-H1v2_FILES := new/heuristic/main-hr_H1v2.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))

HR-H2v1 := app_HR-H2v1
app_HR-H2v1_FILES := new/heuristic/main-hr_H2v1.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))

HR-H2v2 := app_HR-H2v2
app_HR-H2v2_FILES := new/heuristic/main-hr_H2v2.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))

HR-H3v1 := app_HR-H3v1
app_HR-H3v1_FILES := new/heuristic/main-hr_H3v1.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))

HR-H3v2 := app_HR-H3v2
app_HR-H3v2_FILES := new/heuristic/main-hr_H3v2.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))

HR-BREADTHv1 := app_HR-BREADTHv1
app_HR-BREADTHv1_FILES := new/heuristic/main-hr_Hbreadthv1.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))

HR-BREADTHv2 := app_HR-BREADTHv2
app_HR-BREADTHv2_FILES := new/heuristic/main-hr_Hbreadthv2.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))

##ALL_APP := ${SEQ-OLD} ${PAR-OLD} ${BF-SEQ} ${TADM-CREATE} ${BF-PAR}
#BF-MAIN := app_MAIN
#app_MAIN_FILES := new/main.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp))
ALL_APP := ${CREATE} ${BF-ALL} ${BF-SEQ} ${BF-PAR} ${BF-DEGREE} ${BF-CYCLES_M1} ${BF-CYCLES_M2} ${BF-ARTICULATION} ${HR-ALL} ${HR-H1v1} ${HR-H1v2} ${HR-H2v1} ${HR-H2v2} ${HR-H3v1} ${HR-H3v2} ${HR-BREADTHv1} ${HR-BREADTHv2} ${SEQ-OLD} ${PAR-OLD} ${TADM-CREATE} ${BF-MAIN}

#--Main rule names--
RELEASE_BUILD := build-release
DEBUG_BUILD := build-debug
DOC_BUILD := build-doc

RELEASE_RUN := run-release
DEBUG_RUN := run-debug

RELEASE_CLEAN := clean-release
DEBUG_CLEAN := clean-debug
DOC_CLEAN := clean-doc

#--Auxiliary Rules--
.PHONY: all build run clean doc
all: ${RELEASE_CLEAN} ${RELEASE_BUILD}
build: ${RELEASE_BUILD}
run: ${RELEASE_RUN}
debug: ${DEBUG_BUILD}
clean: ${RELEASE_CLEAN} ${DEBUG_CLEAN}
doc: ${DOC_BUILD}

#--Build rules--
.PHONY: ${RELEASE_BUILD} ${DEBUG_BUILD} ${DOC_BUILD}
${RELEASE_BUILD}: CXXFLAGS := ${CXXFLAGS} ${RELEASE_FLAGS}
${RELEASE_BUILD}: INCLUDE := ${INCLUDE} ${SRC_INCLUDE}
${RELEASE_BUILD}: $(addprefix ${RELEASE_BUILD_DIR}/, ${ALL_APP})

${DEBUG_BUILD}: CXXFLAGS := ${CXXFLAGS} ${DEBUG_FLAGS}
${DEBUG_BUILD}: INCLUDE := ${INCLUDE} ${SRC_INCLUDE}
${DEBUG_BUILD}: $(addprefix ${DEBUG_BUILD_DIR}/, ${ALL_APP})

define APP_template =
${2}/${1}: $(addsuffix .o, $(addprefix ${2}/, $(${1}_FILES)))
endef

$(foreach app,${ALL_APP},$(eval $(call APP_template,${app},${RELEASE_BUILD_DIR})))
$(foreach app,${ALL_APP},$(eval $(call APP_template,${app},${DEBUG_BUILD_DIR})))

$(addsuffix /app_%, ${RELEASE_BUILD_DIR} ${DEBUG_BUILD_DIR}):
	${CXX} ${CXXFLAGS} ${INCLUDE} $^ -o $@

$(addsuffix /%.cpp.o, ${RELEASE_BUILD_DIR} ${DEBUG_BUILD_DIR}): ${SRC_DIR}/%.cpp
	${MKDIR_P} ${dir $@}
	${CXX} ${CXXFLAGS} ${INCLUDE} -c $< -o $@

${DOC_BUILD}:
	cd ${DOC_DIR} && doxygen

#--Clean rules--
.PHONY: ${RELEASE_CLEAN} ${DEBUG_CLEAN} ${DOC_CLEAN}
${RELEASE_CLEAN}:
	${RM} -r $(addprefix ${RELEASE_BUILD_DIR}/, *.o *.d app_* ./*/)

${DEBUG_CLEAN}:
	${RM} -r $(addprefix ${DEBUG_BUILD_DIR}/, *.o *.d app_* ./*/)

${DOC_CLEAN}:
	${RM} -r ${DOC_DIR}/html ${DOC_DIR}/latex

#--Auxiliary variables--
RELEASE_DEPS := $(shell find ${RELEASE_BUILD_DIR} -name *.d)
DEBUG_DEPS := $(shell find ${DEBUG_BUILD_DIR} -name *.d)
MKDIR_P := mkdir -p

#--Execution rules--
.PHONY: ${RELEASE_RUN} ${DEBUG_RUN}
include ${SRC_DIR}/run.mk

#--Dependency rules--
-include $(RELEASE_DEPS)
-include $(DEBUG_DEPS)