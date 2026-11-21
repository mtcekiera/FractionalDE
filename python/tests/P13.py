import numpy as np
import pandas as pd
import matplotlib.pyplot as plt

from pathlib import Path
from matplotlib.ticker import LogLocator, NullFormatter


# ============================================================
# Configuration
# ============================================================

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

# Fitting configuration
SKIP_FIT = ()
MIN_R2 = 0.98

# Recalculate EOC from errors instead of reading CSV values
RECOMPUTE_EOC = True


# ============================================================
# EOC calculation
# ============================================================

def calculate_eoc(h, error):
    """
    Calculate empirical order of convergence:

        p_i = log(e_i / e_{i-1}) / log(h_i / h_{i-1})

    Assumes h is sorted in ascending order.

    EOC is assigned to the larger step size of each pair.
    """

    h = np.asarray(h, dtype=float)
    error = np.asarray(error, dtype=float)

    eoc = np.full(len(h), np.nan)

    for i in range(1, len(h)):

        h1, h2 = h[i - 1], h[i]
        e1, e2 = error[i - 1], error[i]

        if not (
            np.isfinite(h1)
            and np.isfinite(h2)
            and np.isfinite(e1)
            and np.isfinite(e2)
        ):
            continue

        if h1 <= 0 or h2 <= 0:
            continue

        if e1 <= 0 or e2 <= 0:
            continue

        if h1 == h2:
            continue

        eoc[i] = np.log(e2 / e1) / np.log(h2 / h1)

    return eoc


# ============================================================
# Data loading
# ============================================================

def load_data(directory):

    data = {}

    for name, filename in METHODS.items():

        filepath = directory / filename

        df = pd.read_csv(filepath)

        required = {"h", "final_error", "max_error"}

        missing = required - set(df.columns)

        if missing:
            raise ValueError(
                f"{filepath}: missing columns {missing}"
            )

        # Convert relevant columns to numeric values
        for column in required:
            df[column] = pd.to_numeric(
                df[column], errors="coerce"
            )

        # Remove invalid time steps
        df = df[
            np.isfinite(df["h"]) & (df["h"] > 0)
        ].copy()

        # Sort by increasing h
        df = df.sort_values("h").reset_index(drop=True)

        if RECOMPUTE_EOC:

            h = df["h"].to_numpy()

            df["eoc_final"] = calculate_eoc(
                h,
                df["final_error"].to_numpy()
            )

            df["eoc_max"] = calculate_eoc(
                h,
                df["max_error"].to_numpy()
            )

        data[name] = df

    return data


# ============================================================
# Axes configuration
# ============================================================

def configure_axes(ax):

    ax.set_xscale("log", base=10)

    # Major ticks: powers of 10
    ax.xaxis.set_major_locator(
        LogLocator(base=10)
    )

    # Minor ticks: 2, 3, ..., 9 per decade
    ax.xaxis.set_minor_locator(
        LogLocator(
            base=10,
            subs=np.arange(2, 10)
        )
    )

    ax.xaxis.set_minor_formatter(
        NullFormatter()
    )

    ax.grid(
        True,
        which="major",
        linestyle="-",
        linewidth=0.7,
        alpha=0.35
    )

    ax.grid(
        True,
        which="minor",
        linestyle=":",
        linewidth=0.5,
        alpha=0.2
    )

    ax.tick_params(
        axis="both",
        which="both",
        direction="in",
        top=True,
        right=True
    )


# ============================================================
# Error plots
# ============================================================

def plot_errors(
    data,
    column,
    ylabel,
    title,
    filename,
    *,
    skip_fit=(),
    min_r2=None
):

    fig, ax = plt.subplots(figsize=(9, 6))

    for name, df in data.items():

        h = df["h"].to_numpy()
        error = df[column].to_numpy()

        color = COLORS[name]

        # Valid data for logarithmic plotting
        valid = (
            np.isfinite(h)
            & np.isfinite(error)
            & (h > 0)
            & (error > 0)
        )

        h_valid = h[valid]
        error_valid = error[valid]

        if len(h_valid) == 0:
            continue

        # ----------------------------------------------------
        # Determine whether fitting is possible
        # ----------------------------------------------------

        do_fit = (
            name not in skip_fit
            and len(np.unique(h_valid)) >= 2
        )

        if do_fit:

            log_h = np.log10(h_valid)
            log_error = np.log10(error_valid)

            # Fit: log10(error) = p * log10(h) + b
            p, b = np.polyfit(
                log_h,
                log_error,
                1
            )

            predicted = p * log_h + b

            # R² in logarithmic space
            ss_res = np.sum(
                (log_error - predicted)**2
            )

            ss_tot = np.sum(
                (log_error - np.mean(log_error))**2
            )

            r2 = (
                1 - ss_res / ss_tot
                if ss_tot > 0
                else np.nan
            )

            # Optional fit quality check
            if min_r2 is not None:

                do_fit = (
                    len(h_valid) >= 3
                    and np.isfinite(r2)
                    and r2 >= min_r2
                )

        # ----------------------------------------------------
        # Numerical data
        # ----------------------------------------------------

        ax.scatter(
            h_valid,
            error_valid,
            marker="o",
            s=40,
            color=color,
            zorder=3,
            label=None if do_fit else name
        )

        if not do_fit:
            continue

        # ----------------------------------------------------
        # Fitted power law: error = C * h^p
        # ----------------------------------------------------

        h_fit = np.geomspace(
            h_valid.min(),
            h_valid.max(),
            300
        )

        error_fit = 10**b * h_fit**p

        ax.plot(
            h_fit,
            error_fit,
            linestyle="-",
            linewidth=1.5,
            color=color,
            label=f"{name} (p = {p:.3f})"
        )

    # --------------------------------------------------------
    # Axes
    # --------------------------------------------------------

    ax.set_yscale("log", base=10)

    configure_axes(ax)

    ax.set_xlabel(r"Time step $h$")
    ax.set_ylabel(ylabel)
    ax.set_title(title)

    ax.legend()

    fig.tight_layout()

    filename = Path(filename)
    filename.parent.mkdir(parents=True, exist_ok=True)

    fig.savefig(
        filename,
        dpi=300,
        bbox_inches="tight"
    )

    plt.close(fig)


# ============================================================
# EOC plots
# ============================================================

def plot_eoc(
    data,
    column,
    title,
    filename
):

    fig, ax = plt.subplots(figsize=(9, 6))

    for name, df in data.items():

        h = df["h"].to_numpy()
        eoc = df[column].to_numpy()

        valid = (
            np.isfinite(h)
            & np.isfinite(eoc)
            & (h > 0)
        )

        if not np.any(valid):
            continue

        ax.scatter(
            h[valid],
            eoc[valid],
            marker="o",
            s=40,
            color=COLORS[name],
            label=name,
            zorder=3
        )

    # --------------------------------------------------------
    # Axes
    # --------------------------------------------------------

    configure_axes(ax)

    ax.set_xlabel(r"Time step $h$")
    ax.set_ylabel("Empirical order of convergence")
    ax.set_title(title)

    ax.axhline(
        0,
        color="gray",
        linestyle="--",
        linewidth=0.8,
        alpha=0.6
    )

    ax.legend()

    fig.tight_layout()

    filename = Path(filename)
    filename.parent.mkdir(parents=True, exist_ok=True)

    fig.savefig(
        filename,
        dpi=300,
        bbox_inches="tight"
    )

    plt.close(fig)


# ============================================================
# Main
# ============================================================

if __name__ == "__main__":

    for alpha_name, alpha in ALPHAS.items():

        data_dir = DATA_DIR / alpha_name
        plots_dir = PLOTS_DIR / alpha_name

        plots_dir.mkdir(
            parents=True,
            exist_ok=True
        )

        data = load_data(data_dir)

        # ----------------------------------------------------
        # Final absolute error
        # ----------------------------------------------------

        plot_errors(
            data,
            "final_error",
            r"$\varepsilon_{\mathrm{final}} = |y_N-y(T)|$",
            rf"Final absolute error ($\alpha={alpha}$)",
            plots_dir / "final_error.png",
            skip_fit=SKIP_FIT,
            min_r2=MIN_R2
        )

        # ----------------------------------------------------
        # Maximum absolute error
        # ----------------------------------------------------

        plot_errors(
            data,
            "max_error",
            r"$\varepsilon_{\max} = \max_n |y_n-y(t_n)|$",
            rf"Maximum absolute error ($\alpha={alpha}$)",
            plots_dir / "max_error.png",
            skip_fit=SKIP_FIT,
            min_r2=MIN_R2
        )

        # ----------------------------------------------------
        # Final error EOC
        # ----------------------------------------------------

        plot_eoc(
            data,
            "eoc_final",
            rf"Final error EOC ($\alpha={alpha}$)",
            plots_dir / "eoc_final.png"
        )

        # ----------------------------------------------------
        # Maximum error EOC
        # ----------------------------------------------------

        plot_eoc(
            data,
            "eoc_max",
            rf"Maximum error EOC ($\alpha={alpha}$)",
            plots_dir / "eoc_max.png"
        )

        # ----------------------------------------------------
        # Print convergence tables
        # ----------------------------------------------------

        print(f"\n{'=' * 65}")
        print(f"alpha = {alpha}")
        print(f"{'=' * 65}")

        for name, df in data.items():

            print(f"\n{name}")

            print(
                df[
                    [
                        "h",
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