# Luz Studio

A live motion graphics editor and performer.

<http://createdigitalmotion.com/2011/03/luz-live-motion-graphics-controlled-by-anything-free-on-linux-and-now-with-dmx>

## Video Tutorials

<http://lighttroupe.com/luz>

## The Luz Project consists of:

- **Luz Studio**: a live motion graphics editor (Ruby, Gtk+, OpenGL)
- **Luz Performer**: plays Luz projects fullscreen (Ruby, SDL, OpenGL)
- **Luz Video Recorder**: renders smooth HD video of Luz projects (Ruby, SDL, OpenGL, ffmpeg)
- **Luz Audio Player**: plays music and sends song progress percentage to Luz (Ruby, GStreamer)
- **Luz Input Manager**: sends live input device data to Luz (C++, Gtk+, XOrg API, libwiimote, SDL Input)
- **Luz Spectrum Analyzer**: sends audio information to Luz (C++, OpenGL, FFTW)
- **Luz Hand Tracker**: sends hand position, angle, and finger data to Luz (C++, Leap)
- **Luz Body Tracker**: sends human skeleton data to Luz (C++, OpenGL, OpenNI+NITE)

# Getting Luz

Luz currently only runs on Linux.  Help is wanted porting it to OSX.

1. **git clone git://github.com/lighttroupe/luz.git**

2. Install dependencies as described in the INSTALL file.

3. Run **make** in the root directory to build Input Manager and Spectrum Analyzer.

4. Optionally run the **./build** scripts in utils/video-file, utils/video, and utils/chipmunk.

5. Run **wget openanswers.org/gtkglext.so** to get the Ruby bindings to GtkGL, which are now missing from the RubyGNOME project.

# Unofficial 64 bit DEB packages from openartist.org

http://openartist.org/debian/trusty64/luz_0.92.1~20130620-1_amd64.deb
http://openartist.org/debian/trusty64/luz-next_20141027-1_amd64.deb
