#include <fstream>
#include <complex>
#include <chrono>
#include <iostream>

using namespace std;
using namespace chrono;

// Function to compute the Mandelbrot iteration count for a specific point
int mandelbrot(int max_iter, double x, double y) {
    complex<double> point(x, y);
    complex<double> z(0, 0);
    int iter = 0;

    while (abs(z) < 2 && iter < max_iter) {
        z = z * z + point;
        iter++;
    }

    return iter;
}

// Function to generate the Mandelbrot set and save it to a PPM file
void generateMandelbrot(const char* filename, int width, int height, int max_iter) {
    ofstream image(filename);
    image << "P3\n" << width << " " << height << "\n255\n";

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            double dx = (double)x / (width - 1) * 4.0 - 2.0;
            double dy = (double)y / (height - 1) * 4.0 - 2.0;
            int iter = mandelbrot(max_iter, dx, dy);
            int color = 255 * iter / max_iter;
            image << color << " " << 0 << " " << 0 << " ";
        }
        image << "\n";
    }

    image.close();
}

int main() {
    int width = 800;      // Width of the image
    int height = 800;     // Height of the image
    int max_iter = 1000;  // Maximum number of iterations

    // Start the timer
    auto start_time = high_resolution_clock::now();

    generateMandelbrot("mandelbrot.ppm", width, height, max_iter);

    // Stop the timer and calculate the duration
    auto stop_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop_time - start_time);

    // Print the time taken
    cout << "Time taken to generate the image: " << duration.count() << " milliseconds." << endl;

    return 0;
}
