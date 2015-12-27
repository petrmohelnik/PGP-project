@echo off
set name=project

pdflatex %name%.tex
bibtex %name%.aux
pdflatex %name%.tex
pdflatex %name%.tex
%name%.pdf