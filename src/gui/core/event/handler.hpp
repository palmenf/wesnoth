/*
	Copyright (C) 2009 - 2022
	by Mark de Wever <koraq@xs4all.nl>
	Part of the Battle for Wesnoth Project https://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#pragma once

#include <iosfwd>
#include <vector>

namespace gui2
{
class window;

namespace event
{
class dispatcher;

class manager
{
public:
	manager();
	~manager();
};

/**
 * The event send to the dispatcher.
 *
 * Events prefixed by SDL are (semi)-real SDL events. The handler does some
 * minor decoding like splitting the button down event to the proper event but
 * nothing more. Events without an SDL prefix are generated by another signal
 * eg the windows signal handler for SDL_MOUSE_MOTION can generate a
 * MOUSE_ENTER, MOUSE_MOTION and MOUSE_LEAVE event and send that to it's
 * children.
 *
 * @note When adding a new entry to the enum also add a unit test.
 */
enum ui_event {
	SDL_ACTIVATE,                   /**< The main application window is activated. */
	DRAW,                           /**< Periodic redraw request. */
	CLOSE_WINDOW,                   /**< A request to close the current window. */
	SDL_VIDEO_RESIZE,               /**< An SDL resize request, coordinate is the new window size. */

	SDL_MOUSE_MOTION,               /**< An SDL mouse motion event. */
	MOUSE_ENTER,                    /**< A mouse enter event for a widget. */
	MOUSE_MOTION,                   /**< A mouse motion event for a widget. */
	MOUSE_LEAVE,                    /**< A mouse leave event for a widget. */

	SDL_LEFT_BUTTON_DOWN,           /**< An SDL left mouse button down event. */
	SDL_LEFT_BUTTON_UP,             /**< An SDL left mouse button up event. */
	LEFT_BUTTON_DOWN,               /**< A left mouse button down event for a widget. */
	LEFT_BUTTON_UP,                 /**< A left mouse button up event for a widget. */
	LEFT_BUTTON_CLICK,              /**< A left mouse button click event for a widget. */
	LEFT_BUTTON_DOUBLE_CLICK,       /**< A left mouse button double click event for a widget. */

	SDL_MIDDLE_BUTTON_DOWN,         /**< An SDL middle mouse button down event. */
	SDL_MIDDLE_BUTTON_UP,           /**< An SDL middle mouse button up event. */
	MIDDLE_BUTTON_DOWN,             /**< See LEFT_BUTTON_DOWN. */
	MIDDLE_BUTTON_UP,               /**< See LEFT_BUTTON_UP. */
	MIDDLE_BUTTON_CLICK,            /**< See LEFT_BUTTON_CLICK. */
	MIDDLE_BUTTON_DOUBLE_CLICK,     /**< See LEFT_BUTTON_DOUBLE_CLICK. */

	SDL_RIGHT_BUTTON_DOWN,          /**< An SDL right mouse button down event. */
	SDL_RIGHT_BUTTON_UP,            /**< An SDL right mouse button up event. */
	RIGHT_BUTTON_DOWN,              /**< See LEFT_BUTTON_DOWN. */
	RIGHT_BUTTON_UP,                /**< See LEFT_BUTTON_UP. */
	RIGHT_BUTTON_CLICK,             /**< See LEFT_BUTTON_CLICK. */
	RIGHT_BUTTON_DOUBLE_CLICK,      /**< See LEFT_BUTTON_DOUBLE_CLICK. */

	SDL_WHEEL_LEFT,                 /**< An SDL wheel left event. */
	SDL_WHEEL_RIGHT,                /**< An SDL wheel right event. */
	SDL_WHEEL_UP,                   /**< An SDL wheel up event. */
	SDL_WHEEL_DOWN,                 /**< An SDL wheel down event. */
	SDL_KEY_DOWN,                   /**< An SDL key down event. */
	SDL_TEXT_INPUT,                 /**< An SDL text input (commit) event. */
	SDL_TEXT_EDITING,               /**< An SDL text editing (IME) event. */

	NOTIFY_REMOVAL,                 /**< Sent by a widget to notify others it's being destroyed. */
	NOTIFY_MODIFIED,                /**<
	                                 * Sent by a widget to notify others its contents or state are modified.
	                                 *
	                                 * What modified means is documented per widget. If not documented the request
	                                 * means nothing.
	                                 */
	REQUEST_PLACEMENT,              /**<
	                                 * Request to place the widget. This may also trigger an update of other
	                                 * layout components.
	                                 */

	RECEIVE_KEYBOARD_FOCUS,         /**< Widget gains keyboard focus. */
	LOSE_KEYBOARD_FOCUS,            /**< Widget loses keyboard focus. */

	SHOW_TOOLTIP,                   /**< Request the widget to show its hover tooltip. */
	NOTIFY_REMOVE_TOOLTIP,          /**< Request the widget to remove its hover tooltip. */
	MESSAGE_SHOW_TOOLTIP,           /**< Request to show a tooltip based on the data sent. */
	SHOW_HELPTIP,                   /**< Request the widget to show its hover helptip. */
	MESSAGE_SHOW_HELPTIP,           /**< Request to show a helptip based on the data sent. */

	SDL_TOUCH_MOTION,
	SDL_TOUCH_UP,
	SDL_TOUCH_DOWN,
	SDL_TOUCH_MULTI_GESTURE,

	SDL_RAW_EVENT					/**< Raw SDL event. */
};

/**
 * Connects a dispatcher to the event handler.
 *
 * @param dispatcher              The dispatcher to connect.
 */
void connect_dispatcher(dispatcher* dispatcher);

/**
 * Disconnects a dispatcher to the event handler.
 *
 * @param dispatcher              The dispatcher to disconnect.
 */
void disconnect_dispatcher(dispatcher* dispatcher);

/**
 * Gets all event dispatchers in the Z order.
 */
std::vector<dispatcher*>& get_all_dispatchers();

/**
 * Initializes the location of the mouse.
 *
 * After a layout of the window the mouse location needs to be updated to
 * test whether it entered or left a widget. Also after closing a window it's
 * needed to send a dummy mouse move.
 */
void init_mouse_location();

/**
 * Captures the mouse.
 *
 * A dispatcher can capture the mouse, when for example it's pressed on a
 * button, this means all mouse events after that are send to that widget.
 *
 * @param dispatcher              The dispatcher which should get the mouse
 *                                focus.
 */
void capture_mouse(dispatcher* dispatcher);

/**
 * Releases a captured mouse.
 *
 * @param dispatcher              The dispatcher which should release the mouse
 *                                capture.
 */
void release_mouse(dispatcher* dispatcher);

/**
 * Captures the keyboard.
 *
 * A dispatcher can capture the keyboard, when for example it's pressed on a
 * button, this means all keyboard events after that are send to that widget.
 *
 * @param dispatcher              The dispatcher which should get the keyboard
 *                                focus.
 */
void capture_keyboard(dispatcher* dispatcher);

std::ostream& operator<<(std::ostream& stream, const ui_event event);

} // namespace event

/**
 * Keeps track of any open windows of any type (modal, non-modal, or tooltip) in the
 * order in which they were opened. Currently only used as a helper for is_in_dialog(),
 * but could potentially be useful for other things in the future.
 */
extern std::vector<window*> open_window_stack;

/** Removes a entry from the open_window_stack list. This should be used instead of pop_back. */
void remove_from_window_stack(window* window);

/**
 * Is a dialog open?
 *
 * @note added as backwards compatibility for gui::is_in_dialog.
 */
bool is_in_dialog();

} // namespace gui2
