# Documents
DOCS := spectrum-analyzer.pdf journal.pdf presentation.pdf

# LaTeX compiler settings
BUILD_DIR := build
TEX := xelatex \
		-8bit \
		-shell-escape \
		-interaction=nonstopmode \
		-halt-on-error \
		-output-directory=$(BUILD_DIR)
		
# Recipes
.PHONY: uml dir count
all: $(DOCS)

%.pdf: %.tex dir
	$(TEX) $<
	cp $(BUILD_DIR)/$@ .

uml: dir
	$(MAKE) -C figures/uml/

dir:
	mkdir -p $(BUILD_DIR)/tex $(BUILD_DIR)/tikz

count:
	texcount -inc -total -letters spectrum-analyzer.tex
