import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from pathlib import Path

DATA_DIR = Path("data/P11_extra2")
PLOTS_DIR = Path("plots/P11_extra2")

df = pd.read_csv(DATA_DIR / "P11_extra2.csv")

PLOTS_DIR.mkdir(
    parents=True,
    exist_ok=True
)

def plot_errors(df, error_type, output):
    plt.figure(figsize=(8, 5))

    columns = [
        col for col in df.columns
        if col.startswith(f"{error_type}_error_a")
    ]

    for col in columns:
        alpha = float(col.split("_a")[-1]) / 10

        plt.plot(
            df["terms"],
            df[col].replace(0, np.nan),
            label=rf"$\alpha = {alpha:g}$",
            linewidth=1.5
        )

    plt.xlabel("Number of terms")
    plt.ylabel("Absolute error")
    plt.yscale("log")
    plt.title(f"{error_type.capitalize()} error")
    plt.legend()
    plt.grid(True, which="both", alpha=0.3)
    plt.tight_layout()

    plt.savefig(output)
    plt.close()


plot_errors(
    df,
    "max",
    PLOTS_DIR / "max_error.png"
)

plot_errors(
    df,
    "final",
    PLOTS_DIR / "final_error.png"
)