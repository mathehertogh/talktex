import subprocess as sp
import os

DEFAULT_LATEX_STRING = "\\documentclass{article} \\begin{document} Lorem \\end{document}"
class Compiler:
	def __init__(self, filepath="../latex/talktex-output/talktex.tex"):
		self.latex_dirname = os.path.dirname(filepath)
		self.latex_filename = os.path.basename(filepath)
		self.pdf_filename = os.path.splitext(os.path.basename(filepath))[0] + ".pdf"
		self.reader_process = None


	def compile(self, latex_string=DEFAULT_LATEX_STRING):
		latex_filepath = os.path.join(self.latex_dirname, self.latex_filename)
		pdf_filepath = os.path.join(self.latex_dirname, self.pdf_filename)

		# Write LaTeX string to LaTeX file
		f = open(latex_filepath, 'w+')
		f.write(latex_string)
		f.close()

		# Compile LaTeX file into PDF non-blockingly
		sp.Popen("latexmk -pdf -output-directory=" + self.latex_dirname + " " + latex_filepath,
		       shell=True,
		       stdout=sp.DEVNULL,
		       stderr=sp.DEVNULL)


	def initialize(self, latex_string=DEFAULT_LATEX_STRING):
		latex_filepath = os.path.join(self.latex_dirname, self.latex_filename)
		pdf_filepath = os.path.join(self.latex_dirname, self.pdf_filename)

		# Create output directory if needed
		if not os.path.exists(self.latex_dirname) and self.latex_dirname != '':
			os.makedirs(self.latex_dirname)

		# Write LaTeX string to LaTeX file
		f = open(latex_filepath, 'w+')
		f.write(latex_string)
		f.close()

		# Compile LaTeX file into PDF blockingly
		sp.run("latexmk -pdf -output-directory=" + self.latex_dirname + " " + latex_filepath,
		       shell=True,
		       stdout=sp.DEVNULL,
		       stderr=sp.DEVNULL)

		# Open PDF reader
		self.reader_process = sp.Popen("xdg-open " + pdf_filepath, shell=True)
