#Main build setup of the project. 
#It is extended by ´run.mk` in each component directory. See those files for run examples.

# HIL: No spaces or comments after otherwise it captures them!
# Determine the platform
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
	CXX := clang++ -arch x86_64
else
	CXX := g++
endif

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

CREATE := app_CREATE
app_CREATE_FILES := new/tools/main-create.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp)) $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/my_libs/ -name *.cpp))

BF-SEQ := app_BF-SEQ
app_BF-SEQ_FILES := new/brute-force/main-bf.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp)) $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/my_libs/ -name *.cpp))

BF-ADJACENCY := app_BF-ADJACENCY
app_BF-ADJACENCY_FILES := new/brute-force/main-bf_adjacency.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp)) $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/my_libs/ -name *.cpp))

BF-EDGES := app_BF-EDGES
app_BF-EDGES_FILES := new/brute-force/main-bf_edges.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp)) $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/my_libs/ -name *.cpp))

BF-CYCLES := app_BF-CYCLES
app_BF-CYCLES_FILES := new/brute-force/main-bf_cycles.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp)) $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/my_libs/ -name *.cpp))

HR-H1v1 := app_HR-H1v1
app_HR-H1v1_FILES := new/heuristic/main-hr_H1v1.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp)) $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/my_libs/ -name *.cpp))

HR-H1v2 := app_HR-H1v2
app_HR-H1v2_FILES := new/heuristic/main-hr_H1v2.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp)) $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/my_libs/ -name *.cpp))

HR-H1v3 := app_HR-H1v3
app_HR-H1v3_FILES := new/heuristic/main-hr_H1v3.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp)) $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/my_libs/ -name *.cpp))

HR-H2v1 := app_HR-H2v1
app_HR-H2v1_FILES := new/heuristic/main-hr_H2v1.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp)) $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/my_libs/ -name *.cpp))

HR-H2v2 := app_HR-H2v2
app_HR-H2v2_FILES := new/heuristic/main-hr_H2v2.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp)) $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/my_libs/ -name *.cpp))

HR-H3v1 := app_HR-H3v1
app_HR-H3v1_FILES := new/heuristic/main-hr_H3v1.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp)) $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/my_libs/ -name *.cpp))

HR-H3v2 := app_HR-H3v2
app_HR-H3v2_FILES := new/heuristic/main-hr_H3v2.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp)) $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/my_libs/ -name *.cpp))

HR-H4v1 := app_HR-H4v1
app_HR-H4v1_FILES := new/heuristic/main-hr_H4v1.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp)) $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/my_libs/ -name *.cpp))

HR-H4v2r1 := app_HR-H4v2r1
app_HR-H4v2r1_FILES := new/heuristic/main-hr_H4v2r1.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp)) $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/my_libs/ -name *.cpp))

HR-H4v2r2 := app_HR-H4v2r2
app_HR-H4v2r2_FILES := new/heuristic/main-hr_H4v2r2.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp)) $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/my_libs/ -name *.cpp))

HR-H4v2r3 := app_HR-H4v2r3
app_HR-H4v2r3_FILES := new/heuristic/main-hr_H4v2r3.cpp $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/code/ -name *.cpp)) $(subst ${SRC_DIR}/, , $(shell find ${SRC_DIR}/new/my_libs/ -name *.cpp))

ALL_APP := ${CREATE} ${BF-SEQ} ${BF-ADJACENCY} ${BF-EDGES} ${BF-CYCLES} ${HR-H1v1} ${HR-H1v2} ${HR-H1v3} ${HR-H2v1} ${HR-H2v2} ${HR-H3v1} ${HR-H3v2} ${HR-H4v1} ${HR-H4v2r1} ${HR-H4v2r2} ${HR-H4v2r3}

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
