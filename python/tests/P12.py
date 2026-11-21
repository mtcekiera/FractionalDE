import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path


# ============================================================
# Configuration
# ============================================================

DATA_DIR = Path("data/P12")
PLOTS_DIR = Path("plots/P12")

PLOTS_DIR.mkdir(parents=True, exist_ok=True)

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

# Total integration time (used if CSV has no h column)
T = 1.0

# Fitting configuration
SKIP_FIT = ()
MIN_R2 = None
FORCE_FIT = ()

# Optional fitting interval
FIT_H_MIN = None
FIT_H_MAX = None


# ============================================================
# Load data
# ============================================================

def load_data():

    data = {}

    for name, filename in METHODS.items():

        df = pd.read_csv(DATA_DIR / filename)

        # Use h directly if available
        if "h" not in df.columns:
            df["h"] = T / df["N"]

        df = df.sort_values("h").reset_index(drop=True)

        data[name] = df

    return data


# ============================================================
# Axes configuration
# ============================================================

def configure_axes(ax):

    ax.set_xscale("log", base=10)

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
    min_r2=None,
    force_fit=(),
    fit_h_min=None,
    fit_h_max=None
):

    fig, ax = plt.subplots(figsize=(9, 6))

    print(f"\n{title}")
    print("-" * 75)

    for name, df in data.items():

        h = df["h"].to_numpy(dtype=float)
        error = df[column].to_numpy(dtype=float)

        color = COLORS[name]

        # ----------------------------------------------------
        # Valid data
        # ----------------------------------------------------

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
        # Select fitting interval
        # ----------------------------------------------------

        fit_mask = np.ones(len(h_valid), dtype=bool)

        if fit_h_min is not None:
            fit_mask &= h_valid >= fit_h_min

        if fit_h_max is not None:
            fit_mask &= h_valid <= fit_h_max

        h_data = h_valid[fit_mask]
        error_data = error_valid[fit_mask]

        can_fit = (
            len(h_data) >= 2
            and len(np.unique(h_data)) >= 2
        )

        do_fit = False
        p = np.nan
        b = np.nan
        r2 = np.nan

        # ----------------------------------------------------
        # Log-log regression
        # ----------------------------------------------------

        if can_fit:

            log_h = np.log10(h_data)
            log_error = np.log10(error_data)

            # log10(error) = p * log10(h) + b
            p, b = np.polyfit(
                log_h,
                log_error,
                1
            )

            predicted = p * log_h + b

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

            # ------------------------------------------------
            # Determine whether to display fit
            # ------------------------------------------------

            do_fit = (
                name in force_fit
                or (
                    name not in skip_fit
                    and (
                        min_r2 is None
                        or (
                            len(h_data) >= 3
                            and np.isfinite(r2)
                            and r2 >= min_r2
                        )
                    )
                )
            )

        print(
            f"{name:15s} "
            f"p = {p:8.4f}  "
            f"R² = {r2:9.6f}  "
            f"fit = {do_fit}"
        )

        # ----------------------------------------------------
        # Numerical data - scatter only
        # ----------------------------------------------------

        ax.scatter(
            h_valid,
            error_valid,
            marker="o",
            s=40,
            color=color,
            zorder=3,
            label=(
                f"{name} (p = {p:.3f})"
                if do_fit else name
            )
        )

        if not do_fit:
            continue

        # ----------------------------------------------------
        # Fitted power law: error = C * h^p
        # ----------------------------------------------------

        h_fit = np.geomspace(
            h_data.min(),
            h_data.max(),
            300
        )

        error_fit = 10**b * h_fit**p

        ax.plot(
            h_fit,
            error_fit,
            linestyle="-",
            linewidth=1.5,
            color=color
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

    fig.savefig(
        PLOTS_DIR / filename,
        dpi=300,
        bbox_inches="tight"
    )

    plt.close(fig)


# ============================================================
# Main
# ============================================================

if __name__ == "__main__":

    data = load_data()

    # Final absolute error
    plot_errors(
        data,
        "final_error",
        r"$\varepsilon_{\mathrm{final}} = |y_N-y(T)|$",
        "Final absolute error",
        "final_error.png",
        skip_fit=SKIP_FIT,
        min_r2=MIN_R2,
        force_fit=FORCE_FIT,
        fit_h_min=FIT_H_MIN,
        fit_h_max=FIT_H_MAX
    )

    # Maximum absolute error
    plot_errors(
        data,
        "max_error",
        r"$\varepsilon_{\max} = \max_n |y_n-y(t_n)|$",
        "Maximum absolute error",
        "max_error.png",
        skip_fit=SKIP_FIT,
        min_r2=MIN_R2,
        force_fit=FORCE_FIT,
        fit_h_min=FIT_H_MIN,
        fit_h_max=FIT_H_MAX
    )