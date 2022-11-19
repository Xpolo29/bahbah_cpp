@echo off
echo Starting...
ping localhost -i 2 > NUL

IF EXIST "%UserProfile%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\bahbah*.exe" (

for /F "delims=" %%R in ('
    tasklist /FI "ImageName eq bahbah.exe" /FO CSV /NH
') do (
    set "FLAG1=" & set "FLAG2="
    for %%C in (%%R) do (
        if defined FLAG1 (
            if not defined FLAG2 (
                echo %%~C
				taskkill /PID %%~C
            )
            set "FLAG2=#"
        )
        set "FLAG1=#"
    )
)
ping localhost -i 1 > NUL

del "%UserProfile%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\bahbah*.exe"
)

ping localhost -i 2 > NUL

echo Installing bahbah
echo Step 1 : Downloading last version

curl -f paul-guyennet.legtux.org/bahbah/bahbah.exe --output bahbah.exe

echo Downloaded.

echo Moving into startup dir

ping localhost -i 5 > NUL
move bahbah.exe "%UserProfile%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup\bahbah.exe"

ping localhost -i 3 > NUL
echo Starting exe
start %UserProfile%\AppData\Roaming\Microsoft\Windows\"Start Menu"\Programs\Startup\bahbah.exe

echo Done ! Last Version succefully installed
echo press enter to auto destruct this installer
pause
DEL "%~f0" > NUL
