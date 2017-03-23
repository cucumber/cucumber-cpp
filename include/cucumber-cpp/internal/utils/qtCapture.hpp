/* Based on Sir Digby Chicken post from:
 * http://stackoverflow.com/questions/5419356/redirect-stdout-stderr-to-a-string
 * who granted his work on BSD license
 * adjusted for Qt by konserw
 */
#ifdef _MSC_VER
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
    static void Init() {
        // make stdout & stderr streams unbuffered
        // so that we don't need to flush the streams
        // before capture and after capture 
        // (fflush can cause a deadlock if the stream is currently being 
        QMutexLocker locker(&m_mutex);
        setvbuf(stdout, NULL, _IONBF, 0);
        setvbuf(stderr, NULL, _IONBF, 0);
    }

    static void BeginCapture() {
        QMutexLocker locker(&m_mutex);
        if (m_capturing)
            return;

        secure_pipe(m_pipe);
        m_oldStdOut = secure_dup(STD_OUT_FD);
        m_oldStdErr = secure_dup(STD_ERR_FD);
        secure_dup2(m_pipe[WRITE], STD_OUT_FD);
        secure_dup2(m_pipe[WRITE], STD_ERR_FD);
        m_capturing = true;
#ifndef _MSC_VER
        secure_close(m_pipe[WRITE]);
#endif
    }

    static bool IsCapturing() {
        QMutexLocker locker(&m_mutex);
        return m_capturing;
    }

    static void EndCapture() {
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
#ifdef _MSC_VER
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
#ifdef _MSC_VER
        secure_close(m_pipe[WRITE]);
#endif
        m_capturing = false;
    }

    static std::string GetCapture() {
        QMutexLocker locker(&m_mutex);
        return m_captured;
    }

private:
    enum PIPES { READ, WRITE };

    static int secure_dup(int src) {
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

    static void secure_pipe(int * pipes) {
        int ret = -1;
        bool fd_blocked = false;
        do {
#ifdef _MSC_VER
            ret = pipe(pipes, 65536, O_BINARY);
#else
            ret = pipe(pipes) == -1;
#endif
            fd_blocked = (errno == EINTR ||  errno == EBUSY);
            if (fd_blocked)
                QThread::msleep(10);
        } while (ret < 0);
    }

    static void secure_dup2(int src, int dest) {
        int ret = -1;
        bool fd_blocked = false;
        do {
             ret = dup2(src,dest);
             fd_blocked = (errno == EINTR ||  errno == EBUSY);
             if (fd_blocked)
                QThread::msleep(10);
        } while (ret < 0);
    }

    static void secure_close(int & fd) {
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

    static int m_pipe[2];
    static int m_oldStdOut;
    static int m_oldStdErr;
    static bool m_capturing;
    static QMutex m_mutex;
    static std::string m_captured;
};

// actually define vars.
int qtCapture::m_pipe[2];
int qtCapture::m_oldStdOut;
int qtCapture::m_oldStdErr;
bool qtCapture::m_capturing;
QMutex qtCapture::m_mutex;
std::string qtCapture::m_captured;
