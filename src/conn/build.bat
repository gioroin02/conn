@echo off

set "compiler=zig cc --std=c99"

set "base=src\rn\base\export.c"
set "base_memory=src\rn\base\memory\export.c"
set "structure=src\rn\structure\export.c"
set "system_memory=src\rn\system\memory\export.c"
set "system_network=src\rn\system\network\export.c"
set "system_asyncio=src\rn\system\asyncio\export.c"

set "conn=src\conn\export.c"

set "server=src\conn\server.c"
set "client=src\conn\client.c"

%compiler% %base% %base_memory% %structure% %system_memory% %system_network% ^
    %system_asyncio% %conn% %server% -lws2_32 -o bin\server.exe

%compiler% %base% %base_memory% %structure% %system_memory% %system_network% ^
    %system_asyncio% %conn% %client% -lws2_32 -o bin\client.exe
