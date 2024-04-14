#include <iostream>
#include <fstream>
#include <complex>
#include <mpi.h>

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 800;
const int MAX_ITER = 1000;

int mandelbrot(double x, double y) {
    complex<double> point(x, y);
    complex<double> z(0, 0);
    int iter = 0;
    while (abs(z) < 2 && iter < MAX_ITER) {
        z = z * z + point;
        iter++;
    }
    return iter;
}

void save_to_file(const char* filename, int* counts) {
    ofstream image(filename);
    image << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";
    for (int i = 0; i < WIDTH * HEIGHT; i++) {
        int color = (255 * counts[i]) / MAX_ITER;
        image << color << " " << 0 << " " << 0 << " ";
        if ((i + 1) % WIDTH == 0) image << "\n";
    }
    image.close();
}

int main(int argc, char **argv) {
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n_rows = HEIGHT / size;  // Number of rows per process
    int* sub_counts = new int[WIDTH * n_rows];
    int* all_counts = nullptr;

    double startTime = MPI_Wtime();

    for (int i = 0; i < n_rows; i++) {
        int y = rank * n_rows + i;
        for (int x = 0; x < WIDTH; x++) {
            double dx = (double)x / (WIDTH - 1) * 4.0 - 2.0;
            double dy = (double)y / (HEIGHT - 1) * 4.0 - 2.0;
            sub_counts[i * WIDTH + x] = mandelbrot(dx, dy);
        }
    }

    if (rank == 0) {
        all_counts = new int[WIDTH * HEIGHT];
    }

    MPI_Gather(sub_counts, WIDTH * n_rows, MPI_INT, all_counts, WIDTH * n_rows, MPI_INT, 0, MPI_COMM_WORLD);

    double endTime = MPI_Wtime();

    if (rank == 0) {
        cout << "Time taken to generate the fractal: " << (endTime - startTime) << " seconds." << endl;
        save_to_file("mandelbrot_mpi.ppm", all_counts);
        delete[] all_counts;
    }

    delete[] sub_counts;
    MPI_Finalize();
    return 0;
}
