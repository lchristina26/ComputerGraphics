#configuration
CONFIG +=  cross_compile shared qpa no_mocdepend release qt_no_framework
host_build {
    QT_ARCH = x86_64
    QT_TARGET_ARCH = arm
} else {
    QT_ARCH = arm
    QMAKE_DEFAULT_LIBDIRS = /opt/android/ndk/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/lib/gcc/arm-linux-androideabi/4.8 /opt/android/ndk/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/lib/gcc /opt/android/ndk/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/arm-linux-androideabi/lib /opt/android/ndk/platforms/android-9/arch-arm/usr/lib
    QMAKE_DEFAULT_INCDIRS = /usr/local/ssl/include . /opt/android/ndk/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/lib/gcc/arm-linux-androideabi/4.8/include /opt/android/ndk/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/lib/gcc/arm-linux-androideabi/4.8/include-fixed /opt/android/ndk/platforms/android-9/arch-arm/usr/include
}
QT_CONFIG +=  minimal-config small-config medium-config large-config full-config no-pkg-config evdev c++11 accessibility egl eglfs opengl opengles2 shared qpa reduce_exports clock-gettime clock-monotonic mremap getaddrinfo ipv6ifname inotify eventfd png freetype harfbuzz system-zlib openssl rpath concurrent audio-backend release

#versioning
QT_VERSION = 5.4.0
QT_MAJOR_VERSION = 5
QT_MINOR_VERSION = 4
QT_PATCH_VERSION = 0

#namespaces
QT_LIBINFIX = 
QT_NAMESPACE = 

QT_GCC_MAJOR_VERSION = 4
QT_GCC_MINOR_VERSION = 8
QT_GCC_PATCH_VERSION = 0
