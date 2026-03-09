import numpy as np
import numpy.linalg as LA
from scipy.special import binom

N = 14
Y = np.array([[1/(i+j+1) for i in range(N)] for j in range(N)])

np.set_printoptions(precision=4, formatter={'all': lambda x: '{:.4f}'.format(x)})

print(Y @ LA.inv(Y))
