project('talktex', 'cpp', version : '1.0', default_options : ['warning_level=3', 'c_std=c11', 'cpp_std=c++17'])

build_system = build_machine.system() # Name of the system we build on.

tclap = subproject('tclap-1.4.0-rc1_vendored')

# Generators
flex = find_program('flex', required: true)
flex_gen = generator(
    flex,
    output: ['flex.@BASENAME@.cpp', 'flex.@BASENAME@.h'],
    arguments: [ '--outfile=@OUTPUT0@', '--header-file=@OUTPUT1@', '@INPUT@'])


bison = find_program('bison', required: true)

# check bison version
version_checker = find_program('bison/version_extractor.sh', required: true)
r = run_command(version_checker, check: true)
version = r.stdout().strip()
# -Wcounterexamples available on version 3.7.6
if version.version_compare('>=3.7.6')
    args = [ '-Wall', '-v', '-Wcounterexamples', '--output=@OUTPUT0@', '--defines=@OUTPUT1@', '@INPUT@' ]
else
    args =  [ '-Wall', '-v', '--output=@OUTPUT0@', '--defines=@OUTPUT1@', '@INPUT@' ]
endif

bison_gen = generator(
    bison,
    output: ['bison.@BASENAME@.cpp', 'bison.@BASENAME@.h'],
    arguments: args)

# Variables
## Global variables
builddir_name = get_option('with-builddir')
builddir_rpath_own = join_paths('$ORIGIN', '..', '..')

## Subproject variables
builddir = join_paths(meson.source_root(), builddir_name)

# binaries
subdir('src/general')
subdir('src/grammar')
subdir('src/latex-generator')
