@echo off

set tex=C:\texlive\2017\bin\win32\xelatex
set texargs=-interaction=nonstopmode -halt-on-error -output-directory=build

mkdir build\tex

%tex% %texargs% spectrum-analyzer.tex
%tex% %texargs% spectrum-analyzer.tex

xcopy /y build\spectrum-analyzer.pdf .

pause