libgrammar_depends = [libgeneral_dep, libgrammar_dep]
inc = include_directories('src/public')

libgrammar_files = []
subdir('src/') # This adds all source files

libgrammar = library('latex-generator', libgrammar_files, include_directories : inc, dependencies : libgrammar_depends, install : true)
libgrammar_dep = declare_dependency(include_directories : inc, link_with : libgrammar, dependencies : libgrammar_depends)

libgrammar_exe = executable(
	'compiler_latex_generator', 
	'src/cpp/entrypoint/main.cpp', 
	dependencies: libgrammar_depends + [tclap.get_variable('tclap_dep'), libgrammar_dep],
	include_directories: inc,
	install: true
)

shared_library(
	'compiler_latex_generator', 
	'src/cpp/entrypoint/main.cpp', 
	dependencies: libgrammar_depends + [tclap.get_variable('tclap_dep'), libgrammar_dep],
	include_directories: inc,
	install: true
)
