#
#	$Id$
#
# GU monitor whiteboard module Makefile
#
BIN=guWhiteboardMonitor

ALL_TARGETS=host atom doc
DEPENDS=gusimplewhiteboard

CPP_SRCS=monitor.cpp
HDRS=monitor.h 
DOC_HDRS=README.md
CI_SERVER_DOC_SUBDIR=utils

NEW_WHITEBOARD_SRCS+=guwhiteboardgetter.cpp guwhiteboardposter.cpp

.include "../../mk/c++11.mk"
.include "../../mk/whiteboard.mk"	# required for whiteboard clients
.include "../../mk/mipal.mk"		# comes last!
