all: desktop-sequence.pdf desktop-classes.pdf embedded-flowchart.pdf

%.pdf: %.txt
	plantuml $< -pdf $@

%.latex: %.txt
	plantuml $< -tlatex:nopreamble $@
