@echo off




cl src/*.c ntdll.lib user32.lib /Fe:suspend.exe
del *.obj