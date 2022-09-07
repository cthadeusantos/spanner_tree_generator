#This file extends the makefile in the root directory with run examples. It is not intended to be executed by itself.
#The following rules are little tests done through the project's development.
#Each run has its own name, but you may add the suffix "-db" to use the debug app.

#--Configuration--
BF_SEQ_OUT_DIR := ${WORKSPACE_DIR}/bf-seq-out
EXEC_DIR = ${RELEASE_BUILD_DIR}

#--Default rules--
${RELEASE_RUN}: bf-seq-grafo-10
${DEBUG_RUN}: bf-seq-grafo-10-db 

#--Sequential Brute Force--
bf-seq-grafo-10-db: EXEC_DIR = ${DEBUG_BUILD_DIR}
bf-seq-grafo-10-db: bf-seq-grafo-10
bf-seq-grafo-10:
	${MKDIR_P} ${BF_SEQ_OUT_DIR}
	${EXEC_DIR}/${BF-SEQ} < ${INSTANCE_DIR}/grafos/grafo_10.txt > ${BF_SEQ_OUT_DIR}/grafo_10.out 2> ${BF_SEQ_OUT_DIR}/grafo_10.err

bf-seq-help-db: EXEC_DIR = ${DEBUG_BUILD_DIR}
bf-seq-help-db: bf-seq-help
bf-seq-help: #Application configuration
	${EXEC_DIR}/${BF-SEQ} --help