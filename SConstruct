# shamelessly stolen from and adjusted for my purposes:
# https://github.com/dblalock/scons-example/blob/master/test/sconscript

# Set our required libraries
libraries 		= []
library_paths 	= ''
cppDefines 		= {}
debugFlags 		= ['-g']
cppFlags 		= '-O3 -fdiagnostics-color=always -Wall -Wformat -Werror -Wfatal-errors'.split(' ')
cxxFlags 		= []
linkerFlags     = '-lfmt -lspdlog -lconfig++ -lsfml-graphics -lsfml-window -lsfml-system'.split(' ')

AddOption('--build', dest='build', type='string', nargs=1, action='store', help="activate build configurations")
AddOption('--tests-only', dest='tests_only', nargs=0, help="build test binary only")
AddOption('--no-tests', dest='no_tests', nargs=0, help="build src binary only")

# define the attributes of the build environment shared between
# both the debug and release builds
common_env = Environment()
common_env.Append(LIBS 			= libraries)
common_env.Append(LIBPATH 		= library_paths)
common_env.Append(CPPDEFINES 	= cppDefines)
common_env.Append(CPPFLAGS 		= cppFlags)
common_env.Append(CXXFLAGS 		= cxxFlags)
common_env.Append(LINKFLAGS		= linkerFlags)

# uncomment to force g++ for c code also--creates warnings but
# avoids need for extern "C"
common_env['CC'] = 'g++'

build_configs = {}

options_to_build=[]
if GetOption('build') is None:
	options_to_build = ['debug', 'release']
if GetOption('build') == 'debug':
	options_to_build = ['debug']
if GetOption('build') == 'release':
	options_to_build = ['release']

if 'release' in options_to_build:
	print("adding release config")
	# Our release build is derived from the common build environment...
	release_env = common_env.Clone()
	release_env.Append(CPPDEFINES=['RELEASE'])
	release_env.VariantDir('build/release/src', 'src', duplicate=0)
	release_env.VariantDir('build/release/test', 'test', duplicate=0)
	build_configs['release'] = release_env

if 'debug' in options_to_build:
	print("adding debug config")
	# We define our debug build environment in a similar fashion...
	debug_env = common_env.Clone()
	debug_env.Append(CPPDEFINES=['DEBUG'])
	common_env.Append(CPPFLAGS 		= debugFlags)
	debug_env.VariantDir('build/debug/src', 'src', duplicate=0)
	debug_env.VariantDir('build/debug/test', 'test', duplicate=0)
	build_configs['debug'] = debug_env

# Now that all build environment have been defined, let's iterate over
# them and invoke the lower level SConscript files.
for mode, env in build_configs.items():
	modeDir = 'build/%s' % mode
	# print(dict(tests_only=GetOption('tests_only'), no_tests=GetOption('no_tests')))
	
	# tests_only	|	no_tests	|	result
	#	1			|	1			|	-
	#	1			|	0			|	tests
	#	0			|	1			|	src
	#	0			|	0			|	tests / src
	build_tests = GetOption("no_tests") != ()
	build_srcs = GetOption("tests_only") != ()
	print(dict(build_tests=build_tests, build_srcs=build_srcs))

	if build_tests:
		print("calling test SConscript")
		env.SConscript('%s/test/SConscript' % modeDir, {'env': env})
	if build_srcs:
		print("calling src SConscript")
		env.SConscript('%s/src/SConscript' % modeDir, {'env': env})