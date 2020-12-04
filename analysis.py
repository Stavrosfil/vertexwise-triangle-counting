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

versions = [v3, v4]

# print(libraries)

for v in versions:
    datasets = v.dataset.unique()
    libraries = v.library.unique()
    threads = v.threads.unique()
    version = v.version.unique()[0]

    for library in libraries:
        for dataset_name in datasets:
            d = v[(v.dataset == dataset_name) & (v.library == library)]

            baseline_time = d[d.threads == 1].time.mean()
            # print(f"Baseline time: {baseline_time}")

            for i in d.index:
                if v.loc[i].threads != 1:
                    v.loc[i, "speedup"] = baseline_time / d.loc[i].time

        with sns.axes_style("whitegrid"):
            fig, ax = plt.subplots(figsize=(20, 10))
            ax.set(
                xlabel="Threads (n)",
                ylabel="Speedup (x)",
                title=f"{version} using: {library}",
            )

            ax.set_xticks(threads)
            # ax.set_xlim((min(threads), max(threads)))
            # ax.set_xticks(np.arange(0, max(threads) + 1, 1))
            # ax.xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, pos: x if x in threads else None))

            sns.lineplot(
                data=v[v.library == library],
                x="threads",
                y="speedup",
                hue="dataset",
                style="dataset",
                markers=True,
                markersize=10,
                linewidth=2.5,
                # ci=25,
                # err_style="bars",
                # marker="o",
            )
            fig.savefig(f"./results/figures/{version}_{library}.png")
            # fig.legend(
            #     labels=[
            #         "test",
            #     ]
            # )
