@echo off

set "compiler=zig cc"

set "impl=%impl% src\pax\base\export.c"
set "impl=%impl% src\pax\structure\export.c"
set "impl=%impl% src\pax\system\memory\export.c"
set "impl=%impl% src\pax\system\network\export.c"
set "impl=%impl% src\pax\system\async\export.c"
set "impl=%impl% src\pax\system\async\network\export.c"

set "impl=%impl% src\conn\export.c"

set "server=src\conn\server.c src\conn\main_server.c"
set "client=src\conn\client.c src\conn\main_client.c"

%compiler% --std=c89 %impl% %server% -lws2_32 -o bin\server.exe
%compiler% --std=c89 %impl% %client% -lws2_32 -o bin\client.exe
