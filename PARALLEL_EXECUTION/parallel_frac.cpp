#include <iostream>
#include <fstream>
#include <complex>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;
using namespace chrono;

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

void compute_rows(int start_row, int end_row, int* output) {
    for (int y = start_row; y < end_row; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            double dx = (double)x / (WIDTH - 1) * 4.0 - 2.0;
            double dy = (double)y / (HEIGHT - 1) * 4.0 - 2.0;
            int iter = mandelbrot(dx, dy);
            output[y * WIDTH + x] = iter;
        }
    }
}

void save_to_file(const char* filename, const int* output) {
    ofstream image(filename);
    image << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

    for (int i = 0; i < WIDTH * HEIGHT; ++i) {
        int color = (255 * output[i]) / MAX_ITER;
        image << color << " " << 0 << " " << 0 << " ";
        if ((i + 1) % WIDTH == 0) image << "\n";
    }

    image.close();
}

int main() {
    vector<thread> threads;
    int num_threads = thread::hardware_concurrency();
    int* output = new int[WIDTH * HEIGHT];

    // Start the timer
    auto start_time = high_resolution_clock::now();

    int rows_per_thread = HEIGHT / num_threads;
    for (int i = 0; i < num_threads; ++i) {
        int start_row = i * rows_per_thread;
        int end_row = (i + 1) == num_threads ? HEIGHT : (i + 1) * rows_per_thread;
        threads.push_back(thread(compute_rows, start_row, end_row, output));
    }

    for (auto& t : threads) {
        t.join();
    }

    // Stop the timer and calculate the duration
    auto stop_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop_time - start_time);

    cout << "Time taken to generate the image in parallel: " << duration.count() << " milliseconds." << endl;

    save_to_file("mandelbrot_parallel.ppm", output);

    delete[] output;
    return 0;
}
