from building import *

cwd     = GetCurrentDir()
CPPPATH = [cwd, str(Dir('#'))]

src     = Glob('*.c')


group = DefineGroup('MAX17048', src, depend = [''], CPPPATH = CPPPATH)

Return('group')

