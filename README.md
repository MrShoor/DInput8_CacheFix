# DInput8_CacheFix

This is a fix for USB microphone stuttering and other USB issues occurring while playing different games.

A some games (like Crash Bandicoot, MK11, DOA6,detroitbecomehuman, okamihd, Wo Long etc.) tries to enumerate all DirectInput devices, i.e. search for new controllers, about 10 times a second which is both I/O intensive and completely unnecessary.

The original fix ( https://github.com/Quidrex/CrashFix ) just reports 0 input devices without actually doing any I/O.
But it can be a problem when games really need such devices.

The current fix returns real devices, but stores a list of all devices for 30 seconds in the local cache.

## Installation \#1 (global for whole system)

For installation this fix you need to rename real "C:\Windows\System32\dinput8.dll" into "C:\Windows\System32\\_dinput8.dll" 
and copy dinput8.dll from this releases into "C:\Windows\System32\dinput8.dll"

For renaming original "C:\Windows\System32\dinput8.dll" into "C:\Windows\System32\\_dinput8.dll" you may require to take ownership of the original dinput8.dll file
It can be done by this command (with Administration rights):
takeown /F "C:\Windows\System32\dinput8.dll"

## Uninstalling \#1

For uninstalling - just remove fake "C:\Windows\System32\dinput8.dll" and then rename "C:\Windows\System32\\_dinput8.dll" back into "C:\Windows\System32\dinput8.dll"

## Installation \#2 (for particular game only)

For installation of this fix you need to make a copy of the real "C:\Windows\System32\dinput8.dll" as "C:\Windows\System32\\_dinput8.dll" 
Then you need to put this new fixed dinput8.dll into the same directory as *.exe of a particular game

## Uninstalling \#2

Delete the copy of "C:\Windows\System32\\_dinput8.dll"
Delete fixed dll, what you placed in the game directory

## Remarks

The first installation mode of this fix is global for the whole system. Beware some anti-cheat programs can detect it as a modified DLL.
The second installation mode - can not cover the all of cases, and this fix may not work for some games.

Regardless used installation mode fix has a 30-sec cache. So if you change your gamepads/keyboard on the fly it can require up to 30 sec to update device information for the game. Or you can just restart the game.

## License

Copyright (c) 2018 Gian Perrone

Copyright (c) 2023 Alex Busarov

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
