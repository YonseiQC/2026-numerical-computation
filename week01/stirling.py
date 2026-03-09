import numpy as np
import math
from scipy.special import factorial

n = np.array(range(1,11))
S_n = np.sqrt(2*math.pi*n) * np.power(n / math.e, n)
fact_n = factorial(n)

abs_err = np.abs(S_n - fact_n)
rel_err = abs_err / fact_n

header = f"{'n':>4} {'S_n':>15} {'fact_n':>15} {'abs_err':>12} {'rel_err':>12}"
sep = "-" * len(header)
print(sep)
print(header)
print(sep)
for i in range(len(n)):
    print(f"{n[i]:>4} {S_n[i]:>15.4e} {fact_n[i]:>15.4e} {abs_err[i]:>12.4e} {rel_err[i]:>12.6f}")
print(sep)
