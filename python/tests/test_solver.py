import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import scipy.special as spc


def plot_relation(
    x, numerical, analytical, filename, *,
    num_label="Numerical",
    ana_label="Analytical",
    xlabel="x",
    ylabel="f(x)",
    title=None,
    legend=True,
    logy = False
):

    x = np.asarray(x)
    numerical = np.asarray(numerical)
    exact = analytical(x)

    fig, ax = plt.subplots()

    ax.plot(x, numerical, color="black",
            linestyle="-", label=num_label)

    ax.plot(x, exact, color="blue",
            linestyle="--", label=ana_label)
    if logy:
        ax.set_yscale("log")
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
    plt.clf()


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

LAMBDA = 1.0
def theory_1(t):
    return np.exp(-LAMBDA * t)

# z = t**alpha, alpha = 0.5 => z = t**0.5
def theory_2(t):
    return np.exp(LAMBDA**2 * t) * spc.erfc(LAMBDA * np.sqrt(t))
# alpha = 1


def plot_tests(prefix):
    data_a1 = pd.read_csv(f"data/{prefix}_test_a1.csv")
    plot_relation(data_a1["t"], data_a1["y0"], theory_1, f"plots/{prefix}_test_a1.png", title=r"$\alpha=1.0,\,h=0.001$")
    plot_error(data_a1["t"], data_a1["y0"], theory_1, f"plots/{prefix}_test_a1_error.png", title=r"$\alpha=1.0,\,h=0.001$")

    # alpha = 0.5
    data_a05 = pd.read_csv(f"data/{prefix}_test_a05.csv")
    plot_relation(data_a05["t"], data_a05["y0"], theory_2, f"plots/{prefix}_test_a05.png", title=r"$\alpha=0.5,\,h=0.001$")
    plot_error(data_a05["t"], data_a05["y0"], theory_2, f"plots/{prefix}_test_a05_error.png", title=r"$\alpha=0.5,\,h=0.001$")

solvers = ['gl', 'l1', 'abm', 'pt']
for solver in solvers:
    plot_tests(solver)