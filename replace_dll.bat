cd "C:\Games\Steam\steamapps\common\Sven Co-op\svencoop\addons\metamod\dlls"

if exist PlayerStatus_old.dll (
    del Emotes_old.dll
)
if exist Emotes.dll (
    rename Emotes.dll Emotes_old.dll 
)

exit /b 0