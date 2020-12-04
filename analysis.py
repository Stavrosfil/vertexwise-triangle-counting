# %%
import re
from datetime import time

import matplotlib.pyplot as plt
import matplotlib.ticker as ticker
import numpy as np
import pandas as pd
import seaborn as sns

sns.set_theme(style="whitegrid")

df = pd.read_csv(
    "./results/results_hpc.csv",
)

for i in df.index:
    df.loc[i, "dataset"] = df.loc[i, "dataset"].split("/")[-1].split(".")[0]

df["speedup"] = 1
print(df)


# %%
v3 = df[df["version"] == "v3"]
# v3 = df[df["version"] == "v3"].loc[:, ["dataset", "library", "threads", "time", "speedup"]]
v4 = df[df["version"] == "v4"]
# v4 = df[df["version"] == "v4"].loc[:, ["dataset", "library", "threads", "time", "speedup"]]

datasets = df.dataset.unique()
libraries = df.library.unique()
threads = df.threads.unique()

for dataset_name in datasets:
    d = v3[v3.dataset == dataset_name]
    baseline_time = d[d.threads == 1].time.mean()
    print(f"Baseline time: {baseline_time}")

    for i in d.index:
        if v3.loc[i].threads != 1:
            v3.loc[i, "speedup"] = baseline_time / d.loc[i].time
            # print(d.loc[i].speedup)

with sns.axes_style("whitegrid"):
    fig, ax = plt.subplots(figsize=(20, 10))
    ax.set(
        xlabel="Threads (n)",
        ylabel="Speedup (x)",
        title="Speedup",
    )

    ax.set_xticks(threads)
    # ax.set_xlim((min(threads), max(threads)))
    # ax.set_xticks(np.arange(0, max(threads) + 1, 1))
    # ax.xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, pos: x if x in threads else None))

    sns.lineplot(
        data=v3,
        x="threads",
        y="speedup",
        hue="dataset",
        style="dataset",
        markers=True,
        markersize=10,
        linewidth=2.5,
        ci=25,
        # err_style="bars",
        # marker="o",
    )
    # fig.legend(
    #     labels=[
    #         "test",
    #     ]
    # )
