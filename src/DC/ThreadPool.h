#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include "dc_global.h"

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <QtCore/QThread>
#include <QtCore/QList>

// �����̳߳�
class DC_EXPORT ThreadPool : public QObject
{
    Q_OBJECT

private:

    static bool instanceFlag;
    static ThreadPool *single;

    QList<QThread *> m_threadPool;
    QList<QThread *> m_reservedThreadPool;

    QMutex m_lock;

    ThreadPool(); //˽�й���

public:
    static ThreadPool* getInstance();
    ~ThreadPool()
    {
        instanceFlag = false;
    }

    void stop();

    QThread *getThread();
    QThread *getReservedThread();

public slots:

signals:
};

#endif // _THREADPOOL_H_
