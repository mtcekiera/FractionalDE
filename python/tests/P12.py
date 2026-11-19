import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

DATA_DIR = Path("data/P12")
PLOTS_DIR = Path("plots/P12")

methods = {
    "GL": "gl.csv",
    "L1": "l1.csv",
    "ABM": "abm.csv",
    "PI-trapezoid": "pt.csv"
}

data = {
    name: pd.read_csv(DATA_DIR / filename)
    for name, filename in methods.items()
}


def plot_errors(column, ylabel, title, filename):

    plt.figure(figsize=(9, 6))

    for name, df in data.items():

        N = df["N"].to_numpy()
        error = df[column].to_numpy()

        valid = (N > 0) & (error > 0)

        log_N = np.log(N[valid])
        log_error = np.log(error[valid])

        a, b = np.polyfit(log_N, log_error, 1)

        points, = plt.loglog(
            N, error,
            marker="x",
            linestyle="None",
            markersize=10
        )

        color = points.get_color()

        N_fit = np.geomspace(N[valid].min(), N[valid].max(), 200)
        error_fit = np.exp(b) * N_fit**a

        plt.loglog(
            N_fit, error_fit,
            linestyle="--",
            linewidth=1.5,
            color=color,
            label=f"{name} (a = {a:.3f})",
            alpha=0.5
        )

    plt.xlabel("Number of time steps $N$")
    plt.ylabel(ylabel)
    plt.title(title)

    plt.xticks(
        [2**k for k in range(4, 11)],
        [str(2**k) for k in range(4, 11)]
    )

    plt.grid(True, which="both", alpha=0.3)
    plt.legend()
    plt.tight_layout()

    plt.savefig(PLOTS_DIR / filename, dpi=300)


plot_errors(
    "final_error",
    r"$\varepsilon_N = |y_N-y(T)|$",
    "Final absolute error",
    "final_error.png"
)

plot_errors(
    "max_error",
    r"$\varepsilon_{\max} = \max_n |y_n-y(t_n)|$",
    "Maximum absolute error",
    "max_error.png"
)