# Robosim II
Robotic simulation software

I wrote Robosim II as part of my doctoral dissertation research at Vanderbilt University. I received my PhD in 1993 and I did some updates to Robosim over the next few years, but I eventually lost track of the source code in the late 90s. However, I recently happened upon an old floppy disk that had the source code on it.

The original Robosim was written in Fortran for DEC VAX by Ken Fernandez at Vanderbilt and it ran on Evans & Sutherland graphics terminals. Robosim II is very different and was written in C++ targeting Windows. I originally wrote it targeting NT 3.1, but it also ran on Win32s at the time, which was the 32-bit subset that ran on 16-bit Windows 3.1. It used MFC, and I wrote all of the rendering code for it as well, and it initially included stereoscopic views. A few years after I graduated, I updated it to use OpenGL and removed the steroscopic code. This is the version of the software that I found. I spent a little bit of time updating it to build with current Visual C++ and replaced some of the outdated code with C++ "std" replacements. I did a pass on renaming and fixed a few issues that the current compiler found. The version number is updated to 1.4.

I initially developed Robosim II on a 486 CPU running at 33Mhz with 4M of RAM and a 120M hard disk. This was quite slow at running NT 3.1, but it worked for me at the time.

I received a NASA Certificate of Recognition for my work on Robosim II in 2005. Here is a mention of Robosim and Robosim II in some work at NASA from 2004.
https://ntrs.nasa.gov/api/citations/20050215332/downloads/20050215332.pdf

My dissertation discusses some of the internals of Robosim II in more detail, but I have not been able to find an electronic copy of it. However, I did find the Word document that was originally used to generate a Windows HLP file. I've converted this to HTML help and those files (and the CHM generated from them) are part of this repo as well.

I am releasing this under the MIT license, and I hope others may find this useful.
