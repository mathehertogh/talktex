inc = include_directories('src/public')

libgeneral_files = []
subdir('src') # This adds all source files

libgeneral = library('general', libgeneral_files, include_directories : inc, install : true)
libgeneral_dep = declare_dependency(include_directories : inc, link_with : libgeneral)