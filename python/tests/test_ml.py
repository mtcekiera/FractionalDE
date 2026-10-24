import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path
import scipy.special as spc
import pandas as pd

def plot_relation(
    x, numerical, analytical, filename, *,
    num_label="Numerical",
    ana_label="Analytical",
    xlabel="x",
    ylabel="f(x)",
    title=None,
    legend=True
):

    x = np.asarray(x)
    numerical = np.asarray(numerical)
    exact = analytical(x)

    fig, ax = plt.subplots()

    ax.plot(x, numerical, color="black",
            linestyle="-", label=num_label)

    ax.plot(x, exact, color="blue",
            linestyle="--", label=ana_label)

    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    if title is not None:
        ax.set_title(title)
    ax.grid()

    if legend:
        ax.legend()

    # Path(filename).parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(filename, dpi=300, bbox_inches="tight")
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

    fig, ax = plt.subplots()

    ax.scatter(x, error, s=15)

    if log_scale:
        ax.set_yscale("log")

    ax.set_xlabel(xlabel)

    if ylabel is None:
        ylabel = "Relative error" if relative else "Absolute error"

    ax.set_ylabel(ylabel)
    if title is not None:
        ax.set_title(title)
    ax.grid()

    # Path(filename).parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(filename, dpi=300, bbox_inches="tight")
    plt.close(fig)

    return error

if __name__ == "__main__":



    DATA_DIR = Path("data")
    PLOTS_DIR = Path("plots")

    PLOTS_DIR.mkdir(parents=True, exist_ok=True)
    data = pd.read_csv(DATA_DIR / "ML_05_1.csv")
    z = data["z"]
    x = -z
    
    data_a05 = data["ML1"]
    analytic_a05 = lambda x: np.exp(x**2) * spc.erfc(x)
    
    data_a1 = data["ML2"]
    analytic_a1 = lambda z: np.exp(z)

    
    plot_relation(
        x,
        data_a05,
        analytic_a05,
        PLOTS_DIR / "ML_a05.png",
        num_label=r"$E_{1/2}(-x)$",
        ana_label=r"exp($x^2$)erfc(x)",
        xlabel=r"$x$",
        ylabel=r"$E_{1/2}(-x)$",
        title=r"$\alpha=0.5$"
    )

    errors_a05 = plot_error(
        x,
        data_a05,
        analytic_a05,
        PLOTS_DIR / "ML_a05_error.png",
        log_scale=False,
        xlabel="x",
        ylabel=r"err($E_{1/2}(z)$)",
        title=r"$\alpha=0.5$"
    )




    plot_relation(
        z,
        data_a1,
        analytic_a1,
        PLOTS_DIR / "ML_a1.png",
        num_label=r"$E_1(z)$",
        ana_label=r"$e^z$",
        xlabel=r"$z$",
        ylabel=r"$E_1(z)$",
        title=r"$\alpha=1$"
    )

    errors_a1 = plot_error(
        z,
        data_a1,
        analytic_a1,
        PLOTS_DIR / "ML_a1_error.png",
        log_scale=False,
        xlabel="z",
        ylabel=r"err($E_1(z))$",
        title=r"$\alpha=1$"
    )


    print("max error a=0.5: " + str(np.max(errors_a05)))
    print("max error a=1: " + str(np.max(errors_a1)))