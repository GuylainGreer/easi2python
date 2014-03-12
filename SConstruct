import os

cflags = '-DBOOST_RESULT_OF_USE_DECLTYPE '
ccflags = '-std=c++11 '
if int(ARGUMENTS.get('debug', 1)):
    cflags += ' -g3 -ggdb -DDEBUG -DENABLE_SPIRIT_DEBUGGING '
    linkflags = ''
else:
    cflags += ' -O2 -DNDEBUG -flto '
    linkflags = ' -flto '

if ARGUMENTS.get('cl', 'clang') == 'gcc':
    cc = 'gcc-4.8'
    cpp = 'g++-4.8'
    cflags += ' -fopenmp -Wall -fvisibility=hidden -ftemplate-depth-10000'
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
