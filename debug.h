#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG_LOG(msg) qDebug() << "[" << __FILE__ << ":" << __LINE__ << "]" << "[" << __FUNCTION__ << "]" << msg

#endif // DEBUG_H
