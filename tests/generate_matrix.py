#!/usr/bin/env python3
import sys
import numpy as np

def generate_matrix(size: int, det_value: float) -> np.ndarray:
    for _ in range(10):
        A = np.random.uniform(-100, 100, (size, size))
        current_det = np.linalg.det(A)

        if abs(current_det) < 1e-8:
            continue

        ratio = abs(det_value / current_det)
        scale_factor = ratio ** (1.0 / size)

        A_scaled = A * scale_factor

        final_det = np.linalg.det(A_scaled)
        if np.sign(final_det) != np.sign(det_value):
            A_scaled[0, :] *= -1

        return A_scaled

def main():
    if len(sys.argv) != 3:
        print("Usage: matrix_generator.py <size> <determinant>")
        sys.exit(1)

    size = int(sys.argv[1])
    det_value = float(sys.argv[2])

    matrix = generate_matrix(size, det_value)

    print(size)
    for row in matrix:
        print(" ".join(str(x) for x in row))

if __name__ == "__main__":
    main()

