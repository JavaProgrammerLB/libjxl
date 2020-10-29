# JPEG XL reference implementation

This repository contains a reference implementation of JPEG XL (encoder and
decoder).

**NOTE**

JPEG XL is in the final stages of standardization, but minor changes to the
codestream are still likely. **WARNING:** until further notice, do not depend on
future decoders being able to decode the output of a current encoder.

## Quick start guide

For more details and other workflows see the "Advanced guide" below.

### Checking out the code

```bash
git clone https://gitlab.com/wg1/jpeg-xl.git --recursive
```

This repository uses git submodules to handle some third party dependencies
under `third_party/`, that's why is important to pass `--recursive`. If you
didn't check out with `--recursive`, or any submodule has changed, run:
`git submodule update --init --recursive`.

Important: If you downloaded a zip file or tarball from the web interface you
won't get the needed submodules and the code will not compile. Use `git clone`
as described before.

### Installing dependencies

Required dependencies for compiling the code, in a Debian/Ubuntu based
distribution run:

```bash
sudo apt install cmake pkg-config
```

Optional dependencies for supporting other formats in the `cjxl`/`djxl` tools,
in a Debian/Ubuntu based distribution run:

```bash
sudo apt install libgif-dev libjpeg-dev libopenexr-dev libpng-dev libwebp-dev
```

We recommend using a recent Clang compiler (version 7 or newer), for that
install clang and set `CC` and `CXX` variables. For example, with clang-7:

```bash
sudo apt install clang-7
export CC=clang-7 CXX=clang++-7
```

### Building

```bash
cd jpeg-xl
cmake -Bbuild -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTING=OFF .
cmake --build build
```

The encoder/decoder tools will be available in the `build/tools` directory.

### Basic encoder/decoder

To encode a source image to JPEG XL with default settings:

```bash
build/tools/cjxl input.png output.jxl
```

For more settings run `build/tools/cjxl --help` or for a full list of options
run `build/tools/cjxl -v -v --help`.

To decode a JPEG XL file run:

```bash
build/tools/djxl input.jxl output.png
```

When possible `cjxl`/`djxl` are able to read/write the following
image formats: .exr, .gif, .jpeg/.jpg, .pfm, .pgm/.ppm, .pgx, .png.

### Benchmarking

For speed benchmarks on single images in single or multi-threaded decoding
`djxl` can print decoding speed information. See `djxl --help` for details
on the decoding options and note that the output image is optional for
benchmarking purposes.

For a more comprehensive comparison of compression density between multiple
options see "Benchmarking with benchmark_xl" section below.

## Advanced guide

### Building with Docker

We build a common environment based on Debian/Ubuntu using Docker. Other
systems may have different combinations of versions and dependencies that
have not been tested and may not work. For those cases we recommend using the
Docker environment as explained in the
[step by step guide](doc/developing_in_docker.md).

### Building JPEG XL for developers

For experienced developers, we also provide build instructions for an [up to
date Debian-based Linux](doc/developing_in_debian.md) and [64-bit
Windows](doc/developing_in_windows.md). If you encounter any difficulties,
please use Docker instead.

## Benchmarking with benchmark_xl

We recommend `build/tools/benchmark_xl` as a convenient method for reading
images or image sequences, encoding them using various codecs (jpeg jxl png
webp), decoding the result, and computing objective quality metrics. An example
invocation is:

```bash
build/tools/benchmark_xl --input "/path/*.png" --codec jxl:wombat:d1,jxl:cheetah:d2
```

Multiple comma-separated codecs are allowed. The characters after : are
parameters for the codec, separated by colons, in this case specifying maximum
target psychovisual distances of 1 and 2 (higher implies lower quality) and
the encoder effort (see below). Other common parameters are `r0.5` (target
bitrate 0.5 bits per pixel) and `q92` (quality 92, on a scale of 0-100, where
higher is better). The `jxl` codec supports the following additional parameters:

Speed: `falcon`, `cheetah`, `hare`, `wombat`, `squirrel`, `kitten`, `tortoise`
control the encoder effort in ascending order.

*   `falcon` disables all of the following tools.
*   `cheetah` enables coefficient reordering, context clustering, and heuristics
    for selecting DCT sizes and quantization steps.
*   `hare` enables Gaborish filtering, chroma from luma, and an initial estimate
    of quantization steps.
*   `wombat` enables error diffusion quantization and full DCT size selection
    heuristics.
*   `squirrel` (default) enables dots, patches, and spline detection, and full
    context clustering.
*   `kitten` optimizes the adaptive quantization for a psychovisual metric.
*   `tortoise` enables a more thorough adaptive quantization search.

Mode: JPEG XL has several modes for various types of content. The default mode
is suitable for photographic material. One of the following alternatives may be
selected:

*   `mg` activates modular mode (useful for non-photographic images such as
    screen content).
*   `bg` activates lossless JPEG reconstruction with parallel decoding (the
    input must have been a JPEG file).
*   `b:file` activates lossless JPEG reconstruction with more compact encodings,
    but without the option of parallel decoding.

Other arguments to benchmark_xl include:

*   `save_compressed`: save codestreams to `output_dir`.
*   `save_decompressed`: save decompressed outputs to `output_dir`.
*   `output_extension`: selects the format used to output decoded images.
*   `num_threads`: number of codec instances that will independently
    encode/decode images, or 0.
*   `inner_threads`: how many threads each instance should use for parallel
    encoding/decoding, or 0.
*   `encode_reps`/`decode_reps`: how many times to repeat encoding/decoding
    each image, for more consistent measurements (we recommend 10).

The benchmark output begins with a header:

```
Compr              Input    Compr            Compr       Compr  Decomp  Butteraugli
Method            Pixels     Size              BPP   #    MP/s    MP/s     Distance    Error p norm           BPP*pnorm   Errors
```

`ComprMethod` lists each each comma-separated codec. `InputPixels` is the number
of pixels in the input image. `ComprSize` is the codestream size in bytes and
`ComprBPP` the bitrate. `Compr MP/s` and `Decomp MP/s` are the
compress/decompress throughput, in units of Megapixels/second.
`Butteraugli Distance` indicates the maximum psychovisual error in the decoded
image (larger is worse). `Error p norm` is a similar summary of the psychovisual
error, but closer to an average, giving less weight to small low-quality
regions. `BPP*pnorm` is the product of `ComprBPP` and `Error p norm`, which is a
figure of merit for the codec (lower is better). `Errors` is nonzero if errors
occurred while loading or encoding/decoding the image.

## License

This software is available under Apache 2.0 license which can be found in the
[LICENSE](LICENSE) file.

## Additional documentation

### Codec description

*   [Introductory paper](https://www.spiedigitallibrary.org/proceedings/Download?fullDOI=10.1117%2F12.2529237) (open-access)
*   [XL Overview](doc/xl_overview.md) - a brief introduction to the
    source code modules
*   [JPEG XL committee draft](https://arxiv.org/abs/1908.03565)
*   JPEG XL white paper with overview of applications and coding tools:
    WG1 output document number wg1n86059

### Development process
*   [Docker setup - **start here**](doc/developing_in_docker.md)
*   [Building on Debian](doc/developing_in_debian.md) - for experts only
*   [Building on Windows](doc/developing_in_windows.md) - for experts only
*   [More information on testing/build options](doc/building_and_testing.md)
*   [Git guide for JPEG XL](doc/developing_in_gitlab.md) - for developers only
*   [Building Web Assembly artifacts](doc/building_wasm.md)
