#
#	$Id$
#
# GU monitor whiteboard module Makefile
#
BIN=guWhiteboardMonitor

ALL_TARGETS=host robot analyse xc doc

CPP_SRCS=monitor.cpp
HDRS=monitor.h 
DOC_HDRS=mainpage.md
CI_SERVER_DOC_SUBDIR=utils

NEW_WHITEBOARD_SRCS+=guwhiteboardgetter.cpp guwhiteboardposter.cpp

.include "../../mk/c++11.mk"
.include "../../mk/whiteboard.mk"	# required for whiteboard clients
.include "../../mk/mipal.mk"		# comes last!
