#ifndef NETWORKQUEUE_H
#define NETWORKQUEUE_H

#include <QVector>
#include <QMutex>

#include "WebLoader.h"
#include "NetworkRequestInternal.h"

/*!
 * \brief Класс, реализующий очередь запросов
 * Реализован как паттерн Singleton
 */
class NetworkQueue : public QObject
{
    Q_OBJECT
public:

    /*!
     * \brief Метод, возвращающий указатель на инстанс класса
     */
    static NetworkQueue* getInstance();
    /*!
     * \brief Метод, помещающий в очередь запрос
     */
    void put(NetworkRequestInternal*);
    /*!
     * \brief Метод, останавливающий выполняющийся запрос
     * или же убирающий его из очереди
     */
    void stop(NetworkRequestInternal*);

signals:
    /*!
     * \brief Прогресс отправки запроса на сервер
     */
    void uploadProgress(int);
    /*!
     * \brief Прогресс загрузки данных с сервера
     */
    void downloadProgress(int);
    /*!
     * \brief Данные загружены
     */
    void downloadComplete(QByteArray);
    void downloadComplete(QString);
    /*!
     * \brief Сигнал об ошибке
     */
    void error(QString);

private slots:
    /*!
     * \brief Слот, выполняющийся после завершения выполнения запроса
     * Начинает выполнение следующего запроса в очереди
     */
    void downloadComplete(WebLoader *wl);

private:
    /*!
     * \brief Приватные конструкторы и оператор присваивания
     * Для реализации паттерна Singleton
     */
    NetworkQueue();
    NetworkQueue(const NetworkQueue&);
    NetworkQueue& operator=(const NetworkQueue&);

    /*!
     * \brief Извлечение запроса из очереди и его выполнение
     */
    void pop();
    /*!
     * \brief Настройка параметров для WebLoader'а
     */
    void setWLParams(WebLoader *wl, NetworkRequestInternal *r);

    /*!
     * \brief Очередь запросов
     */
    QList<NetworkRequestInternal*> queue;
    /*!
     * \brief Множество, содержащее запросы в очереди
     * Необходим для быстрого определения, находится ли запрос в очереди
     */
    QSet<NetworkRequestInternal*> inQueue;
    /*!
     * \brief WebLoader'ы, выполняющие запрос и соответствующие им запросы
     */
    QMap<WebLoader*, NetworkRequestInternal*> usedWL;
    /*!
     * \brief Список свободных WebLoader'ов
     */
    QList<WebLoader*> freeWL;
    QMutex mtx;
};

#endif // NETWORKQUEUE_H
