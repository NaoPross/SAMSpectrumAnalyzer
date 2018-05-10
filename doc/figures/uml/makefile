all: desktop-sequence.pdf

%.pdf: %.txt
	plantuml $< -tpdf $@

%.latex: %.txt
	plantuml $< -tlatex:nopreamble $@
