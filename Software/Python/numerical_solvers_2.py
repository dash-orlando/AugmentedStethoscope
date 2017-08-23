"""
Numerical solvers:

Bisection, Secant & Newton Raphson Method.

AUTHOR  : Mohammad Odeh
DATE    : Aug. 22nd, 2017
"""
import  time
import  numpy           as      np
import  numpy.matlib    as      npmat
from    numpy.linalg    import  norm
from    numpy.linalg    import  inv
from    math            import  cos
from    math            import  sin

'''
*
* f		:	Function of interest
* f_		:	Derivative of function (if needed)
* [a, b]	:	Interval (if needed)
* TOL		:	Tolerance for convergence
* NMAX		:	Max number of iterations
*
'''

######################################################
#                   FUNCTION DEFINITIONS
######################################################

###
### NON-LINEAR SOLVERS
###
def func(i, x0):
    #x, y = x0
    k  = 15
    a  = 0.5
    T0 = 150
    T1 = x0[0]
    T2 = x0[1]
    T3 = x0[2]
    T4 = x0[3]
    TL = 25
    if( i == 0 ):
        #f = cos(x) - y
        f = (k+a/2*(T1+T0))*T0-(2*k+a/2*(T0+2*T1+T2))*T1+(k+a/2*(T1+T2))*T2
        return ( f )
    
    elif( i == 1):
        #f = x - sin(y)
        f = (k+a/2*(T2+T1))*T1-(2*k+a/2*(T1+2*T2+T3))*T2+(k+a/2*(T2+T3))*T3
        return ( f )
    
    elif( i == 2):
        f = (k+a/2*(T3+T2))*T2-(2*k+a/2*(T2+2*T3+T4))*T3+(k+a/2*(T3+T4))*T4
        return ( f )

    elif( i == 3):
        f = (k+a/2*(T4+T3))*T3-(2*k+a/2*(T3+2*T4+TL))*T4+(k+a/2*(T4+TL))*TL
        return ( f )
    
    else:
        raise Exception

# Approximate Jacobian using forward finite difference (FFD)
##def jacobian( i, j, x0, h=1e-5 ):
##    a = x0[:]                   # Create a local copy of the initial guess
##    a[j] = a[j] + h             # Add the finite difference to the corresponding variable
##
##    # Estimate derivative
##    df = (func(i, (a)) - func(i, (x0)))/(h)
##
##    # Return answer
##    return ( df )

def jacobian( i, j, x0, h=1e-5 ):
    x = []
    for k in range(len(x0)):
        x.append(x0.item(k))
    a = x[:]                    # Create a local copy of the initial guess
    a[j] = a[j] + h             # Add the finite difference to the corresponding variable

    # Estimate derivative
    df = (func(i, (a)) - func(i, (x)))/(h)

    # Return answer
    return ( df )
    
# Newton Raphson for a system of non-linear equations
##def NR_NL( x0, TOL=1e-5, NMAX=100 ):
##    """
##    INPUT : (function, derivative, initial guess, OPTIONAL)
##
##    RETURN: tuple(SOLUTION, RESIDUAL, ITERATIVE CONVERGENCE, ITERATIONS)
##    """
##    n=0                                 # Counter
##    res = []                            # Residual
##    iter_conv = []                      # Iterative convergance
##    N = len(x0)                         # Get vector length for matrix construction
##    J = npmat.zeros((N,N), dtype='f')   # Construct matrix of size (NxN)
##    print(type(J))
##    b = npmat.zeros((N,1), dtype='f')
##    while( n <= NMAX ):
##        for i in range( N ):
##            for j in range( N ):
##                J[i, j] = jacobian(i, j, x0)
##            b[i] = -func(i, x0)
##
##        z = J.I*b                       # Array
##        x0 = x0 + z                     # Update solution
##        res.append( norm(b) )           # Evaluate residual
##        iter_conv.append( norm(z) )     # Evaluate iterative convergence
##
##        if (abs(res[-1]) < TOL) and (iter_conv[-1] < TOL):
##            return (x0, res, iter_conv, n)
##
##        else:
##            n = n + 1                   # Increment counter
##
##    return(0, res, iter_conv, n)
##    #return False                        # No solution found within iteration limit

def NR_NL( x0, TOL=1e-5, NMAX=100 ):
    """
    INPUT : (function, derivative, initial guess, OPTIONAL)

    RETURN: tuple(SOLUTION, RESIDUAL, ITERATIVE CONVERGENCE, ITERATIONS)
    """
    n=0                                         # Counter
    res = []                                    # Residual
    iter_conv = []                              # Iterative convergance
    N = len(x0)                                 # Get vector length for matrix construction
    J = np.empty((N,N), dtype='float64')        # Construct matrix of size (NxN)
    b = np.empty((N,1), dtype='float64')
    while( n <= NMAX ):
        for i in range( N ):
            for j in range( N ):
                J[i, j] = jacobian(i, j, x0)
            b[i] = -func(i, x0)

        #time.sleep(0.5)
        #print("Jacobian = ")
        #print(J)
        #print("...")
        #time.sleep(0.5)
        #print("b = ")
        #print(b)
        #print("...")
        #time.sleep(0.5)
        
        z = inv(J).dot(b)               # Array
        #print("z =")
        #print(z)
        #print("...")
        
        x0 = x0 + z                     # Update solution
        res.append( norm(b) )           # Evaluate residual
        iter_conv.append( norm(z) )     # Evaluate iterative convergence

        if (abs(res[-1]) < TOL) and (iter_conv[-1] < TOL):
            return (x0, res, iter_conv, n)

        else:
            n = n + 1                   # Increment counter

    return(0, res, iter_conv, n)
    #return False                        # No solution found within iteration limit       


######################################################
#                   START PROGRAM
######################################################
if __name__ == "__main__":

    # Invoking Newton-Raphson for nonlinear system
    x0 = np.array(([100], [100], [100], [100]), dtype='float64')
    print( "Newton-Raphson Method - NON-LINEAR" )
    sln, res, conv, n = NR_NL(x0, NMAX=25, TOL=1e-10)
    print( "SOLUTION:" )
    print(sln)
    print( "Iterations: %3.3f" %( n) )
    print( "   Residual\t  ||   Convergence" )
    for i in range( len(res) ):
        print( "%2i: %1.2e\t  ||\t%1.2e" %(i, res[i], conv[i]) )
        if ( i == len(res) - 1):
            print('')


"""
References
1 - http://kestrel.nmt.edu/~raymond/software/python_notes/paper003.html
"""
