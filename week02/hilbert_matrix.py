import numpy as np
import numpy.linalg as LA
from scipy.special import binom

N = 14
A = np.array([[1/(i+j+1) for i in range(N)] for j in range(N)])

np.set_printoptions(precision=4, formatter={'all': lambda x: '{:.4f}'.format(x)})

y = np.random.rand(N)
x0 = LA.solve(A, y)
y[4] += 1e-4
x1 = LA.solve(A, y)

print(np.abs(x1 - x0))

print(np.abs(A @ x1 - y))
