//
// Copyright (c) 2008-2020 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "../Core/Object.h"
#include "../Input/InputConstants.h"


namespace Atomic
{

/// Mouse button pressed.
ATOMIC_EVENT(E_MOUSEBUTTONDOWN, MouseButtonDown)
{
    ATOMIC_PARAM(P_BUTTON, Button);                // int
    ATOMIC_PARAM(P_BUTTONS, Buttons);              // int
    ATOMIC_PARAM(P_QUALIFIERS, Qualifiers);        // int
    ATOMIC_PARAM(P_CLICKS, Clicks);                // int
}

/// Mouse button released.
ATOMIC_EVENT(E_MOUSEBUTTONUP, MouseButtonUp)
{
    ATOMIC_PARAM(P_BUTTON, Button);                // int
    ATOMIC_PARAM(P_BUTTONS, Buttons);              // int
    ATOMIC_PARAM(P_QUALIFIERS, Qualifiers);        // int
}

/// Mouse moved.
ATOMIC_EVENT(E_MOUSEMOVE, MouseMove)
{
    ATOMIC_PARAM(P_X, X);                          // int (only when mouse visible)
    ATOMIC_PARAM(P_Y, Y);                          // int (only when mouse visible)
    ATOMIC_PARAM(P_DX, DX);                        // int
    ATOMIC_PARAM(P_DY, DY);                        // int
    ATOMIC_PARAM(P_BUTTONS, Buttons);              // int
    ATOMIC_PARAM(P_QUALIFIERS, Qualifiers);        // int
}

/// Mouse wheel moved.
ATOMIC_EVENT(E_MOUSEWHEEL, MouseWheel)
{
    ATOMIC_PARAM(P_WHEEL, Wheel);                  // int
    ATOMIC_PARAM(P_BUTTONS, Buttons);              // int
    ATOMIC_PARAM(P_QUALIFIERS, Qualifiers);        // int
}

/// Key pressed.
ATOMIC_EVENT(E_KEYDOWN, KeyDown)
{
    ATOMIC_PARAM(P_KEY, Key);                      // int
    ATOMIC_PARAM(P_SCANCODE, Scancode);            // int
    ATOMIC_PARAM(P_BUTTONS, Buttons);              // int
    ATOMIC_PARAM(P_QUALIFIERS, Qualifiers);        // int
    ATOMIC_PARAM(P_REPEAT, Repeat);                // bool
}

/// Key released.
ATOMIC_EVENT(E_KEYUP, KeyUp)
{
    ATOMIC_PARAM(P_KEY, Key);                      // int
    ATOMIC_PARAM(P_SCANCODE, Scancode);            // int
    ATOMIC_PARAM(P_BUTTONS, Buttons);              // int
    ATOMIC_PARAM(P_QUALIFIERS, Qualifiers);        // int
}

/// Text input event.
ATOMIC_EVENT(E_TEXTINPUT, TextInput)
{
    ATOMIC_PARAM(P_TEXT, Text);                    // String
}

/// Text editing event.
ATOMIC_EVENT(E_TEXTEDITING, TextEditing)
{
    ATOMIC_PARAM(P_COMPOSITION, Composition);      // String
    ATOMIC_PARAM(P_CURSOR, Cursor);                // int
    ATOMIC_PARAM(P_SELECTION_LENGTH, SelectionLength);  // int
}

/// Joystick connected.
ATOMIC_EVENT(E_JOYSTICKCONNECTED, JoystickConnected)
{
    ATOMIC_PARAM(P_JOYSTICKID, JoystickID);        // int
}

/// Joystick disconnected.
ATOMIC_EVENT(E_JOYSTICKDISCONNECTED, JoystickDisconnected)
{
    ATOMIC_PARAM(P_JOYSTICKID, JoystickID);        // int
}

/// Joystick button pressed.
ATOMIC_EVENT(E_JOYSTICKBUTTONDOWN, JoystickButtonDown)
{
    ATOMIC_PARAM(P_JOYSTICKID, JoystickID);        // int
    ATOMIC_PARAM(P_BUTTON, Button);                // int
}

/// Joystick button released.
ATOMIC_EVENT(E_JOYSTICKBUTTONUP, JoystickButtonUp)
{
    ATOMIC_PARAM(P_JOYSTICKID, JoystickID);        // int
    ATOMIC_PARAM(P_BUTTON, Button);                // int
}

/// Joystick axis moved.
ATOMIC_EVENT(E_JOYSTICKAXISMOVE, JoystickAxisMove)
{
    ATOMIC_PARAM(P_JOYSTICKID, JoystickID);        // int
    ATOMIC_PARAM(P_AXIS, Button);                  // int
    ATOMIC_PARAM(P_POSITION, Position);            // float
}

/// Joystick POV hat moved.
ATOMIC_EVENT(E_JOYSTICKHATMOVE, JoystickHatMove)
{
    ATOMIC_PARAM(P_JOYSTICKID, JoystickID);        // int
    ATOMIC_PARAM(P_HAT, Button);                   // int
    ATOMIC_PARAM(P_POSITION, Position);            // int
}

/// Finger pressed on the screen.
ATOMIC_EVENT(E_TOUCHBEGIN, TouchBegin)
{
    ATOMIC_PARAM(P_TOUCHID, TouchID);              // int
    ATOMIC_PARAM(P_X, X);                          // int
    ATOMIC_PARAM(P_Y, Y);                          // int
    ATOMIC_PARAM(P_PRESSURE, Pressure);            // float
}

/// Finger released from the screen.
ATOMIC_EVENT(E_TOUCHEND, TouchEnd)
{
    ATOMIC_PARAM(P_TOUCHID, TouchID);              // int
    ATOMIC_PARAM(P_X, X);                          // int
    ATOMIC_PARAM(P_Y, Y);                          // int
}

/// Finger moved on the screen.
ATOMIC_EVENT(E_TOUCHMOVE, TouchMove)
{
    ATOMIC_PARAM(P_TOUCHID, TouchID);              // int
    ATOMIC_PARAM(P_X, X);                          // int
    ATOMIC_PARAM(P_Y, Y);                          // int
    ATOMIC_PARAM(P_DX, DX);                        // int
    ATOMIC_PARAM(P_DY, DY);                        // int
    ATOMIC_PARAM(P_PRESSURE, Pressure);            // float
}

/// A touch gesture finished recording.
ATOMIC_EVENT(E_GESTURERECORDED, GestureRecorded)
{
    ATOMIC_PARAM(P_GESTUREID, GestureID);          // unsigned
}

/// A recognized touch gesture was input by the user.
ATOMIC_EVENT(E_GESTUREINPUT, GestureInput)
{
    ATOMIC_PARAM(P_GESTUREID, GestureID);          // unsigned
    ATOMIC_PARAM(P_CENTERX, CenterX);              // int
    ATOMIC_PARAM(P_CENTERY, CenterY);              // int
    ATOMIC_PARAM(P_NUMFINGERS, NumFingers);        // int
    ATOMIC_PARAM(P_ERROR, Error);                  // float
}

/// Pinch/rotate multi-finger touch gesture motion update.
ATOMIC_EVENT(E_MULTIGESTURE, MultiGesture)
{
    ATOMIC_PARAM(P_CENTERX, CenterX);              // int
    ATOMIC_PARAM(P_CENTERY, CenterY);              // int
    ATOMIC_PARAM(P_NUMFINGERS, NumFingers);        // int
    ATOMIC_PARAM(P_DTHETA, DTheta);                // float (degrees)
    ATOMIC_PARAM(P_DDIST, DDist);                  // float
}

/// A file was drag-dropped into the application window.
ATOMIC_EVENT(E_DROPFILE, DropFile)
{
    ATOMIC_PARAM(P_FILENAME, FileName);            // String
}

/// Application input focus or minimization changed.
ATOMIC_EVENT(E_INPUTFOCUS, InputFocus)
{
    ATOMIC_PARAM(P_FOCUS, Focus);                  // bool
    ATOMIC_PARAM(P_MINIMIZED, Minimized);          // bool
}

/// OS mouse cursor visibility changed.
ATOMIC_EVENT(E_MOUSEVISIBLECHANGED, MouseVisibleChanged)
{
    ATOMIC_PARAM(P_VISIBLE, Visible);              // bool
}

/// Mouse mode changed.
ATOMIC_EVENT(E_MOUSEMODECHANGED, MouseModeChanged)
{
    ATOMIC_PARAM(P_MODE, Mode);                    // MouseMode
    ATOMIC_PARAM(P_MOUSELOCKED, MouseLocked);      // bool
}

/// Application exit requested.
ATOMIC_EVENT(E_EXITREQUESTED, ExitRequested)
{
}

/// Raw SDL input event.
ATOMIC_EVENT(E_SDLRAWINPUT, SDLRawInput)
{
    ATOMIC_PARAM(P_SDLEVENT, SDLEvent);           // SDL_Event*
    ATOMIC_PARAM(P_CONSUMED, Consumed);           // bool
}

/// Input handling begins.
ATOMIC_EVENT(E_INPUTBEGIN, InputBegin)
{
}

/// Input handling ends.
ATOMIC_EVENT(E_INPUTEND, InputEnd)
{
}

// ATOMIC BEGIN

/// Application pause requested.
ATOMIC_EVENT(E_PAUSERESUMEREQUESTED, PauseResumeRequested)
{
}

/// Application step frame while paused requested.
ATOMIC_EVENT(E_PAUSESTEPREQUESTED, PauseStepRequested)
{
}

// ATOMIC END

}
