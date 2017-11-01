HEADERS     += \
    src/comm/serial/qextserialbase.h \
    src/comm/serial/posix_qextserialport.h \
    src/comm/serial/interfaceserial.h \
    src/comm/interface.h \
    src/comm/routing.h \
    src/comm/can/interfacecan.h \
    src/comm/error.h\
    src/comm/meter/meterprotocol.h \
    src/comm/meter/meterdriver.h \
    src/comm/meter/meterdevice.h \
    src/comm/htmu/htmu.h \
    src/comm/cardreader/cardreader.h \
    src/comm/htmu/htmuportdc.h \
    src/comm/htmu/htmutransportprotocol.h \
    src/comm/cardreader/mifareone.h


SOURCES     += \
    src/comm/serial/qextserialbase.cpp \
    src/comm/serial/posix_qextserialport.cpp \
    src/comm/serial/interfaceserial.cpp \
    src/comm/interface.cpp \
    src/comm/routing.cpp \
    src/comm/can/interfacecan.cpp \
    src/comm/meter/meterprotocol.cpp \
    src/comm/meter/meterdriver.cpp \
    src/comm/meter/meterdevice.cpp\
    src/comm/error.cpp \
    src/comm/htmu/htmu.cpp \
    src/comm/cardreader/cardreader.cpp \
    src/comm/htmu/htmuportdc.cpp \
    src/comm/htmu/htmutransportprotocol.cpp \
    src/comm/cardreader/mifareone.cpp
