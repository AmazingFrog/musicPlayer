#-------------------------------------------------
#
# Project created by QtCreator 2019-02-04T18:02:50
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = musicPlayer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        decode.cpp \
        linearBuffer.cpp \
        play.cpp \
    crossplatform.cpp

HEADERS += \
        mainwindow.h \
        decode.h \
        linearBuffer.h \
        play.h \
        util.h \
        crossplatform.h \
    include/linux/AL/al.h \
    include/linux/AL/alc.h \
    include/linux/AL/alext.h \
    include/linux/AL/alut.h \
    include/linux/AL/efx-creative.h \
    include/linux/AL/efx-presets.h \
    include/linux/AL/efx.h \
    include/linux/glog/log_severity.h \
    include/linux/glog/logging.h \
    include/linux/glog/raw_logging.h \
    include/linux/glog/stl_logging.h \
    include/linux/glog/vlog_is_on.h \
    include/linux/libavcodec/ac3_parser.h \
    include/linux/libavcodec/adts_parser.h \
    include/linux/libavcodec/avcodec.h \
    include/linux/libavcodec/avdct.h \
    include/linux/libavcodec/avfft.h \
    include/linux/libavcodec/d3d11va.h \
    include/linux/libavcodec/dirac.h \
    include/linux/libavcodec/dv_profile.h \
    include/linux/libavcodec/dxva2.h \
    include/linux/libavcodec/jni.h \
    include/linux/libavcodec/mediacodec.h \
    include/linux/libavcodec/qsv.h \
    include/linux/libavcodec/vaapi.h \
    include/linux/libavcodec/vdpau.h \
    include/linux/libavcodec/version.h \
    include/linux/libavcodec/videotoolbox.h \
    include/linux/libavcodec/vorbis_parser.h \
    include/linux/libavcodec/xvmc.h \
    include/linux/libavformat/avformat.h \
    include/linux/libavformat/avio.h \
    include/linux/libavformat/version.h \
    include/linux/libavutil/adler32.h \
    include/linux/libavutil/aes.h \
    include/linux/libavutil/aes_ctr.h \
    include/linux/libavutil/attributes.h \
    include/linux/libavutil/audio_fifo.h \
    include/linux/libavutil/avassert.h \
    include/linux/libavutil/avconfig.h \
    include/linux/libavutil/avstring.h \
    include/linux/libavutil/avutil.h \
    include/linux/libavutil/base64.h \
    include/linux/libavutil/blowfish.h \
    include/linux/libavutil/bprint.h \
    include/linux/libavutil/bswap.h \
    include/linux/libavutil/buffer.h \
    include/linux/libavutil/camellia.h \
    include/linux/libavutil/cast5.h \
    include/linux/libavutil/channel_layout.h \
    include/linux/libavutil/common.h \
    include/linux/libavutil/cpu.h \
    include/linux/libavutil/crc.h \
    include/linux/libavutil/des.h \
    include/linux/libavutil/dict.h \
    include/linux/libavutil/display.h \
    include/linux/libavutil/downmix_info.h \
    include/linux/libavutil/encryption_info.h \
    include/linux/libavutil/error.h \
    include/linux/libavutil/eval.h \
    include/linux/libavutil/ffversion.h \
    include/linux/libavutil/fifo.h \
    include/linux/libavutil/file.h \
    include/linux/libavutil/frame.h \
    include/linux/libavutil/hash.h \
    include/linux/libavutil/hmac.h \
    include/linux/libavutil/hwcontext.h \
    include/linux/libavutil/hwcontext_cuda.h \
    include/linux/libavutil/hwcontext_d3d11va.h \
    include/linux/libavutil/hwcontext_drm.h \
    include/linux/libavutil/hwcontext_dxva2.h \
    include/linux/libavutil/hwcontext_mediacodec.h \
    include/linux/libavutil/hwcontext_qsv.h \
    include/linux/libavutil/hwcontext_vaapi.h \
    include/linux/libavutil/hwcontext_vdpau.h \
    include/linux/libavutil/hwcontext_videotoolbox.h \
    include/linux/libavutil/imgutils.h \
    include/linux/libavutil/intfloat.h \
    include/linux/libavutil/intreadwrite.h \
    include/linux/libavutil/lfg.h \
    include/linux/libavutil/log.h \
    include/linux/libavutil/lzo.h \
    include/linux/libavutil/macros.h \
    include/linux/libavutil/mastering_display_metadata.h \
    include/linux/libavutil/mathematics.h \
    include/linux/libavutil/md5.h \
    include/linux/libavutil/mem.h \
    include/linux/libavutil/motion_vector.h \
    include/linux/libavutil/murmur3.h \
    include/linux/libavutil/opt.h \
    include/linux/libavutil/parseutils.h \
    include/linux/libavutil/pixdesc.h \
    include/linux/libavutil/pixelutils.h \
    include/linux/libavutil/pixfmt.h \
    include/linux/libavutil/random_seed.h \
    include/linux/libavutil/rational.h \
    include/linux/libavutil/rc4.h \
    include/linux/libavutil/replaygain.h \
    include/linux/libavutil/ripemd.h \
    include/linux/libavutil/samplefmt.h \
    include/linux/libavutil/sha.h \
    include/linux/libavutil/sha512.h \
    include/linux/libavutil/spherical.h \
    include/linux/libavutil/stereo3d.h \
    include/linux/libavutil/tea.h \
    include/linux/libavutil/threadmessage.h \
    include/linux/libavutil/time.h \
    include/linux/libavutil/timecode.h \
    include/linux/libavutil/timestamp.h \
    include/linux/libavutil/tree.h \
    include/linux/libavutil/twofish.h \
    include/linux/libavutil/version.h \
    include/linux/libavutil/xtea.h \
    include/windows/AL/al.h \
    include/windows/AL/alc.h \
    include/windows/AL/efx-creative.h \
    include/windows/AL/EFX-Util.h \
    include/windows/AL/efx.h \
    include/windows/AL/xram.h \
    include/windows/libavcodec/ac3_parser.h \
    include/windows/libavcodec/adts_parser.h \
    include/windows/libavcodec/avcodec.h \
    include/windows/libavcodec/avdct.h \
    include/windows/libavcodec/avfft.h \
    include/windows/libavcodec/d3d11va.h \
    include/windows/libavcodec/dirac.h \
    include/windows/libavcodec/dv_profile.h \
    include/windows/libavcodec/dxva2.h \
    include/windows/libavcodec/jni.h \
    include/windows/libavcodec/mediacodec.h \
    include/windows/libavcodec/qsv.h \
    include/windows/libavcodec/vaapi.h \
    include/windows/libavcodec/vdpau.h \
    include/windows/libavcodec/version.h \
    include/windows/libavcodec/videotoolbox.h \
    include/windows/libavcodec/vorbis_parser.h \
    include/windows/libavcodec/xvmc.h \
    include/windows/libavformat/avformat.h \
    include/windows/libavformat/avio.h \
    include/windows/libavformat/version.h \
    include/windows/libavutil/adler32.h \
    include/windows/libavutil/aes.h \
    include/windows/libavutil/aes_ctr.h \
    include/windows/libavutil/attributes.h \
    include/windows/libavutil/audio_fifo.h \
    include/windows/libavutil/avassert.h \
    include/windows/libavutil/avconfig.h \
    include/windows/libavutil/avstring.h \
    include/windows/libavutil/avutil.h \
    include/windows/libavutil/base64.h \
    include/windows/libavutil/blowfish.h \
    include/windows/libavutil/bprint.h \
    include/windows/libavutil/bswap.h \
    include/windows/libavutil/buffer.h \
    include/windows/libavutil/camellia.h \
    include/windows/libavutil/cast5.h \
    include/windows/libavutil/channel_layout.h \
    include/windows/libavutil/common.h \
    include/windows/libavutil/cpu.h \
    include/windows/libavutil/crc.h \
    include/windows/libavutil/des.h \
    include/windows/libavutil/dict.h \
    include/windows/libavutil/display.h \
    include/windows/libavutil/downmix_info.h \
    include/windows/libavutil/encryption_info.h \
    include/windows/libavutil/error.h \
    include/windows/libavutil/eval.h \
    include/windows/libavutil/ffversion.h \
    include/windows/libavutil/fifo.h \
    include/windows/libavutil/file.h \
    include/windows/libavutil/frame.h \
    include/windows/libavutil/hash.h \
    include/windows/libavutil/hmac.h \
    include/windows/libavutil/hwcontext.h \
    include/windows/libavutil/hwcontext_cuda.h \
    include/windows/libavutil/hwcontext_d3d11va.h \
    include/windows/libavutil/hwcontext_drm.h \
    include/windows/libavutil/hwcontext_dxva2.h \
    include/windows/libavutil/hwcontext_mediacodec.h \
    include/windows/libavutil/hwcontext_qsv.h \
    include/windows/libavutil/hwcontext_vaapi.h \
    include/windows/libavutil/hwcontext_vdpau.h \
    include/windows/libavutil/hwcontext_videotoolbox.h \
    include/windows/libavutil/imgutils.h \
    include/windows/libavutil/intfloat.h \
    include/windows/libavutil/intreadwrite.h \
    include/windows/libavutil/lfg.h \
    include/windows/libavutil/log.h \
    include/windows/libavutil/lzo.h \
    include/windows/libavutil/macros.h \
    include/windows/libavutil/mastering_display_metadata.h \
    include/windows/libavutil/mathematics.h \
    include/windows/libavutil/md5.h \
    include/windows/libavutil/mem.h \
    include/windows/libavutil/motion_vector.h \
    include/windows/libavutil/murmur3.h \
    include/windows/libavutil/opt.h \
    include/windows/libavutil/parseutils.h \
    include/windows/libavutil/pixdesc.h \
    include/windows/libavutil/pixelutils.h \
    include/windows/libavutil/pixfmt.h \
    include/windows/libavutil/random_seed.h \
    include/windows/libavutil/rational.h \
    include/windows/libavutil/rc4.h \
    include/windows/libavutil/replaygain.h \
    include/windows/libavutil/ripemd.h \
    include/windows/libavutil/samplefmt.h \
    include/windows/libavutil/sha.h \
    include/windows/libavutil/sha512.h \
    include/windows/libavutil/spherical.h \
    include/windows/libavutil/stereo3d.h \
    include/windows/libavutil/tea.h \
    include/windows/libavutil/threadmessage.h \
    include/windows/libavutil/time.h \
    include/windows/libavutil/timecode.h \
    include/windows/libavutil/timestamp.h \
    include/windows/libavutil/tree.h \
    include/windows/libavutil/twofish.h \
    include/windows/libavutil/version.h \
    include/windows/libavutil/xtea.h

FORMS += \
        mainwindow.ui

win32{
INCLUDEPATH += ./include/windows
LIBS += -L. -lOpenAL32 -lavcodec -lavutil -lavformat
}

unix{
INCLUDEPATH += -I$$PWD/include/linux
LIBS  += -L$$PWD/lib/linux -lopenal -lavcodec -lavutil -lavformat -lglog
}

DESTDIR = ./bin

RESOURCES += \
    res.qrc
