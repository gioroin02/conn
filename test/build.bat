@echo off

set "compiler=zig cc"

set "impl=%impl% src\pax\base\export.c"
set "impl=%impl% src\pax\base\memory\export.c"
set "impl=%impl% src\pax\structure\export.c"
set "impl=%impl% src\pax\system\memory\export.c"
set "impl=%impl% src\pax\system\network\export.c"
set "impl=%impl% src\pax\system\storage\export.c"
set "impl=%impl% src\pax\system\console\export.c"
set "impl=%impl% src\pax\system\async\export.c"
set "impl=%impl% src\pax\system\async\network\export.c"
set "impl=%impl% src\pax\system\async\storage\export.c"

set "impl=%impl% src\conn\export.c"

set "message=test\message.c"

%compiler% --std=c89 %impl% %message% -lws2_32 -o bin\message.exe
