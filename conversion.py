from PIL import Image

# # Load the PPM image
# img = Image.open("mandelbrot.ppm")

# # Convert the image to JPEG
# img.save("mandelbrot.jpeg", "JPEG")

# Load the PPM image
img = Image.open("mandelbrot_parallel.ppm")

# Convert the image to JPEG
img.save("mandelbrot_parallel.jpeg", "JPEG")