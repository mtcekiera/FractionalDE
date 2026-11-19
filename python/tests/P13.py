import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path


DATA_DIR = Path("data/P13")
PLOTS_DIR = Path("plots/P13")

METHODS = {
    "GL": "gl.csv",
    "L1": "l1.csv",
    "ABM": "abm.csv",
    "PI-trapezoid": "pt.csv"
}

ALPHAS = {
    "a03": 0.3,
    "a05": 0.5,
    "a08": 0.8
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


# ============================================================
# Error plots
# ============================================================

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


# ============================================================
# EOC plots
# ============================================================

def plot_eoc(data, column, title, filename):

    fig, ax = plt.subplots(figsize=(9, 6))

    for name, df in data.items():

        N = df["N"].to_numpy()
        eoc = df[column].to_numpy()

        valid = np.isfinite(eoc)

        ax.scatter(
            N[valid],
            eoc[valid],
            marker="o",
            s=40,
            color=COLORS[name],
            label=name
        )

    configure_axes(ax)

    ax.set_xlabel("Number of time steps $N$")
    ax.set_ylabel("Empirical order of convergence")
    ax.set_title(title)

    ax.legend()
    fig.tight_layout()

    fig.savefig(filename, dpi=300)
    plt.close(fig)


# ============================================================
# Main
# ============================================================

if __name__ == "__main__":

    for alpha_name, alpha in ALPHAS.items():

        data_dir = DATA_DIR / alpha_name
        plots_dir = PLOTS_DIR / alpha_name

        plots_dir.mkdir(parents=True, exist_ok=True)

        data = load_data(data_dir)

        # Final absolute error
        plot_errors(
            data,
            "final_error",
            r"$\varepsilon_N = |y_N-y(T)|$",
            rf"Final absolute error ($\alpha={alpha}$)",
            plots_dir / "final_error.png"
        )

        # Maximum absolute error
        plot_errors(
            data,
            "max_error",
            r"$\varepsilon_{\max}=\max_n|y_n-y(t_n)|$",
            rf"Maximum absolute error ($\alpha={alpha}$)",
            plots_dir / "max_error.png"
        )

        # EOC for final error
        plot_eoc(
            data,
            "eoc_final",
            rf"Final error EOC ($\alpha={alpha}$)",
            plots_dir / "eoc_final.png"
        )

        # EOC for maximum error
        plot_eoc(
            data,
            "eoc_max",
            rf"Maximum error EOC ($\alpha={alpha}$)",
            plots_dir / "eoc_max.png"
        )

        # Print convergence table
        print(f"\n{'='*65}")
        print(f"alpha = {alpha}")
        print(f"{'='*65}")

        for name, df in data.items():

            print(f"\n{name}")

            print(
                df[
                    [
                        "N",
                        "final_error",
                        "max_error",
                        "eoc_final",
                        "eoc_max"
                    ]
                ].to_string(
                    index=False,
                    float_format=lambda x: f"{x:.5e}"
                )
            )