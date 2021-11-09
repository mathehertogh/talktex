inc = include_directories('src/public')

libgrammar_files = []
subdir('src/') # This adds all source files

libgrammar = library('grammar', libgrammar_files, include_directories : inc, install : true)
libgrammar_dep = declare_dependency(include_directories : inc, link_with : libgrammar)

libgrammar_exe = executable(
	'compiler_grammar', 
	'src/cpp/entrypoint/main.cpp', 
	dependencies: libgrammar_dep,
	include_directories: inc,
	install: true
)