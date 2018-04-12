@echo off

set tex=C:\texlive\2017\bin\win32\xelatex
set texargs=-interaction=nonstopmode -halt-on-error -output-directory=build

mkdir build

%tex% %texargs% journal.tex

xcopy /y build\journal.pdf .

pause