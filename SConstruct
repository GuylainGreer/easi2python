import os

cflags = '-DBOOST_RESULT_OF_USE_DECLTYPE '
ccflags = '-std=c++11 '
debug = int(ARGUMENTS.get('debug', 1))
if debug:
    cflags += ' -g3 -ggdb -DDEBUG -DENABLE_SPIRIT_DEBUGGING '
    linkflags = ''
else:
    cflags += ' -O3 -DNDEBUG '
    linkflags = ''

if ARGUMENTS.get('cl', 'gcc') == 'gcc':
    cc = 'gcc'
    cpp = 'g++'
    cflags += ' -fopenmp -Wall -fvisibility=hidden -ftemplate-depth-10000 '
    if not debug:
        cflags += ' -flto '
        linkflags = ' -flto '
else:
    cc = 'clang'
    cpp = 'clang++'

env = Environment(CCFLAGS = cflags, CXXFLAGS = ccflags, CC = cc, CXX = cpp,
                  LINKFLAGS = linkflags, 
                  ENV = {'PATH' : os.environ['PATH'],
                         'TERM' : os.environ['TERM'],
                         'HOME' : os.environ['HOME']})

def GetSourceFiles(dir):
    list = []
    for root, dirs, files in os.walk(dir):
        list += Glob(root + '/*.cpp')
        list += Glob(root + '/*.c')
    return list

env.SetOption('num_jobs', 8) 
env.Program(target = 'easi', source = GetSourceFiles('.'),
            LIBS = 'gomp', CPPPATH='.')
