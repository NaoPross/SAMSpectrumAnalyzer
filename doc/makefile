BUILD_DIR := build
TEX := xelatex \
		-interaction=nonstopmode \
		-halt-on-error \
		-output-directory=$(BUILD_DIR)

.PHONY: dir doc diario presentazione
all: doc
	# cp $(BUILD_DIR)/diario.pdf .
	cp $(BUILD_DIR)/spectrum-analyzer.pdf .
	# cp $(BUILD_DIR)/presentazione.pdf .

doc: dir
	$(TEX) spectrum-analyzer.tex

diario: dir
	$(TEX) diario.tex

presentazione:
	$(TEX) presentazione.tex

dir:
	mkdir -p build/tex
