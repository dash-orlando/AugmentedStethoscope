from __future__ import absolute_import, division, print_function
from mayavi import mlab
import numpy as np
import math

K = 101     # Number of lines to plot
N = 5       # Subdivisions (aka K/N lines are drawn)

# Draw horizontal lines on the xy-, yz-, and xz-planes
lvlCurve_H = np.arange( 0, K, N )                       # Horizontal level curve
lvlCurve_V = np.zeros_like( lvlCurve_H )                # Vertical level curve
H, V = np.meshgrid( lvlCurve_H, lvlCurve_V )            # Mesh both arrays to form a matrix (a grid)
lvlCurve_0 = np.zeros_like( H )                         # Force everything into a 2D plane by setting the 3rd plane to 0
##H, V = H.flatten(), V.flatten()                         # Flatten matrices
##lvlCurve_0 = np.zeros_like( H )                         # Force everything into a 2D plane by setting the 3rd plane to 0

for i in range (0, K, N):
    mlab.mesh( H, V+i, lvlCurve_0, representation='mesh',
               tube_radius = 0.25, color=(1., 1., 1.) )
    mlab.mesh( lvlCurve_0, H, V+i, representation='mesh',
               tube_radius = 0.25, color=(1., 1., 1.) )
    mlab.mesh( H, lvlCurve_0, V+i, representation='mesh',
               tube_radius = 0.25, color=(1., 1., 1.) )
    
##    mlab.plot3d( H, V+i, lvlCurve_0 )                   # xy-plane
##    mlab.plot3d( lvlCurve_0, H, V+i )                   # yz-plane
##    mlab.plot3d( H, lvlCurve_0, V+i )                   # xz-plane

# Draw vertical lines on the xy-, yz-, and xz-planes
lvlCurve_V = np.arange(0, K, N)                         # Vertical level curve
lvlCurve_H = np.zeros_like(lvlCurve_V)                  # Horizontal level curve
H, V = np.meshgrid( lvlCurve_H, lvlCurve_V )            # Mesh both arrays to form a matrix (a grid)
lvlCurve_0 = np.zeros_like( H )                         # Force everything into a 2D plane by setting the 3rd plane to 0
##H, V = H.flatten(), V.flatten()                         # Flatten matrices
##lvlCurve_0 = np.zeros_like( H )                         # Force everything into a 2D plane by setting the 3rd plane to 0

for i in range (0, K, N):
    mlab.mesh( H+i, V, lvlCurve_0, representation='mesh',
               tube_radius = 0.25, color=(1., 1., 1.) )
    mlab.mesh( lvlCurve_0, H+i, V, representation='mesh',
               tube_radius = 0.25, color=(1., 1., 1.) )
    mlab.mesh( H+i, lvlCurve_0, V, representation='mesh',
               tube_radius = 0.25, color=(1., 1., 1.) )
    
##    mlab.plot3d( H+i, V, lvlCurve_0 )                   # xy-plane
##    mlab.plot3d( lvlCurve_0, H+i, V )                   # yz-plane
##    mlab.plot3d( H+i, lvlCurve_0, V )                   # xz-plane
    

# Setup cartesian space
mlab.outline( extent=[0, K-1, 0, K-1, 0, K-1] )
mlab.axes( extent=[0, K-1, 0, K-1, 0, K-1],
           line_width = 1.0,
           x_axis_visibility=True,
           y_axis_visibility=True,
           z_axis_visibility=True )

mlab.show()
