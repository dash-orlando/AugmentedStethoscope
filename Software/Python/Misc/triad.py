"""

Traingle Solutions Plotting

Fluvio L Lobo Fenoglietto
"""


import numpy as np
from itertools import permutations
import matplotlib.pyplot as plt


mag_index = [1,2,3,4,5,6]
mag_x = [0, 100, 200, 0, 100, 200]
mag_y = [125, 175, 125, 0, -50, 0]

for comb in permutations(range(5), 3):
    print comb

plt.plot(mag_x,mag_y,'rs')
plt.show()
