#
#	$Id$
#
# GU monitor whiteboard module Makefile
#
BIN=guWhiteboardMonitor

WERROR=	# FIXME: needs fixing of the code generating the warnings

ALL_TARGETS=host robot

CPP_SRCS=monitor.cpp
HDRS=monitor.h 

NEW_WHITEBOARD_SRCS+=guwhiteboardgetter.cpp guwhiteboardposter.cpp

.include "../../mk/whiteboard.mk"	# required for whiteboard clients
.include "../../mk/mipal.mk"		# comes last!
