"""
To read a STL file and plot in mayavi
First created by Junwei Huang @ Toronto, Feb 26, 2013
"""

from numpy import *
from mayavi import mlab

STLfile="SP_PH02_Torso.obj"
f=open(STLfile,'r')

x=[]
y=[]
z=[]

for line in f:
    if( bool(line.strip()) == False ):
        continue
    else:
        strarray=line.split()
        if strarray[0]=='v':
            #print strarray
            x=append(x,double(strarray[1]))
            y=append(y,double(strarray[2]))
            z=append(z,double(strarray[3]))

triangles=[(i, i+1, i+2) for i in range(0, len(x),3)]
colors = 1.0 * (x + y)/(max(x)+max(y))    # Rainbow color

nodes = mlab.points3d(x, y, z, scale_factor=10.0)
nodes.glyph.scale_mode = 'scale_by_vector'
nodes.mlab_source.dataset.point_data.scalars = colors
mlab.show()
