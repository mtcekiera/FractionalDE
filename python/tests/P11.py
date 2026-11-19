import numpy as np
import matplotlib.pyplot as plt
import scipy.special as spc
import pandas as pd
from pathlib import Path


def plot_relation(
    x, numerical, analytical, filename, *,
    num_label="Numerical",
    ana_label="Analytical",
    xlabel="x",
    ylabel="f(x)",
    title=None
):

    x = np.asarray(x)
    numerical = np.asarray(numerical)

    fig, ax = plt.subplots(figsize=(9, 6))

    ax.scatter(
        x, numerical,
        marker="o",
        s=10,
        color="C0",
        label=num_label,
        zorder=3
    )

    x_fit = np.linspace(x.min(), x.max(), 1000)
    y_fit = analytical(x_fit)

    ax.plot(
        x_fit, y_fit,
        linestyle="-",
        linewidth=3.0,
        color="C1",
        label=ana_label
    )

    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)

    if title is not None:
        ax.set_title(title)

    ax.grid(True, which="both", alpha=0.3)
    ax.legend()
    fig.tight_layout()

    fig.savefig(filename, dpi=300)
    plt.close(fig)


def plot_error(
    x, numerical, analytical, filename, *,
    relative=True,
    log_scale=True,
    xlabel="x",
    ylabel=None,
    title=None
):

    x = np.asarray(x)
    numerical = np.asarray(numerical)
    exact = np.asarray(analytical(x))

    error = np.abs(numerical - exact)

    if relative:
        error = np.divide(
            error,
            np.abs(exact),
            out=np.full_like(error, np.nan, dtype=float),
            where=np.abs(exact) > 0
        )

    fig, ax = plt.subplots(figsize=(9, 6))

    ax.scatter(
        x, error,
        marker="o",
        s=20,
        color="C0"
    )

    if log_scale:
        ax.set_yscale("log")

    ax.set_xlabel(xlabel)

    if ylabel is None:
        ylabel = "Relative error" if relative else "Absolute error"

    ax.set_ylabel(ylabel)

    if title is not None:
        ax.set_title(title)

    ax.grid(True, which="both", alpha=0.3)
    fig.tight_layout()

    fig.savefig(filename, dpi=300)
    plt.close(fig)

    return error


if __name__ == "__main__":

    DATA_DIR = Path("data/P11")
    PLOTS_DIR = Path("plots/P11")

    PLOTS_DIR.mkdir(parents=True, exist_ok=True)

    data = pd.read_csv(DATA_DIR / "P11.csv")

    z = data["z"].to_numpy()
    x = -z

    data_a05 = data["ML1"].to_numpy()
    data_a1 = data["ML2"].to_numpy()

    # Analytical solutions
    analytic_a05 = lambda x: spc.erfcx(x)
    analytic_a1 = lambda z: np.exp(z)


    # ========================================================
    # alpha = 0.5
    # ========================================================

    plot_relation(
        x,
        data_a05,
        analytic_a05,
        PLOTS_DIR / "ML_a05.png",
        num_label=r"Numerical $E_{1/2}(-x)$",
        ana_label=r"$e^{x^2}\mathrm{erfc}(x)$",
        xlabel=r"$x$",
        ylabel=r"$E_{1/2}(-x)$",
        title=r"Mittag-Leffler function ($\alpha=0.5$)"
    )

    errors_a05 = plot_error(
        x,
        data_a05,
        analytic_a05,
        PLOTS_DIR / "ML_a05_error.png",
        relative=True,
        log_scale=False,
        xlabel=r"$x$",
        ylabel="Relative error",
        title=r"Relative error ($\alpha=0.5$)"
    )


    # ========================================================
    # alpha = 1.0
    # ========================================================

    plot_relation(
        z,
        data_a1,
        analytic_a1,
        PLOTS_DIR / "ML_a1.png",
        num_label=r"Numerical $E_1(z)$",
        ana_label=r"$e^z$",
        xlabel=r"$z$",
        ylabel=r"$E_1(z)$",
        title=r"Mittag-Leffler function ($\alpha=1$)"
    )

    errors_a1 = plot_error(
        z,
        data_a1,
        analytic_a1,
        PLOTS_DIR / "ML_a1_error.png",
        relative=True,
        log_scale=False,
        xlabel=r"$z$",
        ylabel="Relative error",
        title=r"Relative error ($\alpha=1$)"
    )


    # ========================================================
    # Error summary
    # ========================================================

    # print(f"Max relative error (alpha=0.5): {np.nanmax(errors_a05):.6e}")
    # print(f"Max relative error (alpha=1.0): {np.nanmax(errors_a1):.6e}")