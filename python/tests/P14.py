import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path


DATA_DIR = Path("data/P14")
PLOTS_DIR = Path("plots/P14")

METHODS = {
    "GL": "gl.csv",
    "L1": "l1.csv",
    "ABM": "abm.csv",
    "PI-trapezoid": "pt.csv"
}


COLORS = {
    "GL": "C0",
    "L1": "C1",
    "ABM": "C2",
    "PI-trapezoid": "C3"
}


def load_data(directory):

    return {
        name: pd.read_csv(directory / filename)
        for name, filename in METHODS.items()
    }


def configure_axes(ax):

    N_ticks = [2**k for k in range(4, 11)]

    ax.set_xscale("log", base=2)
    ax.set_xticks(N_ticks)
    ax.set_xticklabels([str(n) for n in N_ticks])

    ax.grid(True, which="both", alpha=0.3)


def plot_errors(data, column, ylabel, title, filename):

    fig, ax = plt.subplots(figsize=(9, 6))

    for name, df in data.items():

        N = df["N"].to_numpy()
        error = df[column].to_numpy()

        color = COLORS[name]

        valid = np.isfinite(error) & (error > 0)

        N_valid = N[valid]
        error_valid = error[valid]

        if len(N_valid) < 2:
            continue

        # Linear fit in log-log scale
        a, b = np.polyfit(
            np.log(N_valid),
            np.log(error_valid),
            1
        )

        # Numerical data - scatter only
        ax.scatter(
            N_valid,
            error_valid,
            marker="o",
            s=35,
            color=color,
            zorder=3
        )

        # Fitted power law
        N_fit = np.geomspace(
            N_valid.min(),
            N_valid.max(),
            200
        )

        error_fit = np.exp(b) * N_fit**a

        ax.plot(
            N_fit,
            error_fit,
            linestyle="-",
            linewidth=1.5,
            color=color,
            label=f"{name} (a = {a:.3f})"
        )

    ax.set_yscale("log")

    configure_axes(ax)

    ax.set_xlabel("Number of time steps $N$")
    ax.set_ylabel(ylabel)
    ax.set_title(title)

    ax.legend()
    fig.tight_layout()

    fig.savefig(filename, dpi=300)
    plt.close(fig)



def plot_work_precision(
    p13_dir="data/P13/a05",
    p14_dir="data/P14",
    output_dir="plots/P14"
):
    p13_dir = Path(p13_dir)
    p14_dir = Path(p14_dir)
    output_dir = Path(output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    methods = {
        "gl": "GL",
        "l1": "L1",
        "abm": "ABM",
        "pt": "PI-trapez",
    }

    colors = {
        "gl": "#1f3a93",
        "l1": "#c8412b",
        "abm": "#238b45",
        "pt": "#9467bd",
    }

    data = {}

    # Wczytanie i połączenie danych po N
    for method in methods:
        errors = pd.read_csv(p13_dir / f"{method}.csv")
        times = pd.read_csv(p14_dir / f"{method}.csv")

        merged = pd.merge(
            errors,
            times[["N", "time"]],
            on="N",
            how="inner"
        )

        data[method] = merged.sort_values("N")

    # Osobne wykresy dla błędu końcowego i maksymalnego
    for error_type, ylabel, filename in [
        ("final_error", "Final error", "work_precision_final"),
        ("max_error", "Maximum error", "work_precision_max")
    ]:

        fig, ax = plt.subplots(figsize=(7.5, 5))

        for method, label in methods.items():
            df = data[method]

            mask = (
                np.isfinite(df["time"]) &
                np.isfinite(df[error_type]) &
                (df["time"] > 0) &
                (df[error_type] > 0)
            )

            ax.plot(
                df.loc[mask, "time"],
                df.loc[mask, error_type],
                marker="o",
                markersize=5,
                linewidth=1.6,
                color=colors[method],
                label=label
            )

        ax.set_xscale("log")
        ax.set_yscale("log")

        ax.set_xlabel("Computational time [s]")
        ax.set_ylabel(ylabel)

        ax.grid(True, which="major", alpha=0.35)
        ax.grid(True, which="minor", alpha=0.12)

        ax.legend(frameon=True)

        fig.tight_layout()
        fig.savefig(
            output_dir / f"{filename}.png",
            bbox_inches="tight"
        )

        plt.close(fig)




if __name__ == "__main__":


    alpha = 0.5
    data_dir = DATA_DIR
    plots_dir = PLOTS_DIR

    plots_dir.mkdir(parents=True, exist_ok=True)

    data = load_data(data_dir)

    plot_errors(
        data,
        "time",
        r"$t$ [s]",
        rf"Execution time",
        plots_dir / "exec_time.png"
    )
    plot_work_precision()

