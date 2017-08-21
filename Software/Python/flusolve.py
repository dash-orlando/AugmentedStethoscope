"""
Solvers
"""

# Import Modules
import  random
import  numpy                   as      np
from    time                    import  sleep, time
from    math                    import  *
from    scipy.optimize          import  fsolve
from    scipy.optimize          import  broyden1
from    scipy.optimize          import  newton_krylov
from    scipy.optimize          import  root
from    scipy.linalg            import  norm
#from    sympy                   import  nsolve
#from    usbProtocol             import  createUSBPort
#from    sympy.core.symbol       import symbols
#from    sympy.solvers.solveset   import nonlinsolve

# Construct equations to solve for:
def LHS( root ):
    x, y, z = root
    H1_L2, H2_L2, H3_L2 = HNorm
    sensor1 = ( K*( x**2. + y**2. + z**2. )**(-3.) *
              ( 3.*( z**2./(x**2. + y**2. + z**2.) ) + 1 ))
    
    sensor2 = ( K*( (x+.05)**2. + (y-.05)**2. + z**2. )**(-3.) *
              ( 3.*( z**2./((x+.05)**2. + (y-.05)**2. + z**2.) ) + 1 ))
    
    sensor3 = ( K*( (x-.05)**2. + (y-.05)**2. + z**2. )**(-3.) *
              ( 3.*( z**2./((x-.05)**2. + (y-.05)**2. + z**2.) ) + 1 ))
    
    return ( sensor1 - H1_L2**2, sensor2 - H2_L2**2, sensor3 - H3_L2**2 )


# Define constants


# Solve function
sol = root(LHS1, initialGuess, method='lm', options={'maxiter':250000})
print( sol ) 
print( "SOLUTION:" )
print( "x = %.5f || y = %.5f || z = %.5f" %(sol.x[0], sol.x[1], sol.x[2]) )
solution = np.array((sol.x[0], sol.x[1], sol.x[2]), dtype='f')
print( LHS1(solution) )
sleep( .5 )

