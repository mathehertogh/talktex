import os
import subprocess as sp
from shutil import copyfile

OUTPUT_FILENAME_ROOT = "output"
DEFAULT_LATEX_STRING = "\\documentclass{article} \\begin{document} Lorem \\end{document}"
TALKTEX_PACKAGE_FILENAME = "talktex.sty"
TALKTEX_PACKAGE_RELATIVE_DIR_PATH = "../../latex"

class Compiler:
	def __init__(self, script_dir, output_dir_path):
		self.output_dir_path = output_dir_path
		self.output_tex_path = os.path.join(output_dir_path, OUTPUT_FILENAME_ROOT + ".tex")
		self.output_pdf_path = os.path.join(output_dir_path, OUTPUT_FILENAME_ROOT + ".pdf")
		self.output_talktex_package_path = os.path.join(output_dir_path, TALKTEX_PACKAGE_FILENAME)
		self.talktex_package_path = os.path.join(
			script_dir, TALKTEX_PACKAGE_RELATIVE_DIR_PATH, TALKTEX_PACKAGE_FILENAME)

		self.reader_process = None


	def compile(self, latex_string=DEFAULT_LATEX_STRING):
		# Write LaTeX string to LaTeX file
		f = open(self.output_tex_path, 'w+')
		f.write(latex_string)
		f.close()

		# Compile LaTeX file into PDF non-blockingly
		sp.Popen("latexmk -pdf -output-directory=" + self.output_dir_path + " " + self.output_tex_path,
		         shell=True,
		         stdout=sp.DEVNULL,
		         stderr=sp.DEVNULL)


	def initialize(self, latex_string=DEFAULT_LATEX_STRING):
		# Create output directory if needed
		os.makedirs(self.output_dir_path, exist_ok=True)

		# Copy talktex.sty to output directory
		copyfile(self.talktex_package_path, self.output_talktex_package_path)

		# Write LaTeX string to LaTeX file
		f = open(self.output_tex_path, 'w+')
		f.write(latex_string)
		f.close()

		# Compile LaTeX file into PDF blockingly
		sp.run("latexmk -pdf -output-directory=" + self.output_dir_path + " " + self.output_tex_path,
		       shell=True,
		       stdout=sp.DEVNULL,
		       stderr=sp.DEVNULL)

		print("pdf path is " + self.output_pdf_path)

		# Open PDF reader
		self.reader_process = sp.Popen("xdg-open " + self.output_pdf_path, shell=True)
