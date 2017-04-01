#include "cucumber-cpp/internal/drivers/QtTestDriver.hpp"

#include <QtTest/QtTest>

/* Based on Sir Digby Chicken post from:
 * http://stackoverflow.com/questions/5419356/redirect-stdout-stderr-to-a-string
 * who granted his work on BSD license
 * adjusted for Qt by konserw
 */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#define WINDOWS
#include <io.h>
#include <fcntl.h>
#define popen _popen 
#define pclose _pclose
#define stat _stat 
#define dup _dup
#define dup2 _dup2
#define fileno _fileno
#define close _close
#define pipe _pipe
#define read _read
#define eof _eof
#else
#include <errno.h>
#endif

#include <cstdio>
#include <QtConcurrent/QtConcurrent>

#ifndef STD_OUT_FD
#define STD_OUT_FD (fileno(stdout))
#endif
#ifndef STD_ERR_FD
#define STD_ERR_FD (fileno(stderr))
#endif

class qtCapture {
public:
    qtCapture(): m_capturing(false) {
        // make stdout & stderr streams unbuffered
        // so that we don't need to flush the streams
        // before capture and after capture 
        // (fflush can cause a deadlock if the stream is currently being 
        QMutexLocker locker(&m_mutex);
        setvbuf(stdout, NULL, _IONBF, 0);
        setvbuf(stderr, NULL, _IONBF, 0);
    }
    ~qtCapture() {
        if(m_capturing)
            EndCapture();
    }

    void BeginCapture() {
        QMutexLocker locker(&m_mutex);
        if (m_capturing)
            return;

        secure_pipe(m_pipe);
        m_oldStdOut = secure_dup(STD_OUT_FD);
        m_oldStdErr = secure_dup(STD_ERR_FD);
        secure_dup2(m_pipe[WRITE], STD_OUT_FD);
        secure_dup2(m_pipe[WRITE], STD_ERR_FD);
        m_capturing = true;
#ifndef WINDOWS
        secure_close(m_pipe[WRITE]);
#endif
    }

    bool IsCapturing() {
        QMutexLocker locker(&m_mutex);
        return m_capturing;
    }

    void EndCapture() {
        QMutexLocker locker(&m_mutex);
        if (!m_capturing)
            return;

        m_captured.clear();
        secure_dup2(m_oldStdOut, STD_OUT_FD);
        secure_dup2(m_oldStdErr, STD_ERR_FD);

        const int bufSize = 1025;
        char buf[bufSize];
        int bytesRead = 0;
        bool fd_blocked(false);
        do {
            bytesRead = 0;
            fd_blocked = false;
#ifdef WINDOWS
            if (!eof(m_pipe[READ]))
                bytesRead = read(m_pipe[READ], buf, bufSize-1);
#else
            bytesRead = read(m_pipe[READ], buf, bufSize-1);
#endif
            if (bytesRead > 0) {
                buf[bytesRead] = 0;
                m_captured += buf;
            } else if (bytesRead < 0) {
                fd_blocked = (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR);
                if (fd_blocked)
                    QThread::msleep(10);
            }
        }
        while(fd_blocked || bytesRead == (bufSize-1));

        secure_close(m_oldStdOut);
        secure_close(m_oldStdErr);
        secure_close(m_pipe[READ]);
#ifdef WINDOWS
        secure_close(m_pipe[WRITE]);
#endif
        m_capturing = false;
    }

    std::string GetCapture() {
        QMutexLocker locker(&m_mutex);
        return m_captured;
    }

private:
    enum PIPES { READ, WRITE };

    int secure_dup(int src) {
        int ret = -1;
        bool fd_blocked = false;
        do {
             ret = dup(src);
             fd_blocked = (errno == EINTR ||  errno == EBUSY);
             if(fd_blocked)
                 QThread::msleep(10);
        } while (ret < 0);
        return ret;
    }

    void secure_pipe(int * pipes) {
        int ret = -1;
        bool fd_blocked = false;
        do {
#ifdef WINDOWS
            ret = pipe(pipes, 65536, O_BINARY);
#else
            ret = pipe(pipes) == -1;
#endif
            fd_blocked = (errno == EINTR ||  errno == EBUSY);
            if (fd_blocked)
                QThread::msleep(10);
        } while (ret < 0);
    }

    void secure_dup2(int src, int dest) {
        int ret = -1;
        bool fd_blocked = false;
        do {
             ret = dup2(src,dest);
             fd_blocked = (errno == EINTR ||  errno == EBUSY);
             if (fd_blocked)
                QThread::msleep(10);
        } while (ret < 0);
    }

    void secure_close(int & fd) {
        int ret = -1;
        bool fd_blocked = false;
        do {
             ret = close(fd);
             fd_blocked = (errno == EINTR);
             if (fd_blocked)
                QThread::msleep(10);
        } while (ret < 0);

        fd = -1;
    }

    int m_pipe[2];
    int m_oldStdOut;
    int m_oldStdErr;
    bool m_capturing;
    QMutex m_mutex;
    std::string m_captured;
};

namespace cucumber {
namespace internal {

const InvokeResult QtTestStep::invokeStepBody() {
    QtTestObject testObject(this);

    qtCapture capture;
    capture.BeginCapture();
    int returnValue = QTest::qExec(&testObject, 0, NULL);
    capture.EndCapture();

    if(returnValue == 0)
        return InvokeResult::success();
    else
        return InvokeResult::failure(capture.GetCapture());
}

}
}

