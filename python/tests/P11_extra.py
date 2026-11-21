import numpy as np
import matplotlib.pyplot as plt
import scipy.special as spc
import pandas as pd
from pathlib import Path

import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path


def plot_csv(
    filename,
    labels=None,
    xlabel="t",
    ylabel="y(t)",
    title=None,
    output=None,
    figsize=(9, 6),
):
    df = pd.read_csv(filename)

    t = df.iloc[:, 0]
    y = df.iloc[:, 1:]

    if labels is None:
        labels = y.columns.tolist()

    if len(labels) != y.shape[1]:
        raise ValueError("Number of labels must match number of data columns.")

    fig, ax = plt.subplots(figsize=figsize)

    for col, label in zip(y.columns, labels):
        ax.plot(t, y[col], label=label, linewidth=1.5)

    ax.plot(t, np.exp(-t), linestyle="--", color="black", alpha=0.5, label=r"$e^{-t}$")

    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.set_title(title)
    ax.grid(True, alpha=0.3)
    ax.legend()

    fig.tight_layout()

    if output is not None:
        output = Path(output)
        output.parent.mkdir(parents=True, exist_ok=True)
        fig.savefig(output, dpi=300, bbox_inches="tight")

    plt.close(fig)


if __name__ == "__main__":

    DATA_DIR = Path("data/P11_extra")
    PLOTS_DIR = Path("plots/P11_extra")

    PLOTS_DIR.mkdir(parents=True, exist_ok=True)

    plot_csv(DATA_DIR / "P11_extra.csv",
             output=PLOTS_DIR / "P11_extra.png",
             labels= [
                 r"$\alpha=0.3$",
                 r"$\alpha=0.5$",
                 r"$\alpha=0.8$",
                 r"$\alpha=1.0$",
             ],
             xlabel = r"$t$",
             ylabel = r"$ML^{\alpha}_5(t)$",
             title = "Mittag-Leffler (t-domain)"
             )
