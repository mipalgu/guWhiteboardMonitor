guWhiteboardMonitor {#mainpage}
=====================

`guWhiteboardMonitor` is a command line utility that allows live monitoring
of `gusimplewhiteboard` messages.

# Usage #

`guWhiteboardMonitor [messages ...]`

`messages` is an optional parameter specifying the whiteboard messages to monitor.
If left out, all whiteboard messages will be monitored.

## Example ##

`guWhiteboardMonitor Speech fsm_status`

This will monitor `Speech` and finite state machine status (`fsm_status`) messages.

# Compiling #

### Building for the Host ###
To build the tool for the host use `bmake host`.
To install under `$PREFIX`, use `sudo bmake install`

### Building for a Target ###
To build the tool for the default target, simply use
`bmake robot`.
Alternatively, to build the tool for a different target, use
`bmake target TARGET=`*targetname*.

### Creating the ROS Makefile ###

**ROS is currently not supported.**  Once supported, you should be able to
export the sources into `catkin_ws` for ROS using `bmake catkin`.

## ROS ##

**ROS is currently not supported.**  Once supported, the following instructions should apply:
To build the tool for ROS, make sure you have copied the
sources into `catkin_ws` as described above, then simply use
`catkin_make`.
To permanently install the tool, use `catkin_make install`.

