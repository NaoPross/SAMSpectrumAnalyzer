all: desktop-sequence.pdf desktop-classes.pdf

%.pdf: %.txt
	plantuml $< -pdf $@

%.latex: %.txt
	plantuml $< -tlatex:nopreamble $@
