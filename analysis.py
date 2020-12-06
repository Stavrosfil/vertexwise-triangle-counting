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

import matplotlib as mpl

mpl.rcParams["figure.dpi"] = 300


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
            fig, ax = plt.subplots(figsize=(12, 6))
            ax.set(
                xlabel="Threads (n)",
                ylabel="Speedup (x)",
                title=f"{version} using: {library}",
            )

            ax.set_xticks(threads)
            ax.set_ylim(0, 16)

            # ax.set_xlim((min(threads), max(threads)))
            # ax.set_xticks(np.arange(0, max(threads) + 1, 1))
            # ax.xaxis.set_major_formatter(ticker.FuncFormatter(lambda x, pos: x if x in threads else None))

            sns.lineplot(
                data=v[v.library == library],
                x="threads",
                y="speedup",
                hue="dataset",
                style="dataset",
                palette="Set2",
                markers=True,
                markersize=10,
                linewidth=2.5,
                # ci=95,
                # err_style="bars",
                # marker="o",
            )
            # ax2 = ax.twinx()
            # # ax2.plot(100 * np.random.rand(10))
            # sns.lineplot(
            #     data=v[v.library == library],
            #     x="threads",
            #     y="time",
            #     hue="dataset",
            #     style="dataset",
            #     palette="Set2",
            #     markers=True,
            #     markersize=10,
            #     linewidth=2.5,
            #     ci=95,
            #     # err_style="bars",
            #     # marker="o",
            # )

            fig.savefig(f"./results/figures/{version}_{library}.pdf")
            # fig.legend(
            #     labels=[
            #         "test",
            #     ]
            # )

    # with sns.axes_style("whitegrid"):
    #     fig, ax = plt.subplots(figsize=(12, 6))
    #     ax.set(
    #         xlabel="Threads (n)",
    #         ylabel="Speedup (x)",
    #         title=f"{version}",
    #     )

    #     ax.set_xticks(threads)
    #     # ax.set_ylim(0, 16)
    #     # ax.semilogy()

    #     # sns.scatterplot(
    #     #     data=v,
    #     #     x="threads",
    #     #     y="time",
    #     #     hue="dataset",
    #     #     palette="Set2",
    #     #     style="dataset",
    #     #     # markers=True,
    #     #     # markersize=10,
    #     #     # linewidth=2.5,
    #     #     ci=95,
    #     #     # err_style="bars",
    #     #     # marker="o",
    #     # )

    #     sns.lineplot(
    #         data=v,
    #         x="threads",
    #         y="speedup",
    #         hue="dataset",
    #         palette="Set2",
    #         style="dataset",
    #         # markers=True,
    #         # markersize=10,
    #         # linewidth=2.5,
    #         ci=95,
    #         # err_style="bars",
    #         # marker="o",
    #     )

    #     fig.savefig(f"./results/figures/{version}.pdf")
    #     # fig.legend(
    #     #     labels=[
    #     #         "test",
    #     #     ]
    #     # )

# %%

sns.set_context("paper")
with sns.axes_style("whitegrid"):
    fig, ax = plt.subplots(figsize=(12, 6))
    ax.set(
        xlabel="Threads (n)",
        ylabel="Speedup (x)",
        title=f"Single thread performance",
    )

    # for v in versions:
    # p1 = sns.color_palette("Set2", light=0.2)
    # dps = df[df.speedup != 1]
    sns.scatterplot(
        data=df[(df.threads == 20) & (df.library == "openmp")],
        x="dataset",
        hue="version",
        y="time",
        palette="pastel",
        # style="dataset",
        # ci=95,
        # alpha=0.8,
    )
    sns.scatterplot(
        data=df[(df.threads == 20) & (df.library == "cilk")],
        x="dataset",
        hue="version",
        y="time",
        palette="muted",
        # style="dataset",
        # ci=95,
        # alpha=0.8,
    )

    sns.scatterplot(
        data=df[(df.threads == 20) & (df.library == "pthreads")],
        x="dataset",
        hue="version",
        y="time",
        palette="Set2",
        # style="dataset",
        # ci=95,
        # alpha=0.8,
    )
    # sns.scatterplot(
    #     data=v4[v4.threads == 20],
    #     x="dataset",
    #     hue="version",
    #     y="speedup",
    #     palette="muted",
    #     ci=95,
    #     # alpha=0.8,
    # )

    fig.savefig(f"./results/figures/single_thread.pdf")


# %%
with sns.axes_style("whitegrid"):
    fig, ax = plt.subplots(figsize=(12, 6))
    ax.set(
        xlabel="Library",
        ylabel="Execution time (s)",
        title="V3 to V4 Execution times comparison (threads >= 2)",
    )

    sns.violinplot(
        data=df[df.threads > 1],
        x="library",
        y="time",
        hue="version",
        palette="Set2",
        # style="dataset",
        # split=True,
        # scale="count",
        ci=95,
    )
    fig.savefig(f"./results/figures/v3_vs_v4.pdf")


# %%
with sns.axes_style("whitegrid"):
    fig, ax = plt.subplots(figsize=(12, 6))

    ax.set(
        #     xlabel="Library",
        #     ylabel="Execution time (s)",
        title="V3 to V4 Execution times comparison (threads >= 4)",
    )
    ax.set_xticklabels(ax.get_xticklabels(), rotation=45, ha="right")
    g = sns.catplot(
        data=df,
        # data=df[df.threads >= 4],
        x="dataset",
        hue="threads",
        y="time",
        palette="Set2",
        kind="bar",
        col="version",
        # style="dataset",
        # ci=95,
        # alpha=0.8,
    )
    g.set_xticklabels(rotation=30)
    g.savefig(f"./results/figures/v3_vs_v4.pdf")


# %%%
with sns.axes_style("whitegrid"):
    fig, ax = plt.subplots(figsize=(12, 6))

    ax.set(
        #     xlabel="Library",
        #     ylabel="Execution time (s)",
        title="V3 to V4 Execution times comparison (threads >= 4)",
    )
    ax.set_xticklabels(ax.get_xticklabels(), rotation=45, ha="right")
    g = sns.catplot(
        data=df[df.version == "v4"],
        # data=df,
        # x="dataset",
        x="library",
        hue="threads",
        y="speedup",
        palette="Set2",
        kind="bar",
        # col="library",
        col="dataset",
        # style="dataset",
        # ci=95,
        # alpha=0.8,
    )
    g.set_xticklabels(rotation=30)
    g.savefig(f"./results/figures/library_vs.pdf")

# %%%
df[(df.speedup != 1) & (df.threads == 1)]
