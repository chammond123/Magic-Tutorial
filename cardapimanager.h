#ifndef CARDAPIMANAGER_H
#define CARDAPIMANAGER_H

#include <QObject>
#include <QJsonArray>
#include <QMap>
#include "card.h"

class QNetworkAccessManager;
class QNetworkReply;

/**
 * @brief The CardAPIManager class
 */
class CardAPIManager : public QObject
{
    Q_OBJECT

public:

    /**
     * @brief The ReplyType enum
     */
    enum class ReplyType {
        Unknown,
        CardsList,
        SingleCard,
        CardImage
    };

    /**
     * @brief CardAPIManager
     * @param parent
     */
    explicit CardAPIManager(QObject *parent = nullptr);

    // Fetch all cards (original method)
    void fetchCards();

    // Fetch a single card by its exact name
    void fetchCardByName(const QString &cardName);

    // Fetch multiple cards by their names
    void fetchCardsByNames(const QStringList &cardNames);

signals:
    void cardsFetched(const QJsonArray &cards);
    void cardFetched(const Card &card);
    void batchFetchStarted(int totalCards);
    void batchFetchProgress(int completedCards);
    void batchCardsFetched(const QVector<Card> &cards);
    void errorOccurred(const QString &error);

private slots:
    void onFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QMap<QNetworkReply*, ReplyType> pendingReplies;
    QMap<QNetworkReply*, Card> imageDownloads;
    QVector<Card> batchResults;
    int remainingBatchCards = 0;

    // Helper methods
    void downloadCardImage(const Card &card, const QString &imageUrl);
    Card parseCardFromJson(const QJsonObject &cardJson);
    QMap<ManaType, int> parseManaSymbols(const QString &manaCost);
    ManaType determineCardColor(const QJsonArray &colorsArray);
    CardType determineCardType(const QString &typeLine);
    QVector<PropertyType> parseKeywords(const QJsonArray &keywordsArray);

    QMap<QString, QString> symbolMap;
};

#endif // CARDAPIMANAGER_H
