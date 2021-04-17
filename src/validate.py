import numpy as np
import pandas as pd
from scipy.sparse import coo_matrix

# d = pd.DataFrame()
# input = np.loadtxt("../data/ca-AstroPh.mtx", delimiter=" ", dtype=str)

with open("../data/ca-AstroPh.mtx") as f:

    line = f.readline()

    lines = (line for line in f if not line.startswith("%"))

    # lines = next(lines)
    # lines = next(lines)

    input = np.loadtxt(lines, delimiter=" ", dtype=str, skiprows=1)
    print(input)

    rows, cols = input.T
    map_rows = {val: ind for ind, val in enumerate(np.unique(rows))}
    map_cols = {val: ind for ind, val in enumerate(np.unique(cols))}

    print(len(map_rows))
    print(len(map_cols))
    # result = coo_matrix(([map_rows[x] for x in rows], [map_cols[x] for x in cols]))
    result = coo_matrix((np.ones(len(map_cols)), ([map_rows[x] for x in rows], [map_cols[x] for x in cols])))

# N = 1000
# a = np.zeros(shape=(N, N))


# print(a)
