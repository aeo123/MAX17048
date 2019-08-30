from building import *

cwd     = GetCurrentDir()
CPPPATH = [cwd, str(Dir('#'))]

src     = Glob('*.c')


group = DefineGroup('MAX17048', src, depend = ['PKG_USING_MAX17048'], CPPPATH = CPPPATH)

Return('group')

