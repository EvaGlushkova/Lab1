## Report: parallel processing of BMP images

## Experiment methodology
Reproducible experiments were conducted to compare performance. In each run, the execution time of image processing was measured using one and two threads.

The amount of allocated memory for pixels: **308,224 bytes**

### Measurement results:
#### First run:
| One thread | 2 threads |
|------------|-----------|
| 4 ms       | 1 ms      |
| 2 ms       | 1 ms      |
| 7 ms       | 7 ms      |

![](First_run.jpeg)

#### Second run:

| One thread | 12 threads |
|------------|------------|
| 3 ms       | 6 ms       |
| 8 ms       | 2 ms       |
| 7 ms       | 4 ms       |

![](Second_run.jpeg)

**Conclusion**: multithreaded implementation shows improvement in processing time in most cases.


## Peculiarities of testing in VirtualBox
The experiments were conducted in the VirtualBox environment. This could have affected the accuracy and stability of the execution time measurements for the following reasons:

- A virtual machine typically uses a limited number of virtual cores, which does not correspond to the number of physical processors on the host machine.

- Thread management and task scheduling in a virtualized environment may be less efficient than on real hardware.

- Hardware resources may be shared between the virtual machine and other host processes, reducing the performance of multithreaded tasks.

- There may be additional overhead for synchronization and context switching of threads within the virtual environment.

**Conclusion**: Running a multithreaded implementation in VirtualBox may not reflect the actual performance of the code on a physical computer. For an accurate assessment of performance, it is recommended to run the program on a real system without virtualization.

## Performance Note
Running an application with multithreading added on windows shows a 2X increase in execution speed. The difference in execution speed is due to differences in the environment in which the application was run
![](windows_run.jpeg)


# Test Description

| Test | Description |
|------|--|
| **LeftClock90_MultiEqualsSingleThreaded** | Comparison of pixel data after left rotation in single-threaded and multi-threaded implementations |
| **RightClock90_MultiEqualsSingleThreaded** | Comparison of pixel data after right rotation in single-threaded and multi-threaded implementations |
| **Gauss_MultiEqualsSingleThreaded** | Comparison of Gaussian filter application results in single-threaded and multi-threaded versions |
| **RotateLeftThenRight_EqualsOriginal** | Verification that sequential left and right rotations preserve the original image dimensions |
| **RotateLeftFourTimes_EqualsOriginal** | 	Verification that four consecutive left rotations return the image to its original state |
| **ApplyGaussianTwice_NotEqualToOnce** | Verification that applying Gaussian filter twice produces a different result than applying it once|
| **RotateAndSaveImage** | 	Verification of correct saving of rotated image to file and its loading with dimension checking | 
