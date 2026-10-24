import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import scipy.special as spc

data = pd.read_csv("data/gl_test.csv")
t = data["t"]
plt.plot(t, data["y0"])
theory = np.exp(-t)
plt.plot(t, theory, linestyle="--")
plt.show()